use crate::assembler::*;
use crate::tree::AsmNode::*;
use crate::tree::*;

#[derive(Clone, Debug, PartialEq)]
pub struct Macro {
    number_of_arguments: usize,
    content: String,
}

impl Macro {
    fn new(number_of_arguments: usize) -> Self {
        Macro {
            number_of_arguments: number_of_arguments,
            content: "".to_string(),
        }
    }
}

pub fn register_macros(asm: &mut Assembler) {
    let mut in_macro = false;
    let mut new_macro = Macro::new(0);
    let mut new_macro_name = "".to_string();

    let mut register_macros_closure = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                match code[0].as_str() {
                    "@define" => {
                        if in_macro {
                            Some(Error{msg: "Error, macro definitions can't be nested".to_string(), meta: meta.clone()})
                        } else {
                            if code.len() == 3 {
                                in_macro = true;
                                new_macro_name = code[1].clone();
                                let number_of_arguments: Result<usize, _> = code[2].parse();
                                match number_of_arguments {
                                    Ok(x) => {
                                        new_macro = Macro::new(x);
                                        Some(Empty)
                                    },
                                    Err(_) =>
                                        Some(Error{msg: "Error, macro definitions should have the form `@define <macro name> <number_of_arguments>`".to_string(), meta: meta.clone()})
                                }
                            } else {
                                Some(Error{msg: "Error, macro definitions should have the form `@define <macro name> <number_of_arguments>`".to_string(), meta: meta.clone()})
                            }
                        }
                    },
                    "@end" => {
                        if in_macro {
                            in_macro = false;
                            asm.macros.insert(new_macro_name.clone(), new_macro.clone());
                            Some(Empty)
                        } else {
                            Some(Error{msg: "Error, @end should only be used to end macro definitions".to_string(), meta: meta.clone()})
                        }
                    },
                    _ => {
                        if in_macro {
                            new_macro.content.push_str(&meta.raw);
                            new_macro.content.push_str("\n");
                            Some(Empty)
                        } else {
                            None
                        }
                    },
                }
            },
            _ => None,
        }
    };

    asm.root.traverse_tree(&mut register_macros_closure);
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_register_macros() {
    let mut assembler = Assembler::from_text("@define my_macro 3\nmacromacro\ntxttxt\n@end");
    let expected_hash_map = HashMap::from([
        ("my_macro".to_string(), Macro{number_of_arguments: 3, content: "macromacro\ntxttxt\n".to_string()}),
    ]);

    register_macros(&mut assembler);
    assert_eq!(expected_hash_map, assembler.macros);
}

