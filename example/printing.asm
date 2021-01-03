;----------------------
;This lib contain some priting functions

;---------------------
;Prints the char in R1
label printc
    pushr R2 ;addrs
    pushr R4 ;waiting loop pointer
    pushr R5 ;copy of the status register
    read SR
    cpy R5
    set 6 ;geting in byte mode
    cpy SR
    set+ 0 ;UART tx_cmd addr
    cpy R2
    setlab printcLoop
    cpy R4
    label printcLoop
        read R2
        load R2 ;testing that R2 is 0 to see if we are ready to print
        cpy R12
        set 0
        eq R12
        read R4 ;until ready, go back
        jif
    set 1    ;computing the data addr
    add R2
    cpy R12
    read R1 ;writing the char
    str R12
    set 0   ;sending command
    str R2
    read R5 ;restoring status register
    cpy SR
    popr R5 ;restoring registers
    popr R4 
    popr R2
    ret
        
;----------------------
;compute the length of a 0-terminated string in R1 and write the result in R1
label strlen
    pushr R2 ;str pointer
    pushr R3 ;loop pointer
    pushr R4 ;buffer register to store a null byte as a comparaison test
    pushr R5 ;Copy of the status register
    read SR
    cpy R5
    set 6    ;getting tn byte-mode
    cpy SR
    mov R2 R3 ;init registers
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
    popr SR ;restoring tmp registers
    popr R4
    popr R3
    popr R2
    ret

;----------------------
;Print a string whose pointer is in R1 and whose length is in R2
label (print)
    pushr R1
    pushr R2
    pushr R3 ;loop pointer
    pushr R4 ;end loop pointer
    pushr R5 ;contain the constant 1
    setlab (print)-loop
    cpy R3
    setlab (print)-endloop
    cpy R4
    set 1
    cpy R5
    label (print)-loop
        set 0 ;testing for the end of the loop
        eq R2
        read R4
        jif
        load R1 ;printing the char pointed at R1
        cpy R12 ;saving R1 while calling the function printc
        pushr R1
        mov R1 R12
        callf printc  ;as reflet is litle-endian, no need for further process
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
    popr R5 ;restauring registers
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

;----------------------------
;print \n
label CR
    pushr R1
    set 10
    cpy R1
    callf printc
    popr R1
    ret

