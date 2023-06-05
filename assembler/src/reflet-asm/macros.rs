
/// Runs all the implementation-specifics macros
pub fn macros(code: &Vec<String>) -> Result<Option<String>, String> {
    match set8(code) {
        Ok(None) => Ok(None), // Replace with more macro if there is some in the future
        x => x
    }
}

/// Expands the set8 macro
fn set8(code: &Vec<String>) -> Result<Option<String>, String> {
    match code[0].as_str() {
        "@set8" => {
            if code.len() == 2 {
                match format_string_into_byte(&code[1]) {
                    Some(num) => Ok(Some(format_set8(num))),
                    None => Err(format!("Error, macro @set8 should take a single 1-byte number as argument.")),
                }
            } else {
                Err(format!("Error, macro @set8 should take a single 1-byte number as argument."))
            }
        },
        _ => Ok(None),
    }
}

/// Generates the code used for a set8 macro
fn format_set8(num: u8) -> String {
    format!("set 4
    cpy 12
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

