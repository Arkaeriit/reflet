use AsmNode::*;
use super::align::AlignKind;
use super::align::AlignKind::*;

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
#[derive(Debug, PartialEq, Eq, Clone)]
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

    /// Raw value in bytes, very close from the final result
    Raw(Vec<u8>),

    /// Number of bytes the next node should be aligned to. Will be replaced by
    /// some raw bytes.
    Align{kind: AlignKind, meta: Metadata},

    /// A node related to addresses. It can be either the definition of a new
    /// label or a reference to that label.
    Label{name: String, is_definition: bool, meta: Metadata},

    /// A node with denote a section. It contains an inode with every elements
    /// in that section.
    Section{name: String, content: Vec<AsmNode>},
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

    /// Traverse a tree in search for error. If they any are present, return a
    /// text explaining them. If none are present, return None
    pub fn check_for_errors(&mut self) -> Option<String> {
        let mut ret = "".to_string();
        let mut checking_for_errors = | node: &AsmNode | -> Option<AsmNode> {
            match node {
                Error{msg, meta} => {
                    ret.push_str(&format!("{}\nAt line {} from file {}: `{}`\n--------------\n", msg, meta.line, meta.source_file, meta.raw));
                    None
                },
                _ => None,
            }
        };

        self.traverse_tree(&mut checking_for_errors);
        match ret.len() {
            0 => None,
            _ => Some(ret),
        }
    }

    /// Add an element to the tree if it is an inode, panic otherwise. Useful to
    /// add error messages on a root.
    pub fn add_on_top(&mut self, node: AsmNode) {
        match self {
            Inode(nodes) => {
                nodes.push(node);
            },
            _ => {
                panic!("Assembler's root should be an inode");
            },
        }
    }

    /// Add an error on an inode.
    pub fn error_on_top(&mut self, msg: String) {
        self.add_on_top(Error{msg: msg, meta: Metadata{raw: "!!!".to_string(), source_file: "!!!".to_string(), line: 0}});
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
            },
            Raw(data) => {
                let mut ret = "".to_string();
                for byte in data {
                    ret.push_str(&format!("0x{:02X} ", byte));
                }
                ret.push_str("\n");
                ret
            },
            Align{kind, meta: _} => match kind {
                AlignTo(size) => format!("Align to {} bytes\n", size),
                PadUntil(addr) => format!("Pad until address {}\n", addr),
            },
            Label{name, is_definition, meta: _} => {
                if *is_definition {
                    format!("Definition of label {}\n", name)
                } else {
                    format!("Reference to label {}\n", name)
                }
            },
            Section{name, content: _} => {
                 format!("Section {}\n", name)
            }
        }
    }
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

