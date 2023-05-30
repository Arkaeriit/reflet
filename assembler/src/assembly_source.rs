use crate::tree::AsmNode;
use crate::tree::Metadata;
use crate::tree::AsmNode::*;

/// Takes some text and convert it into an AsmNode containing each lines parsed
pub fn parse_source(text: &str, meta_path: &str) -> AsmNode {
    let mut nodes: Vec<AsmNode> = vec![];
    for (parsed_line, meta_line, meta_raw) in parse_text(text) {
        let node: AsmNode = Source{code: parsed_line, meta: Metadata{raw: meta_raw, source_file: meta_path.to_string(), line: meta_line}};
        nodes.push(node);
    }
    Inode(nodes)
}

/* ---------------------------- Helper functions ---------------------------- */

/// Takes as input a line of assembly source code and return a list of each
/// words in it such as "  and R3; Do fancy math" -> ["and", "R3"]
fn parse_line(line: &str) -> Vec<String> {
    // Removing text after a semi-colon
    let mut line_as_char: Vec<char> = vec![];
    for c in line.chars() {
        if c == ';' {
            break;
        }
        line_as_char.push(c);
    }
    line_as_char.push('\n'); // Ensure that we capture the last word
    // Splitting line in words
    let mut ret: Vec<String> = vec![];
    let mut in_whitespace = true;
    let mut word_start = 0;
    for i in 0..line_as_char.len() {
        match in_whitespace {
            true => {
                if !line_as_char[i].is_whitespace() {
                    in_whitespace = false;
                    word_start = i;
                }
            },
            false => {
                if line_as_char[i].is_whitespace() {
                    in_whitespace = true;
                    let word_to_add = &line_as_char[word_start..i];
                    ret.push(word_to_add.iter().cloned().collect::<String>());
                }
            },
        };
    }
    ret
}

/// Take a bunch of text as input and cut it in a lines which are then parsed
/// Empty lines are ignored
/// The relative position of all lines is also returned along with their raw
/// values
fn parse_text(text: &str) -> Vec<(Vec<String>, usize, String)> {
    let mut ret: Vec<(Vec<String>, usize, String)> = vec![];
    let mut line_index = 1;
    for line in text.split("\n") {
        let parsed = parse_line(line);
        if parsed.len() != 0 {
            ret.push((parse_line(line), line_index, line.to_string()));
        }
        line_index += 1;
    }
    ret
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_parse_line() {
    fn test_eq(line: &str, split: Vec<&str>) {
        let converted = parse_line(line);
        assert_eq!(converted, split);
    }

    test_eq("abc def", vec!["abc", "def"]);
    test_eq("  abc def  ", vec!["abc", "def"]);
    test_eq("  abc def;bla bla  ", vec!["abc", "def"]);
    test_eq("  abc def ; bla bla  ", vec!["abc", "def"]);
}

#[test]
fn test_parse_text() {
    fn test_eq(text: &str, split: Vec<Vec<&str>>) {
        let converted = parse_text(text);
        let mut converted_vec: Vec<Vec<String>> = vec![];
        for (elem, _meta1, _meta2) in converted {
            converted_vec.push(elem);
        }
        assert_eq!(converted_vec, split);
    }

    test_eq("abc def\na b;lol\n", vec![vec!["abc", "def"], vec!["a", "b"]]);
    test_eq("abc def ; lol\na b;lol", vec![vec!["abc", "def"], vec!["a", "b"]]);
    test_eq("abc def ; lol\n\n\na b\r\n", vec![vec!["abc", "def"], vec!["a", "b"]]);
    test_eq("abc def\na b", vec![vec!["abc", "def"], vec!["a", "b"]]);
}

#[test]
fn test_parse_source() {
    assert_eq!(parse_source("a b\nc d", "path"), Inode(vec![
            Source{code: vec!["a".to_string(), "b".to_string()], meta: Metadata{raw: "a b".to_string(), source_file: "path".to_string(), line: 1}},
            Source{code: vec!["c".to_string(), "d".to_string()], meta: Metadata{raw: "c d".to_string(), source_file: "path".to_string(), line: 2}}]));
}

