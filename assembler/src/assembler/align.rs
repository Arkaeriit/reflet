use crate::assembler::*;
use crate::tree::AsmNode::*;
use crate::tree::*;

/// Replace the `@align` directives with Align nodes.
fn register_align(asm: &mut Assembler) {
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

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_register_align() {
    let mut asm = Assembler::from_text("@align 3\n@align 10");
    register_align(&mut asm);
    assert_eq!(asm.root.to_string(), "Align to 3 bytes\nAlign to 10 bytes\n");
}

