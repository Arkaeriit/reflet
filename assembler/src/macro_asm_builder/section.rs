use crate::Assembler;
use crate::tree::AsmNode::*;
use crate::tree::*;
use std::collections::HashMap;
use std::collections::HashSet;

/// Traverse the tree and register all `@section` directives.
/// Raise an error if there is more than one section of a kind
fn register_sections(asm: &mut Assembler) {
    let mut known_sections = HashSet::<String>::new();

    let mut registering_sections = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if code[0].as_str() == "@section" {
                    if code.len() == 2 {
                        if known_sections.contains(&code[1]) {
                            Some(Error{msg: format!("Error, section {} has already been declared.\n", &code[1]), meta: meta.clone()})
                        } else {
                            known_sections.insert(code[1].clone());
                            Some(Section{name: code[1].clone(), content: Vec::<AsmNode>::new()})
                        }
                    } else {
                        Some(Error{msg: format!("Error, directive {} takes a name as its only argument.\n", &code[0]), meta: meta.clone()})
                    }
                } else {
                    None
                }
            },
            _ => None,
        }
    };

    asm.root.traverse_tree(&mut registering_sections);
}

/// Read all the code that should be in sections and return a map of code
/// against section names.
fn code_in_sections(asm: &mut Assembler) -> HashMap<String, Vec<AsmNode>> {
    let mut in_section = false;
    let mut ret = HashMap::<String, Vec<AsmNode>>::new();
    let mut new_section_name = String::new();
    let mut new_section_code = Vec::<AsmNode>::new();

    let mut reading_code_in_section = | node: &AsmNode | -> Option<AsmNode> {
        if in_section {
            match node {
                Source{code, meta} => {
                    match code[0].as_str() {
                        "@end-section" => {
                            in_section = false;
                            match ret.get_mut(&new_section_name) {
                                Some(code) => {
                                    code.append(&mut new_section_code); // This clears the new_session code, making it ready for the next section code to encounter.
                                },
                                None => {
                                    ret.insert(new_section_name.clone(), new_section_code.clone());
                                    new_section_code.clear();
                                }
                            }
                            Some(Empty)
                        },
                        "@in-section" => {
                            Some(Error{msg: format!("Error, directives @in-section can't be nested.\n"), meta: meta.clone()})
                        },
                        _ =>  {
                            new_section_code.push(Source{code: code.to_vec(), meta: meta.clone()});
                            Some(Empty)
                        }
                    }
                },
                n => {
                    new_section_code.push(n.clone());
                    Some(Empty)
                }
            }
        } else {
            match node {
                Source{code, meta} => {
                    match code[0].as_str() {
                        "@end-section" => {
                            Some(Error{msg: format!("Error, directives @end-section can only be put after a @in-section directive.\n"), meta: meta.clone()})
                        },
                        "@in-section" => {
                            if code.len() == 2 {
                                in_section = true;
                                new_section_name = code[1].clone();
                                Some(Empty)
                            } else {
                                Some(Error{msg: format!("Error, directive {} takes a name as its only argument.\n", &code[0]), meta: meta.clone()})
                            }
                        },
                        _ => None,
                    }
                },
                _ => None,
            }
        }
    };

    asm.root.traverse_tree(&mut reading_code_in_section);
    if in_section {
        asm.root.error_on_top(format!("Error, section code {} is not closed with `@end-section` directive", new_section_name));
    }
    ret
}

/// Use the map to fill sections. Remove each section from the map once it has
/// been used.
fn fill_sections(asm: &mut Assembler, mut map: HashMap<String, Vec<AsmNode>>) {
    let mut filling_section = |node: &AsmNode| -> Option<AsmNode> {
        match node {
            Section{name, content: _} => {
                match map.remove(name) {
                    Some(code) => Some(Inode(code)),
                    None => Some(Empty), // We don't get any code in that section. It's no problem.
                }
            },
            _ => None,
        }
    };

    asm.root.traverse_tree(&mut filling_section);
    if map.len() != 0 {
        let mut err_msg = format!("Error, {} sections have code in them but have not been declared: ", map.len());
        for section in map.keys() {
            err_msg.push_str(&format!("{}, ", section));
        }
        err_msg.pop(); // Remove two last char to replace with colon
        err_msg.pop();
        err_msg.push_str(".\n");
        asm.root.error_on_top(err_msg);
    }
}

/// Register all section declaration, read code in sections, and then place it
/// at the declarations.
pub fn handle_sections(asm: &mut Assembler) {
    register_sections(asm);
    let map = code_in_sections(asm);
    fill_sections(asm, map);
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_register_sections() {
    let mut assembler = Assembler::from_text("@section OK\n@section NOK\n@section NOK\n");
    register_sections(&mut assembler);
    assert_eq!(assembler.root.to_string().as_str(), "Section OK\nSection NOK\n! Error, section NOK has already been declared.\n file: ./__asm_init line: 3 raw_line: @section NOK\n");
}

#[test]
fn test_code_in_section() {
    let mut assembler = Assembler::from_text("@in-section A\naa aa\n@end-section\nnot in section\n@in-section B\nbb bb\n@end-section\n@in-section A\naaaa\n@end-section\n");
    let mut map = code_in_sections(&mut assembler);
    assert_eq!(assembler.root.to_string().as_str(), "not in section\n");
    let sec_a = map.remove("A").expect("");
    let sec_b = map.remove("B").expect("");
    assert_eq!(map.len(), 0);
    let root_a = Inode(sec_a);
    assert_eq!(root_a.to_string().as_str(), "aa aa\naaaa\n");
    let root_b = Inode(sec_b);
    assert_eq!(root_b.to_string().as_str(), "bb bb\n");
}

#[test]
fn test_handle_sections() {
    let mut assembler = Assembler::from_text("@section B\n@in-section A\naa aa\n@end-section\nnot in section\n@in-section B\nbb bb\n@end-section\n@in-section A\naaaa\n@end-section\n@section A\n");
    handle_sections(&mut assembler);
    assert_eq!(assembler.root.to_string().as_str(), "bb bb\nnot in section\naa aa\naaaa\n");

    assembler = Assembler::from_text("@in-section A\naa aa\n@end-section\nnot in section\n@in-section B\nbb bb\n@end-section\n@in-section A\naaaa\n@end-section\n@section A\n");
    handle_sections(&mut assembler);
    assert_eq!(assembler.root.to_string().as_str(), "not in section\naa aa\naaaa\n! Error, 1 sections have code in them but have not been declared: B.\n file: !!! line: 0 raw_line: !!!\n");
}

