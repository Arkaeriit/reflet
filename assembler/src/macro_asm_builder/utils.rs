
/// Tries to convert the string representation of a u128. If the input is
/// negative, use two complement and tells is on the boolean return.
pub fn format_string_into_number(s: &str) -> Option<(u128, bool)> {
    if s.len() == 0 {
        None
    } else {
        if &s[0..1] == "-" {
            match i128::from_str_radix(s, 10) {
                Ok(num) => {
                    let positive = num * -1;
                    let true_positive = positive as u128;
                    let not = !true_positive;
                    let cc2 = not + 1;
                    Some((cc2, true))
                },
                Err(_) => None,
            }
        } else {
            if s.len() >= 3 {
                if &s[0..2] == "0x" {
                    match u128::from_str_radix(&s[2..s.len()], 0x10) {
                        Ok(num) => Some((num, false)),
                        Err(_) => None,
                    }
                } else {
                    match u128::from_str_radix(s, 10) {
                        Ok(num) => Some((num, false)),
                        Err(_) => None,
                    }
                }
            } else {
                match u128::from_str_radix(s, 10) {
                    Ok(num) => Some((num, false)),
                    Err(_) => None,
                }
            }
        }
    }
}

