use crate::Assembler;
use crate::tree::AsmNode::*;
use crate::tree::*;

/// Replace the `@align` directives with Align nodes.
pub fn register_align(asm: &mut Assembler) {
    fn registering_aligns(node: &AsmNode) -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if &code[0] == "@align" {
                    if code.len() == 2 {
                        match usize::from_str_radix(&code[1], 10) {
                            Ok(num) => Some(Align(num)),
                            Err(_) => Some(Error{msg: "Error, @align directive should take a single numeric argument in base 10.".to_string(), meta: meta.clone()}),
                        }
                    } else {
                        Some(Error{msg: "Error, @align directive should take a single numeric argument in base 10.".to_string(), meta: meta.clone()})
                    }
                } else {
                    None
                }
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
            Align(size) => {
                let mut i = 0;
                let mut padding: Vec<u8> = vec![];
                while current_offset % size != 0 {
                    current_offset = current_offset + 1;
                    padding.push(align_pattern[i % align_pattern.len()]);
                    i = i + 1;
                }
                Some(Raw(padding))
            },
            Label{name: _name,  is_definition, meta: _meta} => {
                if !is_definition {
                    current_offset = current_offset + asm.wordsize;
                }
                None
            },
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

