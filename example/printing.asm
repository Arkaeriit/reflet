;----------------------
;This lib conain some priting functions

;---------------------
;Prints the char in R1
label printc
    pushr R2 ;addrs
    pushr R4 ;waiting loop pointer
    set+ 0 ;UART tx_cmd addr
    cpy R2
    setlab printcLoop
    cpy R4
    label printcLoop
        load R2 ;testing that R2 is 0 to see if we are ready to print
        cpy R12
        set 0
        less
        read R4 ;until ready, go back
        jif
    set 1    ;computing the data addr
    add R2
    cpy R12
    read R1 ;writing the char
    str R12
    set 0   ;sending command
    str R2
    popr R4 ;restoring registers
    popr R2
    ret
        
;----------------------
;compute the length of a 0-terminated string in R1 and write the result in R1
label strlength
    
    
