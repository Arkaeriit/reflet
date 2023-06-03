mod import;
mod macros;

use crate::tree::*;
use crate::assembly_source::parse_source;
use std::collections::HashMap;
use macros::*;

pub struct Assembler {
    root: AsmNode,
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

