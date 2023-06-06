extern crate macro_asm_builder;
use std::process::exit;
use macro_asm_builder::*;

/// This module contains implementation-specific macros such as `@set8`.
mod macros;

/// This module implements the micro-assembler fror the Reflet architecture.
mod micro_assembler;

/// This module reads command line arguments
mod cli_arguments;

pub fn main() {
    let args = match cli_arguments::parse_args() {
        Some(args) => {
            if args.help {
                exit(0);
            }
            args
        },
        None => {
            exit(1);
        }
    };
    let mut asm = make_assembler(&args);
    match asm.assemble() {
        Err(txt) => {
            eprintln!("{}", txt);
            exit(3);
        },
        Ok(data) => {
            println!("{:?}", data);
            exit(0);
        }
    }
}

/// Generate an assembler with correct settings given by the parsed argument
/// list.
fn make_assembler(args: &cli_arguments::Arguments) -> Assembler {
    let mut asm = Assembler::from_file(&args.input);
    if let Some(error) = asm.set_word_size(args.word_size) {
        eprintln!("{}", error);
        exit(2);
    }
    asm.implementation_macro = &macros::macros;
    asm.micro_assembly = &micro_assembler::micro_assembler;
    // Size specific macros
    let align_word = format!("@define @align_word 0\n@align {}\n@end", args.word_size/8);
    let default_sr = match args.word_size {
        8 => 7,
        16 => 5,
        32 => 3,
        _ => 1, // Mediocre support for 64 and 128 bits.
    };
    let set_default_sr = format!("@define @set_default_sr 0\nset {}\n@end", default_sr);
    let set_word_size_byte_def = match args.word_size {
        128 => "set 4\nadd WR\nadd WR\n".to_string(),
        64 => "set 4\n add WR\n".to_string(),
        word_size => format!("set {}", word_size/8),
    };
    let set_word_size_byte = format!("@define @set_wordsize_byte 0\n{}\n@end", set_word_size_byte_def);
    asm.add_text_before(&align_word, "align_word");
    asm.add_text_before(&set_default_sr, "set_default_sr");
    asm.add_text_before(&set_word_size_byte, "set_wordsize_byte");
    // Default macros
    asm.add_text_before(include_str!("default_macros.asm"), "default_macros");
    // Start runtime
    let mut runtime_start = "".to_string();
    if !args.no_prefix {
        runtime_start.push_str("@rawbytes 41 53 52 4d\n");
    }
    runtime_start.push_str("@set_default_sr\ncpy SR\n");
    if args.set_stack {
        if args.stack_addr.as_str() == "" {
            runtime_start.push_str("setlab __code__end__\n");
        } else {
            runtime_start.push_str(&format!("set+ {}\n", args.stack_addr));
        }
        runtime_start.push_str("cpy SP\n");
    }
    if !args.ignore_start {
        runtime_start.push_str("goto start");

    }
    asm.add_text_before(&runtime_start, "runtime_start");
    // End runtime
    asm.add_text_after("@align_word\n@label __code__end__\n", "runtime_end");

    asm
}
