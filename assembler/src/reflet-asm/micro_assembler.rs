
/// Enum used to represent what kind of arguments an instruction can take
enum InstructionArgumentType {
    /// No arguments
    None,

    /// The name of a register
    Register,

    /// A number of a given size in bits
    Number(usize),
}
use InstructionArgumentType::*;

/// Represent one kind of valid instruction
struct Instruction<'a> {
    mnemonic: &'a str,
    argument: InstructionArgumentType,
    opcode: u8,
}

static INSTRUCTION_LIST: &'static [Instruction] = &[
    Instruction{mnemonic: "set", argument: Register, opcode: 0x1},
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

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_format_byte_from_string() {
    assert_eq!(format_byte_from_string("A", 4), Ok(10));
    assert_eq!(format_byte_from_string("0xb", 4), Ok(11));
    assert_eq!(format_byte_from_string("11", 4), Ok(11));
    assert_eq!(format_byte_from_string("7", 3), Ok(7));

    assert_eq!(format_byte_from_string("7", 2), Err("Error, number 7 cannot fit in 2 bits.".to_string()));
    assert_eq!(format_byte_from_string("0xlol", 2), Err("Error, unable to read hexadecimal number 0xlol.".to_string()));
    assert_eq!(format_byte_from_string("lol", 2), Err("Error, unable to read number lol.".to_string()));
}

