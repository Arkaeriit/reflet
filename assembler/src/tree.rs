
enum AsmNode {
    Empty,
    Inode(Vec<AsmNode>),
    Source(Vec<String>),
}

pub fn tree() -> String {
    "Hello from the trees".to_string()
}

