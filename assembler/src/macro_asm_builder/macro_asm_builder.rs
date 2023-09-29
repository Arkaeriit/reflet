/// A module defining the AsmNode enum, which is used to represent and process
/// the assembly code.
mod tree;

/// A module with function to read text and convert it to an Inode with a Source
/// leaf for each line of source code.
mod assembly_source;

/// A module containing the functions used to import assembly files with the
/// `@import` directive.
mod import;

/// A module used to register macros with the `@macro` directive and expand
/// macros called in the source.
mod macros;

/// A module used to register raw values with the directives `@rawbytes` or
/// `@constant`.
mod raw;

/// A module to check and expand `@align` directives.
mod align;

/// A module to register labels and link to them.
mod label;

/// A module to register string and transform them in raw bytes.
mod strings;

/// A module to register sections and put code in them.
mod section;

/// A module to register flat defines and math expressions.
mod define;

/// A mod with miscellaneous useful functions.
pub mod utils;

use crate::tree::*;
use crate::tree::AsmNode::*;
use crate::assembly_source::parse_source;
use std::collections::HashMap;
use macros::*;

/// The collection of the assembly code tree and all the mutable context needed
/// to process it
pub struct Assembler<'a> {
    /// The root element of the tree used to represent and process the assembly
    /// code
    root: AsmNode,

    /// A map of all the macros linking their names to their contents
    macros: HashMap<String, Macro>,

    /// A map of all flat defines linking their names to their contents
    defines: HashMap<String, Vec<String>>,

    /// The number of bytes in a constant or address
    wordsize: usize,

    /// The pattern that will be put as padding to replace `@align` directives
    align_pattern: Vec<u8>,

    /// Address of the first instruction of first data in the resulting binary
    start_address: usize,

    /// A function that expand implementation-specifics macros. Takes a vector
    /// of tokens from a line of code and return Ok<None> if no macro expansion
    /// is needed, Ok<txt> to replace the text with txt, or Err<txt> to return
    /// an error message destined to the user.
    pub implementation_macro: &'a dyn Fn(&Vec<String>) -> Result<Option<String>, String>,

    /// A function used to compile assembly source code into machine code. It
    /// is implementation-specific. It takes a vector if tokens from a line of
    /// code and return Ok<bytes> to return the machine code or Err<txt> to
    /// return an error message destined to the user. When this function is run,
    /// all code left in the tree should be raw assembly and not macros or
    /// directive. Thus, there is no way this function can ignore code.
    pub micro_assembly: &'a dyn Fn(&Vec<String>) -> Result<Vec<u8>, String>,
}

