use macro_asm_builder::utils::format_string_into_number;

/// Runs all the implementation-specifics macros
pub fn macros(code: &Vec<String>) -> Result<Option<String>, String> {
    match get_macro_to_use(&code[0]) {
        Some(f) => match f(code) {
            Ok(x) => Ok(Some(x)),
            Err(x) => Err(x),
        },
        None => Ok(None)
    }
}

/// Gets the function needed
fn get_macro_to_use(code0: &str) -> Option<&dyn Fn(&Vec<String>) -> Result<String, String>> {
    match code0 {
        "@set8" => Some(&set8),
        "@set_sr_for" | "@get_sr_for" => Some(&get_or_set_sr_for),
        _ => None,
    }
}

/* ---------------------------------- Set 8 --------------------------------- */

/// Expands the set8 macro
fn set8(code: &Vec<String>) -> Result<String, String> {
    match code[0].as_str() {
        "@set8" => {
            if code.len() == 2 {
                match format_string_into_number(&code[1]) {
                    Some((num, _)) => if num <= 0xFF {
                        Ok(format_set8(num.try_into().unwrap()))
                    } else {
                        Err(format!("Error, macro @set8 should take a single 1-byte number as argument."))
                    },
                    None => Err(format!("Error, macro @set8 should take a single 1-byte number as argument.")),
                }
            } else {
                Err(format!("Error, macro @set8 should take a single 1-byte number as argument."))
            }
        },
        _ => Err(format!("Error in the assembler, set8 should not parse {:?}!", code)),
    }
}

/// Generates the code used for a set8 macro
fn format_set8(num: u8) -> String {
    format!("set 4
    cpy R12
    set {}
    lsl R12
    cpy R12
    set {}
    or R12", (num & 0xF0) >> 4, num & 0x0F)
}

/* ----------------------- @set_sr_for or @get_sr_for ----------------------- */

/// Comput the ceil of the log2 of a number
fn clog2(n: u128) -> Option<usize> {
    let bit_used: u128 = (u128::BITS - n.leading_zeros()).into();
    if bit_used > 0 {
        Some(<u128 as TryInto<usize>>::try_into(bit_used - 1).unwrap())
    } else {
        None
    }
}

/// Expands the @set_sr_for or @get_sr_for macro
fn get_or_set_sr_for(code: &Vec<String>) -> Result<String, String> {
    match code[0].as_str() {
        "@set_sr_for" | "@get_sr_for" => {
            let format_function = if code[0].as_str() == "@set_sr_for" {
                format_set_sr_for
            } else {
                format_get_sr_for
            };
            if code.len() == 2 {
                match format_string_into_number(&code[1]) {
                    Some((num, false)) => {
                        let sr_value = if let Some(log) = clog2(num / 8) {
                            log + 1
                        } else {
                            return Err(format!("Error, the directive {} can't have {} as input. Valid values are 8, 16, 32, 64, and 128.", code[0], num));
                        };
                        if sr_value < 16 {
                            Ok(format_function(sr_value))
                        } else {
                            Err(format!("Error, the assembler does not support {} with a value of {}.", code[0], num))
                        }
                    },
                    None | Some((_, true)) => Err(format!("Error, macro {} should take a single number as argument.", code[0])),
                }
            } else {
                Err(format!("Error, macro @set_sr_for should take a single 1-byte number as argument."))
            }
        },
        _ => Err(format!("Error in the assembler, get_or_set_sr_for should not parse {:?}!", code)),
    }
}

/// Generates the code used for a @set_sr_for macro
fn format_set_sr_for(sr_value: usize) -> String {
    format!("
    {}
    cpy SR", format_get_sr_for(sr_value))
}

/// Generates the code used for a @get_sr_for macro
fn format_get_sr_for(sr_value: usize) -> String {
    format!("
    ; Use 8 as it is the offset to the reduced behavior bits
    set 8
    cpy R12
    ; The the desires reduced behaviors bits
    set {}
    lsl R12
    xor SR
    ; Now WR contains the 8 LSB of SR and its MSB are the XOR between the desired SR and teh current SR
    lsr R12
    lsl R12
    ; The 8 LSB of the WR have been cleared, doing a xor with SR will get the 8 LSB of SR and the new and updated MSB
    xor SR", sr_value)
}


/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_clog2() {
    assert_eq!(clog2(1), Some(0));
    assert_eq!(clog2(2), Some(1));
    assert_eq!(clog2(3), Some(1));
    assert_eq!(clog2(4), Some(2));
    assert_eq!(clog2(5), Some(2));
    assert_eq!(clog2(6), Some(2));
    assert_eq!(clog2(7), Some(2));
    assert_eq!(clog2(8), Some(3));
    assert_eq!(clog2(9), Some(3));
}

#[test]
fn test_clog2_fail() {
    assert_eq!(clog2(0), None);
}

#[test]
fn test_set_sr_for() {
    fn direct_sr_for(n: usize) -> Result<String, String> {
        let mut ret = Vec::<String>::new();
        for s in format!("@set_sr_for {}", n).split(" ") {
            ret.push(s.to_string());
        }
        get_or_set_sr_for(&ret)
    }
    assert_eq!(direct_sr_for(8), Ok(format_set_sr_for(1)));
    assert_eq!(direct_sr_for(16), Ok(format_set_sr_for(2)));
    assert_eq!(direct_sr_for(32), Ok(format_set_sr_for(3)));
    assert_eq!(direct_sr_for(64), Ok(format_set_sr_for(4)));
}

