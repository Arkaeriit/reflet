use std::process::exit;
use macro_asm_builder::*;
use std::io::Write;
use std::fs::File;

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
    let assembled_bin = match asm.assemble() {
        Err(txt) => {
            eprintln!("{}", txt);
            exit(3);
        },
        Ok(data) => {
            data
        }
    };

    if args.label_dump {
        println!("{}", asm.label_dump());
    }

    let mut output_file = match File::create(&args.output) {
        Ok(file) => file,
        Err(_) => {
            eprintln!("Error, unable to open output file {}", &args.output);
            exit(4);
        },
    };
    if let Err(_) = output_file.write(&assembled_bin) {
        eprintln!("Error, unable to write in file {}", &args.output);
        exit(5);
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
    asm.start_address = args.start_addr;
    // Size specific macros
    let align_word = format!("@macro @align_word 0\n@align {}\n@end", args.word_size/8);
    let set_default_sr = format!("@macro @set_default_sr 0\n@set_sr_for {}\n@end", args.word_size);
    let set_word_size_byte_def = match args.word_size {
        128 => "set 4\nadd WR\nadd WR\n".to_string(),
        64 => "set 4\n add WR\n".to_string(),
        word_size => format!("set {}", word_size/8),
    };
    let set_word_size_byte = format!("@macro @set_wordsize_byte 0\n{}\n@end", set_word_size_byte_def);
    asm.add_text_before(&align_word, "align_word");
    asm.add_text_before(&set_default_sr, "set_default_sr");
    asm.add_text_before(&set_word_size_byte, "set_wordsize_byte");
    // Default macros
    asm.add_text_before(include_str!("default_macros.asm"), "default_macros");
    // Argument-given defines
    for (name, value) in &args.defines {
        asm.add_text_before(&format!("@define {name} {value}"), &format!("{name} definition"));
    }
    // Start runtime
    let mut runtime_start = "".to_string();
    if !args.no_prefix {
        runtime_start.push_str("@string \"ASRM\"\n");
    }
    if !args.no_compatibility {
        runtime_start.push_str("@set_default_sr\n");
    }
    if args.set_stack {
        if args.stack_addr.as_str() == "" {
            runtime_start.push_str("setlab __code__end__\n");
        } else {
            runtime_start.push_str(&format!("set+ {}\n", args.stack_addr));
        }
        runtime_start.push_str("cpy SP\n");
    }
    if !args.ignore_start {
        runtime_start.push_str("goto start\n");

    }
    runtime_start.push_str("@label __code__start__\n");
    asm.add_text_before(&runtime_start, "runtime_start");
    // End runtime
    asm.add_text_after("@align_word\n@label __code__end__\n", "runtime_end");

    asm
}
