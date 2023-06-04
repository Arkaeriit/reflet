/// A module defining the AsmNode enum, which is used to represent and process
/// the assembly code.
mod tree;

/// A module with function to read text and convert it to an Inode with a Source
/// leaf for each line of source code.
mod assembly_source;

/// A module containing the functions used to import assembly files with the
/// `@import` directive.
mod import;

/// A module used to register macros with the `@define` directive and expand
/// macros called in the source.
mod macros;

/// A module used to register raw values with the directives `@rawbytes` or
/// `@constant`.
mod raw;

/// A module to check and expand `@align` directives.
mod align;

/// A module to register labels and link to them.
mod label;

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

    /// The number of bytes in a constant or address
    wordsize: usize,

    /// The pattern that will be put as padding to replace `@align` directives
    align_pattern: Vec<u8>,

    /// Address of the first instruction of first data in the resulting binary
    start_address: usize,

    /// A function that expand implementation-specifics macros. Takes a vector
    /// of tokens from a line of code and return Ok<None> if the macro expansion
    /// is needed, Ok<txt> to replace the text with txt, or Err<txt> to return
    /// an error message destined to the user.
    pub user_macro: &'a dyn Fn(&Vec<String>) -> Result<Option<String>, String>,

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
        fn default_user_macros(_: &Vec<String>) -> Result<Option<String>, String> {
            Ok(None)
        }
        fn default_micro_assembly(_: &Vec<String>) -> Result<Vec<u8>, String> {
            Err("Micro assembly function should be given by the assembler implementation.".to_string())
        }

        Assembler {
            root: parse_source(text, name),
            macros: HashMap::new(),
            wordsize: 0,
            align_pattern: vec![0],
            start_address: 0,
            user_macro: &default_user_macros,
            micro_assembly: &default_micro_assembly,
        }
    }

    /// Initialize the assembler state to be ready to read the input file
    pub fn from_file(path: &str) -> Self {
        let mut import_directive = "@import ".to_string();
        import_directive.push_str(path);
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
        if s.len() == 0 {
            None
        } else {
            if &s[0..1] == "-" {
                match i128::from_str_radix(s, 10) {
                    Ok(num) => {
                        let positive = num * -1;
                        let true_positive = positive as u128;
                        let not = !true_positive;
                        let cc2_u128 = not + 1;
                        let mut cc2 = 0;
                        let mut mask = 0xFF;
                        let mut leftover = cc2_u128;
                        for _i in 0..self.wordsize {
                            cc2 = cc2 | (cc2_u128 & mask);
                            leftover = (mask >> 1) | leftover;
                            mask = mask << 8;
                        }
                        match !leftover {
                            0 => self.format_number(cc2),
                            _ => None
                        }
                    },
                    Err(_) => None,
                }
            } else {
                if s.len() > 3 {
                    if &s[0..2] == "0x" {
                        match u128::from_str_radix(&s[2..s.len()], 0x10) {
                            Ok(num) => self.format_number(num),
                            Err(_) => None,
                        }
                    } else {
                        match u128::from_str_radix(s, 10) {
                            Ok(num) => self.format_number(num),
                            Err(_) => None,
                        }
                    }
                } else {
                    match u128::from_str_radix(s, 10) {
                        Ok(num) => self.format_number(num),
                        Err(_) => None,
                    }
                }
            }
        }
    }
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
