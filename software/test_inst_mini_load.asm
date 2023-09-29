; This is a smaller version of test_instruction.asm than more or less only
; tests the load and the str instruction. It does not need any memory. It
; should run on a 8 bit processor and the debug array is assumed to be at
; the address 0x80.

; At the end, the array should end up with the values: 2, 4, 6, and 8

;---------------------------------- Main code ---------------------------------;

label main
    ; Generation the array address in R2, R3, and R4
    set 4
    cpy R1
    set 8
    lsl R1
    cpy R2
    cpy R3
    cpy R4
    ; Fill the array
    set 2
    cpy R5
    write_in_r3_and_increment
    update_from_R5_and_arr
    update_from_R5_and_arr
    update_from_R5_and_arr
    ; debug the content
    debug_from_r2_and_increment
    debug_from_r2_and_increment
    debug_from_r2_and_increment
    debug_from_r2_and_increment

    quit

;----------------------------------- Macros -----------------------------------;

@macro write_in_r3_and_increment 0
    str R3
    set 1
    add R3
    cpy R3
@end

@macro debug_from_r2_and_increment 0
    load R2
    debug
    set 1
    add R2
    cpy R2
@end

@macro update_from_R5_and_arr 0
    load R4
    add R5
    cpy R5
    write_in_r3_and_increment
@end

