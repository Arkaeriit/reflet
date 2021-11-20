;----------------------
;This lib contains some priting functions.
;It needs an implementation of printc
;it depends on math.asm and string.asm

;----------------------
;Print a string whose pointer is in R1 and whose length is in R2
label (print)
    pushr R1
    pushr R2
    pushr R3 ;loop pointer
    pushr R4 ;end loop pointer
    pushr R5 ;contain the constant 1
    pushr R6 ;pointer to printc
    setlab (print)-loop
    cpy R3
    setlab (print)-endloop
    cpy R4
    set 1
    cpy R5
    setlab printc
    cpy R6
    label (print)-loop
        set 0 ;testing for the end of the loop
        eq R2
        read R4
        jif
        tbm
        load R1 ;printing the char pointed at R1
        tbm
        cpy R12 ;saving R1 while calling the function printc
        pushr R1
        mov R1 R12
        read R6 ;calling printc
        call  ;as reflet is litle-endian, no need for further process
        popr R1 ;restoring R1
        read R1 ;updating R1 and R2
        add R5
        cpy R1
        read R2
        sub R5
        cpy R2
        read R3 ;going back on top of the loop
        jmp
    label (print)-endloop
    popr R6 ;restauring registers
    popr R5
    popr R4
    popr R3
    popr R2
    popr R1
    ret

;--------------------------
;print the 0-terminated string in R1
label print
    pushr R2
    pushr R1
    callf strlen
    mov R2 R1
    popr R1
    callf (print)
    popr R2
    ret

;-------------------------------
;convert the number in R1 into a decimal number in the
;string in R2
label num2dec
    set 0 ;Special case if the number to covert is 0
    eq R1
    setlab num2dec0
    jif
    pushr R1 ;Normal case
    pushr R2 ;R2 will be a scratch register
    pushr R3 ;R3 will hold a pointer to the current char of the string
    pushr R4 ;R4 will hold 10 to to division
    pushr R5 ;R5 will hold 48 to convert numbers to ASCII
    pushr R6 ;loop pointer
    pushr R7 ;pointer to intDiv
    pushr R8 ;copy of the status register
    mov R8 SR ;byte mode
    set 6
    cpy SR
    mov R3 R2 ;init registers
    setlab num2decLoop
    cpy R6
    setr R4 10
    setr R5 48
    setlab intDiv
    cpy R7
    label num2decLoop
        mov R2 R4 ;preparing the division
        read R7 ;calling intDiv
        call
        read R2 ;converting modulo to ASCII
        add R5
        str R3 ;writing result in string. Reflet is little endian so no need for masks
        set 1 ;updating R3
        add R3
        cpy R3
        set 0 ;If the quotient is not 0, we loop back to do the next digit
        eq R1
        read R6
        cmpnot
        jif
    set 0  ;Null-terminating the string
    str R3
    mov SR R8
    popr R8 ;restauring registers
    popr R7
    popr R6
    popr R5
    popr R4
    popr R3
    popr R2
    mov R1 R2 ;fliping the string
    callf strFlip
    popr R1
    ret
    label num2dec0 ;If the number to convert is 0
    set+ 48 ;writing '0' in the first byte
    str R2
    set 1 ;null-terminating the next one
    add R2
    str R12
    set 0
    str R12
    ret

;-----------------------
;Print in decimal the number in R1
label printNum
    pushr R2 ;string
    set+ 100 ;creating a string on the stack to store the result
    add SP
    cpy R2
    callf num2dec ;filling the string with the decimal number
    pushr R1 
    mov R1 R2 ;printing it
    callf print
    popr R1
    popr R2
    ret
    
