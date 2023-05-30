mod import;

use crate::tree::*;
use crate::assembly_source::parse_source;

struct Assembler {
    root: AsmNode,
}

impl Assembler {
    /// Takes some assembly code as input and set up the assembler state with it
    pub fn from_text(text: &str) -> Self {
        Assembler {
            root: parse_source(text, "./__asm_init"),
        }
    }

    /// Initialize the assembler state to be ready to read the input file
    pub fn from_file(path: &str) -> Self {
        let mut import_directive = "@import ".to_string();
        import_directive.push_str(path);
        Self::from_text(&import_directive)
    }
}

