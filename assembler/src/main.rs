/// A module defining the AsmNode enum, which is used to represent and process
/// the assembly code.
mod tree;

/// A module with function to read text and convert it to an Inode with a Source
/// leaf for each line of source code.
mod assembly_source;

/// A module with the Assembler state and the functions used to process the
/// source code tree
mod assembler;

pub fn main() {
    println!("{}", tree::tree());
}

