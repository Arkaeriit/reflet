;----------------------
;This lib contains functions to do basic IO in the simulator

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
        
;----------------------------
;print \n
label CR
    pushr R1
    set 10
    cpy R1
    callf printc
    popr R1
    ret

