use crate::Assembler;
use crate::tree::AsmNode::*;
use crate::tree::*;
use std::collections::HashMap;

/* -------------------------------- Constants ------------------------------- */

/// Traverse the tree to register the `@define` directive.
/// Update the assembler's map.
fn register_define(asm: &mut Assembler) {

    let mut registering_define = |node: &AsmNode| -> Option<AsmNode> {
        match node {
            Source{code, meta} => {
                if code[0].as_str() == "@define" {
                    let name = &code[1];
                    if code.len() < 3 {
                        return Some(Error{msg: format!("Error, directive {} should take as argument a define name and at least a replacement value.\n", &code[0]), meta: meta.clone()});
                    }
                    if asm.defines.contains_key(name) {
                        return Some(Error{msg: format!("Error, define {} has already been defined.\n", name), meta: meta.clone()});
                    }
                    if contains_math_chars(name) {
                        return Some(Error{msg: format!("Error, name of define {} contains not allowed chars (any of {:?}).\n", name, MATH_CHARS), meta: meta.clone()});
                    }

                    let value = code[2..].to_vec();
                    let _ = asm.defines.insert(name.to_string(), value);
                    Some(Empty)
                } else {
                    None
                }
            },
            _ => None,
        }
    };

    asm.root.traverse_tree(&mut registering_define);
}

/// Replace defined constant with their expanded values in a line of text.
/// Return the new words and true if at list a word have been replaced.
fn apply_define_in_line(line: &[String], map: &HashMap<String, Vec<String>>) -> (Vec<String>, bool) {
    let mut applied_define = false;

    let mut new_source = Vec::<String>::new();
    for txt in line {
        if let Some(substitution) = map.get(txt) {
            applied_define = true;
            for word in substitution {
                new_source.push(word.to_string());
            }
        } else {
            new_source.push(txt.to_string());
        }
    }
    (new_source, applied_define)
}

/// Traverse the tree and replace text with are define names with appropriate
/// values. Return true if a defines have been expanded.
fn apply_define(asm: &mut Assembler) -> bool {
    let mut applied_define = false;

    let mut applying_define = |node: &AsmNode| -> Option<AsmNode> {
        if let Source{code, meta} = node {
            let (new_source, applied_a_define) = apply_define_in_line(&code, &asm.defines);
            if applied_define {
                applied_define = true;
            }

            Some(Source{code: new_source, meta: meta.clone()})

        } else {
            None
        }
    };

    asm.root.traverse_tree(&mut applying_define);
    applied_define
}

/// Register all define and apply them. Return true is a define have been
/// applied.
pub fn handle_define(asm: &mut Assembler) -> bool {
    register_define(asm);
    apply_define(asm)
}

/* ---------------------------------- Utils --------------------------------- */

/// Return true if a string got any chars from the `MATH_CHARS`.
fn contains_math_chars(s: &str) -> bool {
    s.contains(MATH_CHARS)
}

/// Return true if the element is in the slice
fn is_in<T: Eq>(a: T, set: &[T]) -> bool {
    for elem in set {
        if a == *elem {
            return true;
        }
    }
    false
}

/* ---------------------------------- Math ---------------------------------- */

const MATH_CHARS: [char; 6] = ['+', '-', '*', '/', '(', ')'];

enum MathValue {
    Value(usize), // TODO: float?
    SubExpression(Box<MathOp>),
}

enum MathOp {
    Add(MathValue, MathValue),
    Sub(MathValue, MathValue),
    Mult(MathValue, MathValue),
    Div(MathValue, MathValue),
}

/// Represent the kind of text that makes up a math expression.
#[derive(Debug, PartialEq)]
enum MathText {
    Name(String), // With a bit of work, I could use &str or the like, but as math constants are probably not going to be used very often, performance is not a great concern.
    Op(char),
}
use MathText::*;

/// Tokenise a line of math expression into a vector of `MathText`.
fn math_token(s: &str) -> Vec<MathText> {
    let mut ret = Vec::<MathText>::new();
    let mut new_name = String::new(); // Word that we are writing

    for c in s.chars() {
        if is_in(c, &MATH_CHARS) {
            if new_name.as_str() != "" { // We were writing a work
                ret.push(Name(new_name.clone()));
                new_name.clear();
            }
            ret.push(Op(c));
        } else {
            new_name.push(c);
        }
    }

    if new_name.as_str() != "" { // We were writing a work
        ret.push(Name(new_name.clone()));
    }
    ret
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_register_define() {
    let mut assembler = Assembler::from_text("@define name 123\n@define name_2 234 345\nnormal text\n@define name error\n");
    register_define(&mut assembler);
    assert_eq!(assembler.root.to_string().as_str(), "normal text\n! Error, define name has already been defined.\n file: ./__asm_init line: 4 raw_line: @define name error\n");
    assert_eq!(assembler.defines, HashMap::from([
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
fn test_apply_define() {
    let mut assembler = Assembler::from_text("@define name 123\n@define name_2 234 345\nnormal text\nname name_2  name\n");
    register_define(&mut assembler);
    apply_define(&mut assembler);
    assert_eq!(assembler.root.to_string().as_str(), "normal text\n123 234 345 123\n");
    assert_eq!(assembler.defines, HashMap::from([
          ("name".to_string(),   vec!["123".to_string()]),
          ("name_2".to_string(), vec!["234".to_string(), "345".to_string()]),
    ]));
}

#[test]
fn test_math_token() {
    let math_line = "+4/88*toto";
    assert_eq!(math_token(math_line), vec![Op('+'), Name("4".to_string()), Op('/'), Name("88".to_string()), Op('*'), Name("toto".to_string())]);
}

