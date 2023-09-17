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

/* ------------------------------- @set_sr_for ------------------------------ */

/// Comput the ceil of the log2 of a number
fn clog2(n: u128) -> usize {
    let bit_used: u128 = (u128::BITS - n.leading_zeros()).try_into().unwrap();
    (bit_used - 1).try_into().unwrap()
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
    format!("set 8
    cpy R12
    set {}
    lsl R12
    cpy SR", sr_value)
}

/* --------------------------------- Testing -------------------------------- */

#[test]
fn test_clog2() {
    assert_eq!(clog2(1), 0);
    assert_eq!(clog2(2), 1);
    assert_eq!(clog2(3), 1);
    assert_eq!(clog2(4), 2);
    assert_eq!(clog2(5), 2);
    assert_eq!(clog2(6), 2);
    assert_eq!(clog2(7), 2);
    assert_eq!(clog2(8), 3);
    assert_eq!(clog2(9), 3);
}

#[test]
#[should_panic]
fn test_clog2_fail() {
    let _ = clog2(0);
}

#[test]
fn test_set_sr_for() {
    fn direct_sr_for(n: usize) -> Result<String, String> {
        let mut ret = Vec::<String>::new();
        for s in format!("@set_sr_for {}", n).split(" ") {
            ret.push(s.to_string());
        }
        set_sr_for(&ret)
    }
    assert_eq!(direct_sr_for(8), Ok(format_set_sr_for(1)));
    assert_eq!(direct_sr_for(16), Ok(format_set_sr_for(2)));
    assert_eq!(direct_sr_for(32), Ok(format_set_sr_for(3)));
    assert_eq!(direct_sr_for(64), Ok(format_set_sr_for(4)));
}

