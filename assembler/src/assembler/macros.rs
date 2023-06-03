use crate::assembler::*;
use crate::tree::AsmNode::*;
use crate::tree::*;

/// The content of a macro is defined by the number of arguments in need to be
/// substituted and the source code content that will replace the macro
/// invocation.
#[derive(Clone, Debug, PartialEq)]
pub struct Macro {
    number_of_arguments: usize,
    content: String,
}

impl Macro {
    /// Generate a new empty macro. Its content will be filled with new lines
    /// of source code.
    fn new(number_of_arguments: usize) -> Self {
        Macro {
            number_of_arguments: number_of_arguments,
            content: "".to_string(),
        }
    }
}

/// Traverse the source in search of `@define` lines. At those line a new macro
/// is defined and all lines until a `@end` line will be put in the macro.
/// Then, the resulting macro is registered in the assembler's state.
/// All used up lines are replaced with Empty nodes.
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

/// Search all the sources lines of the code for macro to be expanded. In those
/// cases, the content of the macro is fetched from the assembler's macro list
/// and the lines are replaced with an Inode containing the expanded macro.
pub fn expand_macros(asm: &mut Assembler) {

    let mut expand_macros_closure = | node: &AsmNode | -> Option<AsmNode> {
        // All the work is done in this function. The outer closure is needed to
        // access the macro list of the Assembler; but we need to do the
        // expansion recursively, which cannot be done from the closure and we
        // must call expand_macros inside of it.
        match node {
            Source{code, meta} => {
                match asm.macros.get(&code[0]) {
                    Some(macro_txt) => {
                        if code.len() == macro_txt.number_of_arguments + 1 {
                            // Formatting name for error reporting
                            let mut macro_name = "macro_".to_string();
                            macro_name.push_str(&code[0]);
                            // Arguments substitution
                            let mut expanded_text = macro_txt.content.clone();
                            for i in 0..macro_txt.number_of_arguments {
                                let pattern = format!("${}", i+1);
                                expanded_text = expanded_text.replace(&pattern, &code[1+i]);
                            }
                            // Result generation
                            let mut expanded_macro = Assembler::from_named_text(&expanded_text, &macro_name);
                            expanded_macro.macros = asm.macros.clone();
                            expanded_macro.macros.remove(&code[0]); // Remove the macro name to prevent infinite recursion. Instead an error will be raised when the macro is not found. Furthermore, this can be used to shadow macros or instructions.
                            expand_macros(&mut expanded_macro);
                            Some(expanded_macro.root)
                        } else {
                            let msg = format!("Error, invalid number of arguments for macro {}. Expected {}, got {}", &code[0], macro_txt.number_of_arguments, code.len() - 1);
                            Some(Error{msg, meta: meta.clone()})
                        }
                    },
                    None => None,
                }
            },
            _ => None,
        }
    };


    asm.root.traverse_tree(&mut expand_macros_closure);
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

#[test]
fn test_expand_macro_no_arg_substitution() {
    let mut assembler = Assembler::from_text("@define m 0\nx x\n@end\nm\nm\n");
    register_macros(&mut assembler);
    expand_macros(&mut assembler);
    assert_eq!(assembler.root.to_string(), "x x\nx x\n");
}

#[test]
fn test_expand_macro() {
    let mut assembler = Assembler::from_text("@define m 2\nx $1 $2\n@end\nm a A\nm b B\n");
    register_macros(&mut assembler);
    expand_macros(&mut assembler);
    assert_eq!(assembler.root.to_string(), "x a A\nx b B\n");
}

#[test]
fn test_expand_macro_recursive() {
    let mut assembler = Assembler::from_text("@define m 2
                                                  $1 $2
                                              @end
                                              @define in 1
                                                  x $1
                                              @end
                                              m in a
                                              m in b");
    register_macros(&mut assembler);
    expand_macros(&mut assembler);
    assert_eq!(assembler.root.to_string(), "x a\nx b\n");
}

