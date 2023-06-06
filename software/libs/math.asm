;---------------------
;This file contain functions used to do some basic math opperations

;---------------------
;Multiplies R1 by R2 and put the result in R1
label intMult
    pushr. R2
    pushr. R3 ; loop counter
    pushr. R4 ; tmp result
    set 0
    cpy R4
    pushr R5 ; scratch register (1)
    set 1
    cpy R5
    pushr. R6 ; start loop pointer
    setlab intMult.loop
    cpy R6
    pushr. R7 ; mid loop pointer
    setlab intMult.midloop
    cpy R7
    ; Number of bits in a number
    set_wordsize_bit
    cpy R3
    label intMult.loop
        ; Check if we need to increase result in that loop turn
        read R5
        and R2
        eq R5
        cmpnot
        read R7
        jif ; jump to .midloop
        ; Increase tmp result
        read R1
        addup R4
        read r4
        label intMult.midloop
        ; Update R1 and R2
        read R1
        lsl R5
        cpy R1
        read R2
        lsr R5
        cpy R2
        ; count down and loop back if needed
        set 1
        cc2
        addup R3
        set 0
        eq R3
        cmpnot
        read R6 ; jump to .loop
        jif
    mov. R1 R4
    popr. R7
    popr. R6
    popr. R5
    popr. R4
    popr. R3
    popr. R2
    ret

;--------------------------
;Put the quotient of the integer division of R1 by R2 into R1 and the modulo in R2
label intDiv
    pushr R3 ;quotient tmp
    pushr R5
    pushr R6
    setlab intDivLoopStart
    cpy R5
    setlab intDivLoopEnd
    cpy R6
    set 0
    cpy R3
    label intDivLoopStart
        read R1 ;checking for modulo < quotient
        les R2
        read R6
        jif
        read R1 ;Redusing the modulo
        sub R2
        cpy R1
        set 1   ;increasing quotient
        add R3
        cpy R3
        read R5 ;going back up
        jmp
    label intDivLoopEnd
    read R1 ;putting results into correc places
    cpy R2
    read R3
    cpy R1
    popr R6
    popr R5
    popr R3
    ret
    
;--------------------------
;put R1 exponent R2 into R1
label intPow
    pushr R3 ;tmp result
    pushr R4 ;loop counter
    pushr R5 ;end loop pointer
    pushr R6 ;start loop pointer
    pushr R7 ;intMult pointer
    setlab intPowLoopEnd
    cpy R5
    setlab intPowLoopStart
    cpy R6
    setlab intMult
    cpy R7
    set 1
    cpy R3
    set 0
    cpy R4
    label intPowLoopStart
        read R4  ;testing loop
        eq R2
        read R5
        jif
        set 1    ;increasing loop counter
        add R4
        cpy R4
        pushr R1 ;Preserving R1 and R2
        pushr R2
        read R3  ;R3 = R1 * R3
        cpy R2
        read R7 ;calling func
        call 
        read R1
        cpy R3
        popr R2 ;restoring R1 and R2
        popr R1
        read R6 ;back to the top
        jmp
    label intPowLoopEnd
    read R3 ;storing result
    cpy R1
    popr R7 ;restoring registers
    popr R6
    popr R5
    popr R4
    popr R3
    ret
        
