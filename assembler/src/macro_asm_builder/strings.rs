use crate::Assembler;
use crate::tree::AsmNode::*;
use crate::tree::*;

/// Register the directives of the `@string` family
pub fn register_strings(asm: &mut Assembler) {
    fn registering_strings(node: &AsmNode) -> Option<AsmNode> {
        match node {
            Source{code, meta} => match code[0].as_str() {
                "@string" | "@string-0" | "@string-nl" | "@string-nl-0" => {
                    if code.len() == 2 {
                        Some(Raw(string_registration(&code)))
                    } else {
                        Some(Error{msg: format!("Error, {} directive should be in the form `{} \"txt\".", &code[0], &code[0]), meta: meta.clone()})
                    }
                },
                _ => None,
            },
            _ => None,
        }
    }
    asm.root.traverse_tree(&mut registering_strings);
}

/// Register a valid string macro
fn string_registration(code: &Vec<String>) -> Vec<u8> {
    let mut ret = code[1].as_bytes().to_vec(); 
    match code[0].as_str() {
        "@string-0" => {
            ret.push(0);
        },
        "@string-nl" => {
            ret.push('\n' as u8);
        },
        "@string-nl-0" => {
            ret.push('\n' as u8);
            ret.push(0);
        },
        "@string" => {},
        _ => {
            panic!("Error, invalid string identifier {}", &code[0]);
        },
    }
    ret
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_string_registration() {
    assert_eq!(string_registration(&vec!["@string".to_string(), "test test".to_string()]), vec![0x74, 0x65, 0x73, 0x74, 0x20, 0x74, 0x65, 0x73, 0x74]);
    assert_eq!(string_registration(&vec!["@string-nl-0".to_string(), "test test".to_string()]), vec![0x74, 0x65, 0x73, 0x74, 0x20, 0x74, 0x65, 0x73, 0x74, 0x0A, 0x0]);
}

#[test]
fn test_register_strings() {
    let mut asm = Assembler::from_text("@string-0 t\n@string-nl \"tt\"\n");
    register_strings(&mut asm);
    assert_eq!(asm.root.to_string(), "0x74 0x00 \n0x74 0x74 0x0A \n");
}

