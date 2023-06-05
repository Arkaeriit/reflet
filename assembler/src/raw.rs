use crate::tree::AsmNode::*;
use crate::tree::*;
use crate::Assembler;
use std::collections::HashMap;

/// Expands the @constant directives into raw bytes
pub fn expand_constants(asm: &mut Assembler) {
    let mut all_constants: Vec<String> = vec![];
    let mut had_error = false;

    // The fist step is to read all the constant and put the one that are well
    // formatted into a list. They can not be processed now as their processing
    // require to borrow the Assembler that is already borrowed by the tree
    // traversing.
    let mut register_constants_in_list = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if &code[0] == "@constant" {
                    if code.len() == 2 {
                        all_constants.push(code[1].clone());
                        None
                    } else {
                        had_error = true;
                        Some(Error{msg: "Error, @constant directive should have a single argument that is its constant.".to_string(), meta: meta.clone()})
                    }
                } else {
                    None
                }
            },
            _ => None,
        }
    };

    asm.root.traverse_tree(&mut register_constants_in_list);
    if had_error {
        return;
    }

    // Then, all elements of the list are processed and the results are put in
    // a hash map for easy access.
    let mut computed_constants: HashMap<String, Result<Vec<u8>, &str>> = HashMap::new();
    for constant in all_constants {
        if !computed_constants.contains_key(&constant) {
            computed_constants.insert(constant.clone(), match asm.format_string_into_number(&constant) {
                Some(num) => Ok(num),
                None => Err("Error, unable to read number."),
            });
        }
    }

    // Lastly, the computed content of the hash map is used to replace the
    // content directives.
    let mut apply_computed_constants = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if &code[0] == "@constant" {
                    // We now that all the values have the right number of elements so no need for further checking. We also know that it is a valid key in the hash map of computed values.
                    match computed_constants.get(&code[1]).unwrap() {
                        Ok(num) => Some(Raw(num.clone())),
                        Err(s) => Some(Error{msg: s.to_string(), meta: meta.clone()}),
                    }
                } else {
                    None
                }
            },
            _ => None
        }
    };

    asm.root.traverse_tree(&mut apply_computed_constants);
}

/// Register the `@rawbytes` directives into Raw nodes
pub fn decode_raw_bytes(asm: &mut Assembler) {
    fn decoding_raw_bytes(node: &AsmNode) -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if &code[0] == "@rawbytes" {
                    // An empty rawbyte statement can be considered valid so no length checking
                    let mut data: Vec<u8> = vec![];
                    for i in 1..code.len() {
                        match u8::from_str_radix(&code[i], 16) {
                            Ok(num) => {data.push(num);},
                            Err(_) => {
                                return Some(Error{msg: format!("Error, unable to read byte {} in @rawbyte directive.", &code[i]), meta: meta.clone()});
                            },
                        }
                    }
                    Some(Raw(data))
                } else {
                    None
                }
            },
            _ => None,
        }
    }

    asm.root.traverse_tree(&mut decoding_raw_bytes);
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_constants() {
    let mut asm = Assembler::from_text("@constant 10\n@constant 0x10");
    assert_eq!(asm.set_word_size(8), None);
    expand_constants(&mut asm);
    assert_eq!(asm.root.to_string(), "0x0A \n0x10 \n")
}

#[test]
fn test_raw_bytes() {
    let mut asm = Assembler::from_text("@rawbytes 10 1 99");
    decode_raw_bytes(&mut asm);
    assert_eq!(asm.root.to_string(), "0x10 0x01 0x99 \n");
}

