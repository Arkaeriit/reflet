use macro_asm_builder::utils::format_string_into_number;

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
                match format_string_into_number(&code[1]) {
                    Some((num, _)) => if num <= 0xFF {
                        Ok(Some(format_set8(num.try_into().unwrap())))
                    } else {
                        Err(format!("Error, macro @set8 should take a single 1-byte number as argument."))
                    },
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
    cpy R12
    set {}
    lsl R12
    cpy R12
    set {}
    or R12", (num & 0xF0) >> 4, num & 0x0F)
}

