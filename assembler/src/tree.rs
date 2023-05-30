
#[derive(Debug, PartialEq, Eq)]
pub enum AsmNode {
    Empty,
    Inode(Vec<AsmNode>),
    Source(Vec<String>),
}

impl AsmNode {
    /// Run a function that can transform each leaf node of the tree if it
    /// is not empty.
    pub fn traverse_tree(&mut self, f: &dyn Fn(&AsmNode) -> AsmNode) {
        match self {
            Self::Empty => {},
            Self::Inode(nodes) => {
                for node in nodes {
                    node.traverse_tree(f);
                }
            },
            _ => {*self = f(self);},
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
    fn empty_tree(_: &AsmNode) -> AsmNode {
        Empty
    }
    let mut tree = Inode(vec![
                         parse_source("a b\nc d"),
                         parse_source(" a\nb\nc"),
                         Empty,]);
    tree.traverse_tree(&empty_tree);
    assert_eq!(tree, Inode(vec![
                           Inode(vec![Empty, Empty]),
                           Inode(vec![Empty, Empty, Empty]),
                           Empty,]));
}

