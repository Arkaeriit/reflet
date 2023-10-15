
/// Enum used to represent what kind of arguments an instruction can take
#[derive(Debug, PartialEq)]
enum InstructionArgumentType {
    /// No arguments
    NoArg,

    /// The name of a register
    Register,

    /// A number of a given size in bits
    Number(usize),
}
use InstructionArgumentType::*;

/// Represent one kind of valid instruction
#[derive(Debug, PartialEq)]
struct Instruction {
    mnemonic: &'static str,
    argument: InstructionArgumentType,
    opcode: u8,
}


/// List of all instructions. THE MNEMONOC MUST BE LOWER CASE!
static INSTRUCTION_LIST: &'static [Instruction] = &[
    // Instructions with 4 bits arguments
    Instruction{mnemonic: "read",        argument: Register,   opcode: 0x0},
    Instruction{mnemonic: "cpy",         argument: Register,   opcode: 0x1},
    Instruction{mnemonic: "set",         argument: Number(4),  opcode: 0x2},
    Instruction{mnemonic: "add",         argument: Register,   opcode: 0x3},
    Instruction{mnemonic: "and",         argument: Register,   opcode: 0x4},
    Instruction{mnemonic: "or",          argument: Register,   opcode: 0x5},
    Instruction{mnemonic: "xor",         argument: Register,   opcode: 0x6},
    Instruction{mnemonic: "not",         argument: Register,   opcode: 0x7},
    Instruction{mnemonic: "lsl",         argument: Register,   opcode: 0x8},
    Instruction{mnemonic: "lsr",         argument: Register,   opcode: 0x9},
    Instruction{mnemonic: "eq",          argument: Register,   opcode: 0xA},
    Instruction{mnemonic: "les",         argument: Register,   opcode: 0xB},
    Instruction{mnemonic: "str",         argument: Register,   opcode: 0xC},
    Instruction{mnemonic: "load",        argument: Register,   opcode: 0xD},
    // Instructions without arguments
    Instruction{mnemonic: "jif",         argument: NoArg,      opcode: 0xE0},
    Instruction{mnemonic: "call",        argument: NoArg,      opcode: 0xE1},
    Instruction{mnemonic: "ret",         argument: NoArg,      opcode: 0xE2},
    Instruction{mnemonic: "pop",         argument: NoArg,      opcode: 0xE3},
    Instruction{mnemonic: "push",        argument: NoArg,      opcode: 0xE4},
    Instruction{mnemonic: "cc2",         argument: NoArg,      opcode: 0xE5},
    Instruction{mnemonic: "cmpnot",      argument: NoArg,      opcode: 0xE6},
    Instruction{mnemonic: "tbm",         argument: NoArg,      opcode: 0xE7},
    Instruction{mnemonic: "quit",        argument: NoArg,      opcode: 0xE8},
    Instruction{mnemonic: "debug",       argument: NoArg,      opcode: 0xE9},
    Instruction{mnemonic: "atom",        argument: NoArg,      opcode: 0xEA},
    Instruction{mnemonic: "retint",      argument: NoArg,      opcode: 0xEB},
    // Instructions with two bit arguments
    Instruction{mnemonic: "setint",      argument: Number(2),  opcode: 0xEC >> 2},
    Instruction{mnemonic: "getint",      argument: Number(2),  opcode: 0xF0 >> 2},
    Instruction{mnemonic: "getintstack", argument: Number(2),  opcode: 0xF4 >> 2},
    Instruction{mnemonic: "setintstack", argument: Number(2),  opcode: 0xF8 >> 2},
    Instruction{mnemonic: "softint",     argument: Number(2),  opcode: 0xFC >> 2},
];

/// Try to read the number or give error message in case of failure.
fn format_byte_from_string(s: &str, size_in_bits: usize) -> Result<u8, String> {
    let try_converting = if s.len() >= 3 && &s[0..2] == "0x" {
        match u8::from_str_radix(&s[2..s.len()], 16) {
            Ok(num) => Ok(num),
            Err(_) => Err(format!("Error, unable to read hexadecimal number {}.", s)),
        }
    } else {
        match u8::from_str_radix(s, 10) {
            Ok(num) => Ok(num),
            Err(_) => match u8::from_str_radix(s, 16) {
                Ok(num) => Ok(num),
                Err(_) => Err(format!("Error, unable to read number {}.", s)),
            }
        }
    };

    match try_converting {
        Ok(num) => {
            if num >> size_in_bits == 0 {
                Ok(num)
            } else {
                Err(format!("Error, number {} cannot fit in {} bits.", s, size_in_bits))
            }
        },
        Err(msg) => Err(msg),
    }
}

/// Return the instruction for a given mnemonic
fn get_instruction(mnemonic: &str) -> Option<&Instruction> {
    let mnemonic_lower = mnemonic.to_ascii_lowercase();
    for instruction in INSTRUCTION_LIST {
        if instruction.mnemonic == mnemonic_lower {
            return Some(instruction);
        }
    }
    None
}

