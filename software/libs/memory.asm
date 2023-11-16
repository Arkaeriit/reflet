;-------------------------------------------------
; This lib contains some functions to manipulate
; memory content or allocate it.

;-------------------------------------------------
;Fill R2 bytes with the char in R3 at the address
; starting at R1
label memset
    pushr R1
    pushr R2
    pushr R3
    pushr R5 ;loop pointer
    pushr R6
    setlab memsetLoop
    cpy R5
    setlab memsetLoopEnd
    cpy R6
    set 1  ;constant 1 in R12
    cpy R12
    label memsetLoop
        set 0 ;testing for loop end
        eq R2
        read R6
        jif
        read R3 ;setting mem
        str8 R1
        read R1 ;updating R1 and R2
        add R12
        cpy R1
        read R2
        sub R12
        cpy R2
        read R5 ;looping back
        jmp
    label memsetLoopEnd
    popr R6 ;restoring registers
    popr R5
    popr R3
    popr R2
    popr R1
    ret

;-------------------------------------------------
; Copies R3 bytes from the memory pointed by R1
; into the memory pointed by R2.
label memcpy
    ; As R1, R2, and R3 will be used, we want to save them
    pushr. R1
    pushr. R2
    pushr. R3
    pushr. SR
    pushr. R4
    pushr. R5
    ; Loop position pointers
    setlab memcpy_loop
    cpy R4
    setlab memcpy_loop_end
    cpy R5
    ; We go into pseudo byte mode here, we will go back to previous mode when we
    ; restore SR from the stack.
    @set_sr_for 8

    label memcpy_loop
        ; Quit loop if no byte need processing
        set 0
        eq R3
        read R5
        jif
        ; Copy a byte
        load R1
        str R2
        ; Update pointer and number of byte left
        set 1
        addup R1
        addup R2
        cc2
        addup R3
        ; Go back to start of the loop
        read R4
        jmp
    label memcpy_loop_end

    ; Restore registers
    popr. R5
    popr. R4
    popr. SR
    popr. R3
    popr. R2
    popr. R1
    ret

