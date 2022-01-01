; This program is meant to test all the instructions and put some results in an
; array. The address of the array is defined by a constant. The point of the
; program is to do easy regression testing on the RTL processor and the
; simulator. This assumes that the assembler works correctly. It is designed
; to work on a 16 bit CPU

;---------------------------------- Constants ---------------------------------;

@align_word
label array_location
@constant 36864 ; 0x9000
label array_size
@constant 20
label stack_start
@constant 32768 ; 0x8000

;---------------------------------- Utilities ---------------------------------;

; This function writes the word in R1 into the address at R9 and increment
; the address at R9
label write_next_word_in_array
    read R1
    str R9
    @set_wordsize_byte
    add R9
    cpy R9
    ret

;------------------------- Global array manipulations -------------------------;

; This function initalizes the array with all zeros
; It does nothings to preserve the cotent of registers
label init_arr
    setlab array_location
    load WR
    cpy R1
    setlab array_size
    load WR
    cpy R2
    set 0
    cpy R3
    label init_loop
        ; str 0
        set 0
        str R1
        ; increment R1
        @set_wordsize_byte
        add R1
        cpy R1
        ; increment R3
        set 1
        add R3
        cpy R3
        ; Jump back if not finished
        read R2
        eq R3
        cmpnot
        setlab init_loop
        jif
    ret

; This function runs a debug instruction on all elements in the array
label debug_arr
    setlab array_location
    load WR
    cpy R1
    setlab array_size
    load WR
    cpy R2
    set 0
    cpy R3
    label debug_loop
        ; read and debug
        load R1
        debug
        ; increment R1
        @set_wordsize_byte
        add R1
        cpy R1
        ; increment R3
        set 1
        add R3
        cpy R3
        ; Jump back if not finished
        read R2
        eq R3
        cmpnot
        setlab debug_loop
        jif
    ret

;--------------------------------- Actual test --------------------------------;

; R9 holds the address where the next number will be put in the array
; R10 holds the address of the function used to register a nuber in the array

; This small macro register the number in the working register
@define register_word 0
    cpy R1
    read R10
    call
@end

; This is the test function. It is supposed to do all the test and initializations
label true_test
    ; varable init
    setlab array_location
    load WR
    cpy R9
    setlab write_next_word_in_array
    cpy R10
    ; testing set command
    set 6
    register_word ; 1st word should be 6 = 0x0006
    ; testing cpy and read
    set+ 3200
    cpy R2
    read R2
    register_word ; 2nd word should be 3200 = 0x0C80
    ; testing add
    set8 10
    cpy R1
    set8 15
    add R1
    register_word ; 3rd word should be 25 = 0x0019
    ; testing sub
    set8 100
    cpy R2
    set8 200
    sub R2
    register_word ; 4th word should be 100 = 0x0064
    set8 50
    sub R2
    register_word ; 5th word should be -50 = 0xFFCE
    ; testing and
    set8 170
    cpy R2
    set8 85
    and R2
    register_word ; 6th word should be 0 = 0x0000
    set 2
    and R2
    register_word ; 7th word should be 2 = 0x0002
    ; testing or
    set8 170
    cpy R2
    set8 85
    or R2
    register_word ; 8th word should be 255 = 0x00FF
    ; testing xor
    set 13
    cpy R3
    set 14
    xor R3
    register_word ; 9th word should be 3 = 0x0003
    ; testing not
    not R2
    register_word ; 10h word should be 653655 = 0xFF55
    ; testing lsl
    set 1
    cpy R2
    set 7
    lsl R2
    register_word ; 11th word should be 14 = 0x000E
    ; testing lsr
    set 6
    lsr R2
    register_word ; 12th word should be 3 = 0x0003
    ; testing eq and jif
    set 1
    cpy R1
    set 1
    eq R1
    setlab eq-OK
    jif
    goto error
    label eq-OK
    set 0
    eq R1
    setlab error
    jif
    ; testing les and jif
    set 1
    les R1
    setlab error
    jif
    set 0
    les R1
    setlab les-OK
    jif
    goto error
    label les-OK
    ; testing load and store
    set8 90
    str SP
    set 0
    load SP
    register_word ; 13th word should be 90 = 0x005A
    ; testing pop and push
    set8 80
    push
    set 0
    pop
    register_word ; 14th word should be 80 = 0x0050
    ; testing CC2
    set8 13
    cc2
    register_word ; 15th word should be -13 = 0xFFF3
    ; testing ret and call
    callf test-ret-call ; 16th word sould be 10 = 0x000A
    ; testing cmpnot
    set 1
    cpy R1
    set 1
    eq R1
    setlab error
    cmpnot
    jif
    ; testing tbm
    set+ 65535
    tbm
    str SR
    set 0
    load SR
    tbm
    register_word ; 17th word should be 255 = 0x00FF
    ; 18th, 19th, and 20th bytes should stay at 0 = 0x0000
    ret

label error
    quit

label test-ret-call
    set 10
    register_word
    ret

;------------------------------------ Main ------------------------------------;
    
label start
    setlab stack_start
    load WR
    cpy SP
    callf init_arr
    callf true_test
    callf debug_arr
    quit