/// Return the numeric representation of a register is the register is valid
fn get_register(register: &str) -> Option<u8> {
    let reg = register.to_ascii_lowercase();
    match reg.as_str() {
        "wr" => Some(0),
        "sr" => Some(13),
        "pc" => Some(14),
        "sp" => Some(15),
        _ => {
            if &reg[0..1] == "r" {
                match format_byte_from_string(&reg[1..reg.len()], 4) {
                    Ok(num) => Some(num),
                    Err(_) => None,
                }
            } else {
                None
            }
        }
    }
}
 
/// Format an instruction with a numeric argument. For instructions that don't
/// take numeric arguments, it is ignored.
fn format_instruction(instruction: &Instruction, num: u8) -> u8 {
    match instruction.argument {
        NoArg => instruction.opcode,
        Register => (instruction.opcode << 4) | num,
        Number(size) => (instruction.opcode << size) | num,
    }
}

/// Compile a line of Reflet assembly
pub fn micro_assembler(code: &Vec<String>) -> Result<Vec<u8>, String> {
    match get_instruction(&code[0]) {
        Some(instruction) => match instruction.argument {
            NoArg => if code.len() == 1 {
                Ok(vec![format_instruction(&instruction, 0)])
            } else {
                Err(format!("Error, instruction {} takes no arguments.", &code[0]))
            },
            Register => if code.len() == 2 {
                match get_register(&code[1]) {
                    Some(num) => Ok(vec![format_instruction(&instruction, num)]),
                    None => Err(format!("Error, instruction {} should take a valid register as argument.", &code[0])),
                }
            } else {
                Err(format!("Error, instruction {} takes one single arguments.", &code[0]))
            },
            Number(size) => if code.len() == 2 {
                match format_byte_from_string(&code[1], size) {
                    Ok(num) => Ok(vec![format_instruction(&instruction, num)]),
                    Err(msg) => Err(msg),
                }
            } else {
                Err(format!("Error, instruction {} takes one single arguments.", &code[0]))
            },
        }
        None => Err(format!("Error, {} is not a valid instruction.", &code[0])),
    }
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_format_byte_from_string() {
    assert_eq!(format_byte_from_string("A", 4),   Ok(10));
    assert_eq!(format_byte_from_string("0xb", 4), Ok(11));
    assert_eq!(format_byte_from_string("11", 4),  Ok(11));
    assert_eq!(format_byte_from_string("7", 3),   Ok(7));

    assert_eq!(format_byte_from_string("7", 2),     Err("Error, number 7 cannot fit in 2 bits.".to_string()));
    assert_eq!(format_byte_from_string("0xlol", 2), Err("Error, unable to read hexadecimal number 0xlol.".to_string()));
    assert_eq!(format_byte_from_string("lol", 2),   Err("Error, unable to read number lol.".to_string()));
}

#[test]
fn test_get_instruction() {
    assert_eq!(get_instruction("set"), Some(&INSTRUCTION_LIST[2]));
    assert_eq!(get_instruction("SET"), Some(&INSTRUCTION_LIST[2]));
    assert_eq!(get_instruction("sEt"), Some(&INSTRUCTION_LIST[2]));

    assert_eq!(get_instruction("Not-a-valid-instruction"), None);
}

#[test]
fn test_get_register() {
    assert_eq!(get_register("r0xA"), Some(0xA));
    assert_eq!(get_register("RA"),   Some(0xA));
    assert_eq!(get_register("SP"),   Some(0xF));
    assert_eq!(get_register("r1"),   Some(1));

    assert_eq!(get_register("1"),   None);
    assert_eq!(get_register("lol"), None);
    assert_eq!(get_register("ssp"), None);
}

#[test]
fn test_micro_assembler() {
    assert_eq!(micro_assembler(&vec!["set".to_string(), "8".to_string()]),  Ok(vec![0x28]));
    assert_eq!(micro_assembler(&vec!["and".to_string(), "R2".to_string()]), Ok(vec![0x42]));
    assert_eq!(micro_assembler(&vec!["EQ".to_string(), "PC".to_string()]),  Ok(vec![0xAE]));
    assert_eq!(micro_assembler(&vec!["Tbm".to_string()]),                   Ok(vec![0xE7]));
    assert_eq!(micro_assembler(&vec!["cc2".to_string()]),                   Ok(vec![0xE5]));

    assert_eq!(micro_assembler(&vec!["lolnope".to_string()]), Err("Error, lolnope is not a valid instruction.".to_string()));
    assert_eq!(micro_assembler(&vec!["set".to_string()]), Err("Error, instruction set takes one single arguments.".to_string()));
    assert_eq!(micro_assembler(&vec!["setint".to_string(), "5".to_string()]), Err("Error, number 5 cannot fit in 2 bits.".to_string()));
}

