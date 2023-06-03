/// A module containing the functions used to import assembly files with the
/// `@import` directive.
mod import;

/// A module used to register macros with the `@define` directive and expand
/// macros called in the source.
mod macros;

use crate::tree::*;
use crate::assembly_source::parse_source;
use std::collections::HashMap;
use macros::*;

/// The collection of the assembly code tree and all the mutable context needed
/// to process it
pub struct Assembler {
    /// The root element of the tree used to represent and process the assembly
    /// code
    root: AsmNode,
    /// A map of all the macros linking their names to their contents
    macros: HashMap<String, Macro>,
}

impl Assembler {
    /// Takes some assembly code as input and set up the assembler state with it
    pub fn from_text(text: &str) -> Self {
        Self::from_named_text(text, "./__asm_init")
    }

    /// As from_text but the name is choosen
    pub fn from_named_text(text: &str, name: &str) -> Self {
        Assembler {
            root: parse_source(text, name),
            macros: HashMap::new(),
        }
    }

    /// Initialize the assembler state to be ready to read the input file
    pub fn from_file(path: &str) -> Self {
        let mut import_directive = "@import ".to_string();
        import_directive.push_str(path);
        Self::from_text(&import_directive)
    }
}

