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
        "@set_sr_for" => Some(&set_sr_for),
        _ => None,
    }
}

/* ---------------------------------- Set 8 --------------------------------- */

/// Expands the set8 macro
fn set8(code: &Vec<String>) -> Result<String, String> {
    match code[0].as_str() {
        "@set8" => {
            if code.len() == 2 {
                match format_string_into_byte(&code[1]) {
                    Some(num) => Ok(format_set8(num)),
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

/// Decode a byte. Return none if it is invalid
fn format_string_into_byte(s: &str) -> Option<u8> {
    if s.len() == 0 {
        None
    } else {
        if &s[0..1] == "-" {
            match i8::from_str_radix(s, 10) {
                Ok(num) => {
                    let positive = num * -1;
                    let true_positive = positive as u8;
                    let not = !true_positive;
                    let cc2 = not + 1;
                    Some(cc2)
                },
                Err(_) => None,
            }
        } else {
            if s.len() > 3 {
                if &s[0..2] == "0x" {
                    match u8::from_str_radix(&s[2..s.len()], 0x10) {
                        Ok(num) => Some(num),
                        Err(_) => None,
                    }
                } else {
                    match u8::from_str_radix(s, 10) {
                        Ok(num) => Some(num),
                        Err(_) => None,
                    }
                }
            } else {
                match u8::from_str_radix(s, 10) {
                    Ok(num) => Some(num),
                    Err(_) => None,
                }
            }
        }
    }
}

/* ------------------------------- @set_sr_for ------------------------------ */

/// Comput the ceil of the log2 of a number
fn clog2(n: u128) -> usize {
    (u128::BITS - n.leading_zeros()).try_into().unwrap()
}

/// Expands the @set_sr_for macro
fn set_sr_for(code: &Vec<String>) -> Result<String, String> {
    match code[0].as_str() {
        "@set_sr_for" => {
            if code.len() == 2 {
                match format_string_into_number(&code[1]) {
                    Some((num, false)) => {
                        let sr_value = clog2(num / 8) + 1;
                        if sr_value < 16 {
                            Ok(format_set_sr_for(sr_value))
                        } else {
                            Err(format!("Error, the assembler does not support @set_sr_for with a value of {}.", num))
                        }
                    },
                    None | Some((_, true)) => Err(format!("Error, macro @set_sr_for should take a single number as argument.")),
                }
            } else {
                Err(format!("Error, macro @set_sr_for should take a single 1-byte number as argument."))
            }
        },
        _ => Err(format!("Error in the assembler, set_sr_for should not parse {:?}!", code)),
    }
}

/// Generates the code used for a @set_sr_for macro
fn format_set_sr_for(sr_value: usize) -> String {
    format!("set {}
    cpy SR", sr_value)
}

