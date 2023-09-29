;-------------------------------------------------
; This lib contains some macros used to jump to a
; function and back using a register as a link
; register instead of using the stack in the RAM.

; Jumps to a function using its label name.
; The first argument of the macro is the label of
; the function, the second is the register used
; as a link register.
@macro callf_lr 2
    setlab $1
    call_lr $2
@end

; Jumps to the content of the working register
; using a link register given as argument.
@macro call_lr 1
    cpy R12
    set 4
    add PC
    cpy $1
    read R12
    cpy PC
@end

; Jumps back from a function using a link
; register. That register should be given as
; argument to the macro.
@macro ret_lr 1
    read $1
    cpy PC
@end

