;----------------------
;This lib contains functions to do basic IO in the simulator

;---------------------
;Prints the char in R1
label printc
    pushr R2 ;storing SR
    mov R2 SR
    set 6
    cpy SR
    set 0    ;tx_cmd
    cpy R11
    set 1    ;tx_data
    cpy R12
    read R1 ;writing char
    str R12
    set 0   ;command
    str R11
    read R5 ;restoring SR
    cpy SR
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

;---------------------------
;ask the user for a char and put it in R1
label getch
   pushr R2   ;storing R2
   read SR ;Getting in byte mode
   cpy R2
   set 6
   cpy SR
   set 2 ;rx_cmd
   cpy R1
   set 0
   str R1
   set 3 ;rx_data
   load WR 
   cpy R1 ;saving result
   read R2 ;restauring values
   cpy SR
   popr R2
   ret
   
