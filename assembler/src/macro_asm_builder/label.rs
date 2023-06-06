use crate::Assembler;
use crate::tree::AsmNode::*;
use crate::tree::*;
use std::collections::HashMap;

/// Register directives `@label` and `@labref` into Label nodes.
pub fn register_labels(asm: &mut Assembler) {
    fn registering_labels(node: &AsmNode) -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                match code[0].as_str() {
                    "@label" | "@labref" => {
                        if code.len() == 2 {
                            Some(Label{name: code[1].clone(), is_definition: &code[0] == "@label", meta: meta.clone()})
                        } else {
                            Some(Error{msg: format!("Error, directive {} takes a label as its only argument.\n", &code[0]), meta: meta.clone()})
                        }
                    },
                    _ => None,
                }
            },
            _ => None,
        }
    }

    asm.root.traverse_tree(&mut registering_labels);
}

/// Replace label references to their addresses. Only works when all the nodes
/// in the tree have known sizes.
pub fn expand_labels(asm: &mut Assembler) {
    // Compute the offset of each defined label.
    let mut label_table: HashMap<String, usize> = HashMap::new();
    let mut current_offset = asm.start_address;
    let mut reading_labels_offsets = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Label{name, is_definition, meta} => {
                if *is_definition {
                    if label_table.contains_key(name) {
                        Some(Error{msg: format!("Error, label {} is defined multiple times.\n", &name), meta: meta.clone()})
                    } else {
                        label_table.insert(name.clone(), current_offset);
                        None
                    }
                } else {
                    current_offset = current_offset+asm.wordsize;
                    None
                }
            },
            Raw(data) => {
                current_offset = current_offset + data.len();
                None
            },
            Error{msg: _, meta: _} => None,
            x => {
                Some(Error{msg: format!("There is a bug in the assembler. A node should not have been left while in expand_labels: {}.", x.to_string()), meta: Metadata{raw: "!!!".to_string(), source_file: "!!!".to_string(), line: !0}})
            },
        }
    };
    asm.root.traverse_tree(&mut reading_labels_offsets);

    // Compute the raw values to be used as reference
    let mut computed_label_references: HashMap<String, Result<Vec<u8>, String>> = HashMap::new();
    for label in label_table.keys() {
        computed_label_references.insert(label.clone(), match asm.format_number(*label_table.get(&label.clone()).unwrap() as u128) { // 😬
            Some(num) => Ok(num),
            None => Err(format!("Error, unable to compute offset for label {} which is {:X}. It's probably because the offset is larger that the valid address range.", &label, label_table.get(&label.clone()).unwrap())),
        });
    }

    // Replace the reference with the raw values
    let mut expanding_label_references = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Label{name, is_definition, meta} => {
                if *is_definition {
                    return None
                }
                match computed_label_references.get(&name.clone()) {
                    Some(offset_result) => match offset_result {
                        Ok(num) => Some(Raw(num.clone())),
                        Err(msg) => Some(Error{msg: msg.to_string(), meta: meta.clone()}),
                    },
                    None => Some(Error{msg: format!("Error, label {} is used but never defined.", &name), meta: meta.clone()}),
                }
            },
            _ => None, // No need for too much testing as some of it have been made in the previous closure.
        }
    };
    asm.root.traverse_tree(&mut expanding_label_references);
}

/// Generate a nice list showing the address of each label
pub fn label_dump(asm: &mut Assembler) -> String {
    let mut ret = String::new();
    let mut current_offset = 0;
    let mut dumping_labels = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Label{name, is_definition, meta: _} => {
                if *is_definition {
                    ret.push_str(&format!("{}: 0x{:X}\n", &name, current_offset));
                } else {
                    current_offset = current_offset+asm.wordsize;
                }
            },
            Raw(data) => {
                current_offset = current_offset + data.len();
            },
            Error{msg: _, meta: _} => {
                return None;
            },
            x => {
                ret.push_str(&format!("The size of the node {} is not known. Thus, we can't make a label dump.", &x.to_string()));
            },
        }
        None
    };

    asm.root.traverse_tree(&mut dumping_labels);
    ret
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_register_labels() {
    let mut asm = Assembler::from_text("@labref lab\n@label lab");
    register_labels(&mut asm);
    assert_eq!(asm.root.to_string(), "Reference to label lab\nDefinition of label lab\n");
}

#[test]
fn test_raw_bytes() {
    let mut asm = Assembler::from_text("@label start\n@labref end\n@labref start\n@label end");
    assert_eq!(asm.set_word_size(16), None);
    register_labels(&mut asm);
    expand_labels(&mut asm);
    assert_eq!(asm.root.to_string(), "Definition of label start\n0x04 0x00 \n0x00 0x00 \nDefinition of label end\n");
}

