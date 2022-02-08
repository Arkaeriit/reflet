; This is a smaller version of test_instruction.asm than more or less only
; tests the set and the str instruction. It does not need any memory. It
; should run on a 8 bit processor and the debug array is assumed to be at
; the address 0x80.

; At the end, the array should end up with the values: 0, 7, and 100 (0x64)

;---------------------------------- Main code ---------------------------------;

label main
    ; Generation the array address in R2 and R3
    set 4
    cpy R1
    set 8
    lsl R1
    cpy R2
    cpy R3
    ; Fill the array
    set 0
    write_in_r2_and_increment
    set 7
    write_in_r2_and_increment
    set8 100
    write_in_r2_and_increment
    ; debug the content
    debug_from_r3_and_increment
    debug_from_r3_and_increment
    debug_from_r3_and_increment

    quit

;----------------------------------- Macros -----------------------------------;

@define write_in_r2_and_increment 0
    str R2
    set 1
    add R2
    cpy R2
@end

@define debug_from_r3_and_increment 0
    load R3
    debug
    set 1
    add R3
    cpy R3
@end

