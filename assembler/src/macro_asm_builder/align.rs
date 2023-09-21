use crate::Assembler;
use crate::tree::AsmNode::*;
use crate::tree::*;
use AlignKind::*;
use crate::utils::format_string_into_number;


/// The different type of alignment we can require
#[derive(Debug, PartialEq, Eq, Clone)]
pub enum AlignKind {
    /// Align to a certain modulus of address
    AlignTo(usize),

    /// Pads up to a certain address
    PadUntil(usize),
}

/// Replace the `@align` directives with Align nodes.
pub fn register_align(asm: &mut Assembler) {
    fn registering_aligns(node: &AsmNode) -> Option<AsmNode> {
        match node {
            Source{code, meta} => match code[0].as_str() {
                "@align" | "@pad-until" => {
                    if code.len() == 2 {
                        match format_string_into_number(&code[1]) {
                            Some((num, false)) => {
                                let try_num_usize: Result<usize, _> = num.try_into();
                                match try_num_usize {
                                    Ok(num_usize) => match code[0].as_str() {
                                        "@align" => Some(Align{kind: AlignTo(num_usize), meta: meta.clone()}),
                                        "@pad-until" => Some(Align{kind: PadUntil(num_usize), meta: meta.clone()}),
                                        _ => {panic!("{} should not be left when writing align", &code[0])},
                                    },
                                    Err(_) => Some(Error{msg: format!("Error, {} is too big to be used in {} directive.", num, &code[0]) , meta: meta.clone()}),
                                }
                            },
                            Some((_, true)) | None => Some(Error{msg: format!("Error, {} directive should take a single positive numeric argument.", &code[0]), meta: meta.clone()}),
                        }
                    } else {
                        Some(Error{msg: format!("Error, {} directive should take a single numeric argument.", &code[0]), meta: meta.clone()})
                    }
                },
                _ => {
                    None
                },
            },
            _ => None
        }
    }

    asm.root.traverse_tree(&mut registering_aligns);
}

/// Replace the Align node with some Raw node of the needed size to ensure
/// proper alignment. This should only be done when the tree contains only node
/// of know size such as Raw nodes.
pub fn expand_align(asm: &mut Assembler) {
    let align_pattern = asm.align_pattern.clone();
    let mut current_offset = asm.start_address;

    let mut expanding_align = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Raw(data) => {
                current_offset = current_offset + data.len();
                None
            },
            Align{kind, meta} => match kind {
                AlignTo(size) => {
                    let mut i = 0;
                    let mut padding: Vec<u8> = vec![];
                    while current_offset % size != 0 {
                        current_offset = current_offset + 1;
                        padding.push(align_pattern[i % align_pattern.len()]);
                        i = i + 1;
                    }
                    Some(Raw(padding))
                },
                PadUntil(addr) => {
                    if current_offset <= *addr {
                        let mut i = 0;
                        let mut padding: Vec<u8> = vec![];
                        while current_offset < *addr {
                            current_offset += 1;
                            padding.push(align_pattern[i % align_pattern.len()]);
                            i += 1;
                        }
                        Some(Raw(padding))
                    } else {
                        Some(Error{msg: format!("Trying to pad until address {} but we are already at address {}.", addr, current_offset), meta: meta.clone()})
                    }
                },
            },
            Label{name: _name,  is_definition, meta: _meta} => {
                if !is_definition {
                    current_offset = current_offset + asm.wordsize;
                }
                None
            },
            Error{msg: _, meta: _} => None,
            x => {
                Some(Error{msg: format!("There is a bug in the assembler. A node should not have been left while in expand_align: {}.", x.to_string()), meta: Metadata{raw: "!!!".to_string(), source_file: "!!!".to_string(), line: !0}})
            },
        }
    };

    asm.root.traverse_tree(&mut expanding_align);
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_register_align() {
    let mut asm = Assembler::from_text("@align 3\n@align 10");
    register_align(&mut asm);
    assert_eq!(asm.root.to_string(), "Align to 3 bytes\nAlign to 10 bytes\n");
}

#[test]
fn test_expand_align() {
    let mut asm = Assembler::from_text("@rawbytes 10\n@align 3\n@rawbytes 11 12\n@align 10");
    asm.align_pattern = vec![0, 1];
    super::raw::decode_raw_bytes(&mut asm);
    register_align(&mut asm);
    expand_align(&mut asm);
    assert_eq!(asm.root.to_string(),"0x10 \n0x00 0x01 \n0x11 0x12 \n0x00 0x01 0x00 0x01 0x00 \n");
}

#[test]
fn test_pad_until() {
    let mut asm = Assembler::from_text("@pad-until 5\n@rawbytes 10\n@align 7");
    asm.align_pattern = vec![0, 1];
    super::raw::decode_raw_bytes(&mut asm);
    register_align(&mut asm);
    expand_align(&mut asm);
    assert_eq!(asm.root.to_string(),"0x00 0x01 0x00 0x01 0x00 \n0x10 \n0x00 \n");
}

#[test]
fn test_pad_fail() {
    let mut asm = Assembler::from_text("@pad-until 5\n@pad-until 5\n@pad-until 4");
    asm.align_pattern = vec![0, 1];
    super::raw::decode_raw_bytes(&mut asm);
    register_align(&mut asm);
    expand_align(&mut asm);
    assert_eq!(asm.root.to_string(),"0x00 0x01 0x00 0x01 0x00 \n\n! Trying to pad until address 4 but we are already at address 5. file: ./__asm_init line: 3 raw_line: @pad-until 4\n");
}
