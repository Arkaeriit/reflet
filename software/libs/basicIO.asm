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
    read R2 ;restoring SR
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
;Set the comparison bit to 1 if the read is successful and to 0 otherwise
label getch
   set 2 ;rx_cmd
   cpy R1
   set 0
   str8 R1
   load8 R1 ; checking success status
   cpy R1
   set 1
   eq R1
   set 3 ;rx_data
   cpy R1
   load8 R1
   cpy R1 ;saving result
   ret
   