impl Assembler<'_> {
    /// Takes some assembly code as input and set up the assembler state with it
    pub fn from_text(text: &str) -> Self {
        Self::from_named_text(text, "./__asm_init")
    }

    /// As from_text but the name is chosen
    fn from_named_text(text: &str, name: &str) -> Self {
        fn default_implementation_macro(_: &Vec<String>) -> Result<Option<String>, String> {
            Ok(None)
        }
        fn default_micro_assembly(_: &Vec<String>) -> Result<Vec<u8>, String> {
            Err("Micro assembly function should be given by the assembler implementation.".to_string())
        }

        Assembler {
            root: parse_source(text, name),
            macros: HashMap::new(),
            defines: HashMap::new(),
            wordsize: 0,
            align_pattern: vec![0],
            start_address: 0,
            implementation_macro: &default_implementation_macro,
            micro_assembly: &default_micro_assembly,
        }
    }

    /// Initialize the assembler state to be ready to read the input file
    pub fn from_file(path: &str) -> Self {
        let mut import_directive = "@import \"".to_string();
        import_directive.push_str(path);
        import_directive.push_str("\"");
        Self::from_text(&import_directive)
    }

    /// Set the wordsize of the assembler given it in bits. Return None if the
    /// value is valid and an error message otherwise.
    pub fn set_word_size(&mut self, wordsize: usize) -> Option<&str> {
        match wordsize {
            8 | 16 | 32 | 64 | 128 => {
                self.wordsize = wordsize / 8;
                None
            },
            x => {
                if x > 128 {
                    Some("Only word sizes of 128 bits or less are supported. This is not a limitation of the Reflet architecture but one of this assembler.")
                } else {
                    Some("Valid word sizes should be 8 bits times a power of two such as 8, 16, or 32.")
                }
            },
        }
    }

    /// Add some text at the beginning of the tree.
    pub fn add_text_before(&mut self, txt: &str, name: &str) {
        match &mut self.root {
            Inode(list) => {
                list.insert(0, parse_source(txt, name));
            },
            _ => {
                panic!("Assembler's root should have been an inode!");
            },
        }
    }

    /// Add some text at the end of the tree.
    pub fn add_text_after(&mut self, txt: &str, name: &str) {
        match &mut self.root {
            Inode(list) => {
                list.push(parse_source(txt, name));
            },
            _ => {
                panic!("Assembler's root should have been an inode!");
            },
        }
    }

    /// Runs all passes of the assembly process that might add new text in the
    /// tree.
    fn run_text_adding_passes(&mut self) {
        // All the operations that might add new text on the tree. If an
        // operation added new text, we want to run them all from the start to
        // ensure that we are not missing anything. It should end with the most
        // costly operations as we don't want to rerun them too much time.
        const TEXT_ADDING_PASSES: [&dyn Fn(&mut Assembler) -> bool; 5] = [
            &import::include_source,
            &macros::register_macros,
            &run_implementation_macros,
            &macros::expand_macros,
            &define::handle_define,
        ];

        let mut pass_index = 0;
        while pass_index < TEXT_ADDING_PASSES.len() {
            if TEXT_ADDING_PASSES[pass_index](self) {
                pass_index = 0;
            } else {
                pass_index += 1;
            }
        }
    }

    /// Perform a complete assembly process. Return a vector of bytes of the
    /// resulting binary in case of success and an error message in case of
    /// error.
    pub fn assemble(&mut self) -> Result<Vec<u8>, String> {
        // Manage macros and directives
        self.run_text_adding_passes();
        section::handle_sections(self);
        label::register_labels(self);
        align::register_align(self);
        raw::expand_constants(self);
        raw::decode_raw_bytes(self);
        strings::register_strings(self);

        // Run the micro-assembler
        self.run_micro_assembler();

        // Finish the linking and padding
        align::expand_align(self);
        label::expand_labels(self);
        let ret = self.collect_raw();

        // Return raw or report errors
        match self.root.check_for_errors() {
            Some(txt) => Err(txt),
            None => Ok(ret),
        }
    }

    /// Executes the implementation-specific micro-assembler.
    fn run_micro_assembler(&mut self) {
        let mut running_micro_assembler = | node: &AsmNode | -> Option<AsmNode> {
            match node {
                Source{code, meta} => match (self.micro_assembly)(code) {
                    Err(msg) => Some(Error{msg, meta: meta.clone()}),
                    Ok(raw) => Some(Raw(raw)),
                },
                _ => None,
            }
        };

        self.root.traverse_tree(&mut running_micro_assembler);
    }

    /// Return a label dump of the tree
    pub fn label_dump(&mut self) -> String {
        label::label_dump(self)
    }

    /// Gather all the raw part of a tree and extract them. Make an error for
    /// non-raw parts.
    fn collect_raw(&mut self) -> Vec<u8> {
        let mut ret: Vec<u8> = vec![];
        let mut collecting_raw = | node: &AsmNode | -> Option<AsmNode> {
            match node {
                Raw(data) => {
                    ret.extend(data);
                    None
                },
                Error{msg: _, meta: _} => None,
                Label{name: _, is_definition: true, meta: _} => None,
                x => Some(Error{msg: format!("There is a bug in the assembler, the node {} should not be left over in collect_raw.", &x.to_string()), meta: Metadata{line: !0, raw: "!!!".to_string(), source_file: "!!!".to_string()}}),
            }
        };

        self.root.traverse_tree(&mut collecting_raw);
        ret
    }

    /// From a number, convert it into a stream of bytes of the size required by
    /// the processor's wordsize. Return None if the number is too big to be
    /// represented.
    fn format_number(&self, number: u128) -> Option<Vec<u8>> {
        let mut n = number;
        let mut ret = vec![];
        for _i in 0..self.wordsize {
            ret.push((n & 0xFF) as u8);
            n = n >> 8
        }
        if n == 0 {
            Some(ret)
        } else {
            None
        }
    }

    /// Tries to convert the string representation of a number into a stream of
    /// bytes.
    fn format_string_into_number(&self, s: &str) -> Option<Vec<u8>> {
        match utils::format_string_into_number(s) {
            Some((num, false)) => self.format_number(num),
            Some((num, true)) => {
                let mut num_shorten = num;
                for i in self.wordsize..(128/8) {
                    // Ensure that there is no data bits above the wordsize
                    let mask: u128 = (0xFF << (i * 8)) >> 1;
                    if ((num & mask) << 1) >> (i * 8) != 0xFF {
                        return None
                    }
                    // Remove sign extenton bits
                    num_shorten = num_shorten & !(0xFF << (i*8));
                }
                self.format_number(num_shorten)
            },
            None => None,
        }
    }
}

