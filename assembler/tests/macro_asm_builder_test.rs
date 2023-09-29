/// This test simulates a very simplistic assembler to check all the steps of
/// the assembly process.
///
/// The instructions are `add <number>` with the opcode 0x10 and 1 byte for the
/// number, and `set <number>` with the opcode 0x11 and 1 byte for the number.

fn my_micro_assembler(code: &Vec<String>) -> Result<Vec<u8>, String> {
    if code.len() != 2 {
        return Err("Instruction should have a single argument.".to_string());
    }
    match code[0].as_str() {
        "add" => Ok(vec![0x10, u8::from_str_radix(&code[1], 10).unwrap()]),
        "set" => Ok(vec![0x11, u8::from_str_radix(&code[1], 10).unwrap()]),
        _ => Err("Invalid instruction".to_string()),
    }
}

fn my_implementation_specific_macros(code: &Vec<String>) -> Result<Option<String>, String> {
    match code[0].as_str() {
        "set0" => match code.len() {
            1 => Ok(Some("set 0".to_string())),
            _ => Err("Set 0 should have no arguments".to_string()),
        },
        _ => Ok(None),
    }
}

const MY_RUNTIME_START: &str = "@macro set_10_plus 1
    set 10
    add $1
@end

@rawbytes \"11\"
@align \"4\"
@label start
";

const MY_RUNTIME_END: &str = "@align 3
@label end_code
";

const CODE_1: &str = "
@labref end_code
set0
add 10
set_10_plus 3
@labref start
";

/// Make an assembler with the implementation defined above
fn make_assembler(code: &str) -> macro_asm_builder::Assembler {
    let mut asm = macro_asm_builder::Assembler::from_text(code);
    asm.add_text_before(MY_RUNTIME_START, "runtime");
    asm.add_text_after(MY_RUNTIME_END, "runtime");
    assert_eq!(asm.set_word_size(16), None);
    asm.micro_assembly = &my_micro_assembler;
    asm.implementation_macro = &my_implementation_specific_macros;
    asm
}

#[test]
/// Test a successful compilation.
fn test_success() {
    let mut asm = make_assembler(CODE_1);
    match asm.assemble() {
        Err(txt) => {
            println!("{}", txt);
            panic!("Test failed!");
        },
        Ok(data) => {
            assert_eq!(data, vec![0x11, 0, 0, 0, // runtime_start
                                  0x12, 0x00,    // labref end_code
                                  0x11, 0,       // set0
                                  0x10, 10,      // add 10
                                  0x11, 10,      // set_10_plus x
                                  0x10, 3,       // set_10_plus 3
                                  4, 0,          // labref start
                                  0, 0,          // Padding for the align at 3 bytes (offset = 18)
                                  ]);
        },
    }
}

const CODE_2: &str = "set0 bad_arg
bad_instruction arg
add too many args
";

#[test]
/// Test errors raised by the specific implementation.
fn test_error_implementation() {
    let mut asm = make_assembler(CODE_2);
    match asm.assemble() {
        Err(txt) => {
            assert_eq!(txt, "Set 0 should have no arguments
At line 1 from file ./__asm_init: `set0 bad_arg`
--------------
Invalid instruction
At line 2 from file ./__asm_init: `bad_instruction arg`
--------------
Instruction should have a single argument.
At line 3 from file ./__asm_init: `add too many args`
--------------
"                       );
        },
        Ok(_) => {
            panic!("Assembly should have failed.");
        },
    }
}

const CODE_3: &str = "
@end
@rawbytes 777
@align -1
@constant 100000000
";

#[test]
/// Test error raised by the macro assembler builder.
fn test_error_lib() {
    let mut asm = make_assembler(CODE_3);
    match asm.assemble() {
        Err(txt) => {
            assert_eq!(txt, "Error, @end should only be used to end macro definitions.
At line 2 from file ./__asm_init: `@end`
--------------
Error, unable to read byte 777 in @rawbyte directive.
At line 3 from file ./__asm_init: `@rawbytes 777`
--------------
Error, @align directive should take a single positive numeric argument.
At line 4 from file ./__asm_init: `@align -1`
--------------
Error, unable to read number.
At line 5 from file ./__asm_init: `@constant 100000000`
--------------
"                       );
        },
        Ok(_) => {
            panic!("Assembly should have failed.");
        },
    }
}

