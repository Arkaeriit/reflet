use crate::Assembler;
use crate::tree::AsmNode::*;
use crate::tree::*;
use std::collections::HashMap;

/* -------------------------------- Constants ------------------------------- */

/// Traverse the tree to register the `@constant` directive.
/// Return a map of the keys of the constants to their values.
fn register_constants(asm: &mut Assembler) -> HashMap<String, Vec<String>> {
    let mut ret = HashMap::<String, Vec<String>>::new();

    let mut registering_constants = |node: &AsmNode| -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if code[0].as_str() == "@constant" {
                    let name = &code[1];
                    if code.len() < 3 {
                        return Some(Error{msg: format!("Error, directive {} should take as argument a constant name and at least a replacement value.\n", &code[0]), meta: meta.clone()});
                    }
                    if ret.contains_key(name) {
                        return Some(Error{msg: format!("Error, constant {} has already been defined.\n", name), meta: meta.clone()});
                    }
                    if contains_math_chars(name) {
                        return Some(Error{msg: format!("Error, name of constant {} contains not allowed chars (any of {:?}).\n", name, MATH_CHARS), meta: meta.clone()});
                    }

                    let value = code[2..].to_vec();
                    let _ = ret.insert(name.to_string(), value);
                    Some(Empty)
                } else {
                    None
                }
            },
            _ => None,
        }
    };

    asm.root.traverse_tree(&mut registering_constants);
    ret
}

/// Traverse the tree and replace text with are constant names with appropriate
/// values.
fn apply_constants(asm: &mut Assembler, map: &HashMap<String, Vec<String>>) {
    let mut applying_constants = |node: &AsmNode| -> Option<AsmNode> {
        if let Source{code, meta} = node {

            let mut new_source = Vec::<String>::new();
            for txt in code {
                if let Some(substitution) = map.get(txt) {
                    for word in substitution {
                        new_source.push(word.to_string());
                    }
                } else {
                    new_source.push(txt.to_string());
                }
            }
            Some(Source{code: new_source, meta: meta.clone()})

        } else {
            None
        }
    };

    asm.root.traverse_tree(&mut applying_constants);
}

/// Register all constants and apply them
pub fn handle_constants(asm: &mut Assembler) {
    let map = register_constants(asm);
    apply_constants(asm, &map);
}

/* ---------------------------------- Utils --------------------------------- */

const MATH_CHARS: [char; 6] = ['+', '-', '*', '/', '(', ')'];

/// Return true if a string got any chars from the `MATH_CHARS`.
fn contains_math_chars(s: &str) -> bool {
    s.contains(MATH_CHARS)
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_register_constants() {
    let mut assembler = Assembler::from_text("@constant name 123\n@constant name_2 234 345\nnormal text\n@constant name error\n");
    let map = register_constants(&mut assembler);
    assert_eq!(assembler.root.to_string().as_str(), "normal text\n! Error, constant name has already been defined.\n file: ./__asm_init line: 4 raw_line: @constant name error\n");
    assert_eq!(map, HashMap::from([
          ("name".to_string(),   vec!["123".to_string()]),
          ("name_2".to_string(), vec!["234".to_string(), "345".to_string()]),
    ]));
}

#[test]
fn test_contains_math_chars() {
    assert_eq!(contains_math_chars("abc+d"), true);
    assert_eq!(contains_math_chars("abcd"), false);
}

#[test]
fn test_apply_constants() {
    let mut assembler = Assembler::from_text("@constant name 123\n@constant name_2 234 345\nnormal text\nname name_2  name\n");
    let map = register_constants(&mut assembler);
    apply_constants(&mut assembler, &map);
    assert_eq!(assembler.root.to_string().as_str(), "normal text\n123 234 345 123\n");
    assert_eq!(map, HashMap::from([
          ("name".to_string(),   vec!["123".to_string()]),
          ("name_2".to_string(), vec!["234".to_string(), "345".to_string()]),
    ]));
}