/// Executes the implementation-specific macros. Return true if some macros
/// have been expanded.
fn run_implementation_macros(asm: &mut Assembler) -> bool {
    let mut expanded_macros = false;
    let mut running_implementation_macros = | node: &AsmNode | -> Option<AsmNode> {
        match node {
            Source{code, meta} => match (asm.implementation_macro)(code) {
                Err(msg) => Some(Error{msg, meta: meta.clone()}),
                Ok(None) => None,
                Ok(Some(txt)) => {
                    expanded_macros = true;
                    Some(parse_source(&txt, &format!("Expantion of line {} from file {}, being {}", meta.line, &meta.source_file, &meta.raw)))
                },
            },
            _ => None,
        }
    };

    asm.root.traverse_tree(&mut running_implementation_macros);
    expanded_macros
}



/* --------------------------------- Testing -------------------------------- */
#[test]
fn test_set_word_size() {
    let mut asm = Assembler::from_text("");
    assert_eq!(asm.set_word_size(8), None);
    assert_eq!(asm.set_word_size(128), None);
    assert_ne!(asm.set_word_size(256), None);
    assert_ne!(asm.set_word_size(63), None);
    assert_ne!(asm.set_word_size(0), None);
}

#[test]
fn test_format_number() {
    let mut asm = Assembler::from_text("");
    assert_eq!(asm.set_word_size(8), None);
    assert_eq!(asm.format_number(12), Some(vec![12]));
    assert_eq!(asm.format_number(250), Some(vec![250]));
    assert_eq!(asm.format_number(260), None);
    assert_eq!(asm.set_word_size(16), None);
    assert_eq!(asm.format_number(12), Some(vec![12, 0]));
    assert_eq!(asm.format_number(250), Some(vec![250, 0]));
    assert_eq!(asm.format_number(260), Some(vec![4, 1]));
    assert_eq!(asm.format_number(0x10000), None);
}

#[test]
fn format_string_into_number() {
    let mut asm = Assembler::from_text("");
    assert_eq!(asm.set_word_size(8), None);
    assert_eq!(asm.format_string_into_number("12"), Some(vec![12]));
    assert_eq!(asm.format_string_into_number("250"), Some(vec![250]));
    assert_eq!(asm.format_string_into_number("260"), None);
    assert_eq!(asm.format_string_into_number("-129"), None);
    assert_eq!(asm.format_string_into_number("-10"), Some(vec![0xF6]));
    assert_eq!(asm.set_word_size(16), None);
    assert_eq!(asm.format_string_into_number("12"), Some(vec![12, 0]));
    assert_eq!(asm.format_string_into_number("250"), Some(vec![250, 0]));
    assert_eq!(asm.format_string_into_number("260"), Some(vec![4, 1]));
    assert_eq!(asm.format_string_into_number("0x10000"), None);
    assert_eq!(asm.format_string_into_number("Potate"), None);
    assert_eq!(asm.format_string_into_number("0xFFF"), Some(vec![0xFF, 0xF]));
    assert_eq!(asm.format_string_into_number("-10"), Some(vec![0xF6, 0xFF]));
}

