use AsmNode::*;

/// Metadata used to store information regarding the source line of a node.
#[derive(Debug, PartialEq, Eq, Clone)]
pub struct Metadata {
    /// The original and unprocessed line of assembly code.
    pub raw: String,

    /// The path of the file that line comes from.
    pub source_file: String,

    /// The number of the line.
    pub line: usize,
}

/// The nodes for the tree representing the assembly code.
#[derive(Debug, PartialEq, Eq)]
pub enum AsmNode {
    /// Some steps of the assembly consume nodes to update the assembler's state
    /// such as when registering macros. In those case, a empty node is left.
    /// This is made for performance reason in order not to resize inodes.
    Empty,

    /// While all actual content is stored in leaf nodes, the structure of tree
    /// is made with inodes that can have an arbitrary number of children.
    Inode(Vec<AsmNode>),

    /// A line of assembly language code. Each token in the line is an element
    /// of the vector.
    Source{code: Vec<String>, meta: Metadata},

    /// When a step of the compilation causes an error, the node responsible for
    /// the error is replaced with an error node that contain a message that
    /// should be displayed to the user.
    Error{msg: String, meta: Metadata},
}

impl AsmNode {
    /// Run a function that can transform each leaf node of the tree if it
    /// is not empty. The function should return `None` if the leaf should stay
    /// untouched and `Some(newNode)` to change the node.
    pub fn traverse_tree(&mut self, f: &mut dyn FnMut(&AsmNode) -> Option<AsmNode>) {
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
            Empty => "".to_string(),
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

#[test]
fn test_traverse_tree() {
    use crate::assembly_source::parse_source;

    fn empty_tree(_: &AsmNode) -> Option<AsmNode> {
        Some(Empty)
    }
    let mut tree = Inode(vec![
                         parse_source("a b\nc d", "path"),
                         parse_source(" a\nb\nc", "path"),
                         Empty,]);
    tree.traverse_tree(&mut empty_tree);
    assert_eq!(tree, Inode(vec![
                           Inode(vec![Empty, Empty]),
                           Inode(vec![Empty, Empty, Empty]),
                           Empty,]));
}

