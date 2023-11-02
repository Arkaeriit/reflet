
;--------------------- Saving and restoring tasks context ---------------------;

; Save all registers in RAM and put the SP to get them in the register given as
; fisrt argument. The bottom value would be the value in the interrupt number
; given as second argument.
@macro ros_save_context 2
    ; Pushing a black space and the 3 first registers
    push
    push
    pushr. R1
    pushr. R2
    ; Saving SP, rewinding 4 time to go to the first value, adding the extra
    ; value, and restoring SP
    read SP
    cpy R1
    pop
    pop
    pop
    pop
    getintstack $2
    push
    mov. SP R1
    ; Pushing all other registers
    pushr. R3
    pushr. R4
    pushr. R5
    pushr. R6
    pushr. R7
    pushr. R8
    pushr. R9
    pushr. R10
    pushr. R11
    pushr. R12
    pushr. SR
    ; Putting SP in R1, PC doesn't need to be saved as we put the return address
    ; we want in the stack
    mov. $1 SP
@end

; Takes a stack pointer in a register given as first argument and then performs
; a return to also restore the PC.
@macro ros_restore_context 1
    mov. SP $1
    popr. SR
    popr. R12
    popr. R11
    popr. R10
    popr. R9
    popr. R8
    popr. R7
    popr. R6
    popr. R5
    popr. R4
    popr. R3
    popr. R2
    popr. R1
    pop
    ret
@end


