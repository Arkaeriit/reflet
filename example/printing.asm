;----------------------
;This lib conain some priting functions

;---------------------
;Prints the char in R1
label printc
    pushr R2 ;addrs
    pushr R4 ;waiting loop pointer
    pushr SR ;using byte mode
    set 6
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
    popr SR
    popr R4 ;restoring registers
    popr R2
    ret
        
;----------------------
;compute the length of a 0-terminated string in R1 and write the result in R1
label strlen
    pushr R2 ;str pointer
    pushr R3 ;loop pointer
    pushr R4 ;buffer register to store a null byte as a comparaison test
    pushr SR ;We need to be in byte-mode for the reading
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
    popr SR ;restauring tmp registers
    popr R4
    popr R3
    popr R2
    ret

