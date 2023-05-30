
#[derive(Debug, PartialEq, Eq, Clone)]
pub struct Metadata {
    pub raw: String,
    pub source_file: String,
    pub line: usize,
}

#[derive(Debug, PartialEq, Eq)]
pub enum AsmNode {
    Empty,
    Inode(Vec<AsmNode>),
    Source{code: Vec<String>, meta: Metadata},
    Error{msg: String, meta: Metadata},
}

impl AsmNode {
    /// Run a function that can transform each leaf node of the tree if it
    /// is not empty. The function should return `None` if the leaf should stay
    /// untouched and `Some(newNode)` to change the node
    pub fn traverse_tree(&mut self, f: &dyn Fn(&AsmNode) -> Option<AsmNode>) {
        match self {
            Self::Empty => {},
            Self::Inode(nodes) => {
                for node in nodes {
                    node.traverse_tree(f);
                }
            },
            _ => if let Some(node) = f(self) {
                *self = node;
            },
        }
    }
}

impl std::string::ToString for AsmNode {
    fn to_string(&self) -> String {
        match self {
            Empty => "???".to_string(),
            Inode(nodes) => {
                let mut ret = "".to_string();
                for node in nodes {
                    ret.push_str(&node.to_string());
                }
                ret
            },
            Source{code, meta: _} => {
                let mut ret = code[0].to_string();
                for i in 1..code.len() {
                    ret.push_str(" ");
                    ret.push_str(&code[i]);
                }
                println!("-{}-", ret);
                ret.push_str("\n");
                ret
            }
            Error{msg, meta} => {
                let mut ret = "! ".to_string();
                ret.push_str(msg);
                ret.push_str(" file: ");
                ret.push_str(&meta.source_file);
                ret.push_str(" line: ");
                ret.push_str(&meta.line.to_string());
                ret.push_str(" raw_line: ");
                ret.push_str(&meta.raw);
                ret.push_str("\n");
                ret
            }
        }
    }
}

pub fn tree() -> String {
    "Hello from the trees".to_string()
}

/* --------------------------------- Testing -------------------------------- */

use AsmNode::*;
use crate::assembly_source::parse_source;

#[test]
fn test_traverse_tree() {
    fn empty_tree(_: &AsmNode) -> Option<AsmNode> {
        Some(Empty)
    }
    let mut tree = Inode(vec![
                         parse_source("a b\nc d", "path"),
                         parse_source(" a\nb\nc", "path"),
                         Empty,]);
    tree.traverse_tree(&empty_tree);
    assert_eq!(tree, Inode(vec![
                           Inode(vec![Empty, Empty]),
                           Inode(vec![Empty, Empty, Empty]),
                           Empty,]));
}

