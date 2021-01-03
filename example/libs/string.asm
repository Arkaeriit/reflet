;----------------------
;This lib contains some function to manipulate C-strings

;----------------------
;compute the length of a 0-terminated string in R1 and write the result in R1
label strlen
    pushr R2 ;str pointer
    pushr R3 ;loop pointer
    pushr R4 ;buffer register to store a null byte as a comparaison test
    pushr R5 ;Copy of the status register
    read SR
    cpy R5
    set 6    ;getting in byte-mode
    cpy SR
    read R1 ;init registers
    cpy R2
    setlab strlenLoop 
    cpy R3
    set 0
    cpy R4
    label strlenLoop
        load R2 ;reading byte
        eq R4   ;comparing
        cmpnot
        set 1   ;incresing the str pointer
        add R2
        cpy R2
        read R3 ;jumping back if needed
        jif
    set 1 ;decreasing the str pointer by 1 as we increased it even if not needed
    cpy R3
    read R2
    sub R3
    sub R1 ;computing the offset
    cpy R1 ;puting the result
    read R5 ;restoring SR
    cpy SR
    popr R5 ;restoring tmp registers
    popr R4
    popr R3
    popr R2
    ret

;-------------------------------------------
;Fill R2 bytes with the char in R3 at the address starting at R1
label memset
    pushr R1
    pushr R2
    pushr R3
    pushr R5 ;loop pointer
    pushr R6
    pushr R4 ;saving the status register
    mov R4 SR
    set 6
    cpy SR
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
        str R1
        read R1 ;updating R1 and R2
        add R12
        cpy R1
        read R2
        sub R12
        cpy R2
        read R5 ;looping back
        jmp
    label memsetLoopEnd
    read R4 ;restoring SR
    cpy SR
    popr R4 ;restoring registers
    popr R6
    popr R4
    popr R3
    popr R2
    popr R1
    ret

;-------------------------------------------
;Flip the string in R1
label strFlip
    pushr R2 ;length
    pushr R3 ;current offset
    pushr R4 ;max offset
    pushr R5 ;loop pointer
    pushr R6
    pushr R7 ;save status register
    pushr R8 ;scratch register
    pushr R1 ;init registers
    callf strlen
    read R1
    cpy R2
    popr R1
    set 0
    cpy R3
    set 1
    cpy R12
    read R2
    lsr R12
    cpy R4
    setlab strFlipLoop
    cpy R5
    setlab strFlipLoopEnd
    cpy R6
    mov R7 SR ;byte mode
    set 6
    cpy SR
    label strFlipLoop
        read R3 ;if done, go to the end
        eq R4
        read R6
        jif
        read R1 ;geting the start char
        add R3
        cpy R11 ;copy of the start index
        load WR
        push
        set 1 ;getting the end char
        cpy R12
        read R1
        add R2
        sub R3
        sub R12
        cpy R12 ;copy of the end index
        load R12
        cpy R8
        pop  ;putting the front char at the end
        str R12
        read R8 ;putting the end char at the front
        str R11
        set 1 ;updating R3
        add R3
        cpy R3
        read R5 ;jumping back
        jmp
    label strFlipLoopEnd
    read R7 ;restoring SR
    cpy SR
    popr R8 ;restoring registers
    popr R7
    popr R6
    popr R5
    popr R4
    popr R3
    popr R2
    ret

