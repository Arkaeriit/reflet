;----------------------
;This lib contains functions to do extended IO in the simulator

@macro set_io_cout 0
    set 0
@end

@macro set_io_cin 0
    set 1
@end

@macro set_io_cmd 0
    set 2
@end

@macro set_io_status 0
    set 3
@end

;---------------------
;Prints the char in R1
label printc
    ; Store and set SR
    pushr. R2
    mov. R2 SR
    set 6
    cpy SR
    ; Set char
    set_io_cout
    cpy R12
    read R1
    str R12
    ; Command
    set_io_cmd
    cpy R12
    set 1
    str R12
    ; Restoring SR
    mov. SR R2
    popr. R2
    ret
        
;----------------------------
;print \n
label CR
    pushr. R1
    set 10
    cpy R1
    callf printc
    popr. R1
    ret

;---------------------------
;ask the user for a char and put it in R1
;Set the comparison bit to 1 if the read is successful and to 0 otherwise
label getch
    ; Command
    set_io_cmd
    cpy R1
    set 2
    str8 R1
    ; Read status
    set_io_status
    cpy R1
    load8 R1
    cpy R1
    set 0
    eq R1
    ; Read char
    set_io_cin
    cpy R1
    load8 R1
    cpy R1
    ret

