use std::path::Path;
use std::fs;
use crate::tree::AsmNode::*;
use crate::tree::*;
use crate::assembly_source::parse_source;
use crate::Assembler;

/// Traverse the tree searching for @import directive in source nodes. When
/// found, they are replaced with the content of the file to be imported.
/// Return true if some text was included.
pub fn include_source(asm: &mut Assembler) -> bool {
    let mut included_source = false;

    let mut _include_source = |node: &AsmNode| -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if code[0] == "@import" {
                    if code.len() == 2 {
                        let origin_dir = match Path::new(&meta.source_file).parent() {
                            Some(x) => x,
                            None => {
                                println!("Error, invalid path {}", &meta.source_file);
                                Path::new(".")
                            },
                        };
                        match origin_dir.to_str() {
                            Some(path) => {
                                let target_path = 
                                    if &code[1].as_str()[0..1] == "/" {
                                        code[1].clone()
                                    } else {
                                        let mut target_path_mut = path.to_string();
                                        target_path_mut.push_str("/");
                                        target_path_mut.push_str(&code[1]);
                                        target_path_mut
                                    };
                                match fs::read_to_string(&target_path) {
                                    Ok(txt) => {
                                        let ret_node = parse_source(&txt, &target_path);
                                        included_source = true;
                                        Some(ret_node)
                                    },
                                    Err(_) => {
                                        Some(Error{msg: "Error, unable to open file for @import directive.".to_string(), meta: meta.clone()})
                                    }
                                }
                            },
                            None => {
                                Some(Error{msg: "Error, unable to open file for @import directive.\nAll path should be valid UTF-8".to_string(), meta: meta.clone()})
                            },
                        }
                    } else {
                        Some(Error{msg: "Error, @import directive should have a single argument that is the path of the file to import. If there is whitespace in the path, put it between quotes.".to_string(), meta: meta.clone()})
                    }
                } else {
                    None
                }
            }
            _ => None
        }
    };

    asm.root.traverse_tree(&mut _include_source);
    included_source
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_include() {
    let mut assembler = Assembler::from_file("tests/include/root");
    let mut need_to_run = true;
    while need_to_run {
        need_to_run = include_source(&mut assembler);
    }
    assert_eq!(assembler.root.to_string(), "a\na\nd\nd\nb\na\na\n");
}

