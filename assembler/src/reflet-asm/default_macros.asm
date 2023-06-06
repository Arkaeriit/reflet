;This macro is used to put the result of the 2 word macro in the WR.
;It is used to form set+ or setlab
@define set_to_constant 2
    mov R11 SR
    @set_default_sr
    cpy SR
    @set_wordsize_byte
    cpy R12
    @align_word
    set 2
    add R12
    add PC
    jmp
    $1 $2
    ;Jumped here
    set 3
    add R12 ;Contains the word size
    cc2
    add PC
    load WR
    mov SR R11
@end

@define set+ 1
    set_to_constant @constant $1
@end

@define setlab 1
    set_to_constant @labref $1
@end

@define goto 1
    setlab $1
    jmp
@end

@define callf 1
    setlab $1
    call
@end

@define jifl 1
    setlab $1
    jif
@end

@define setr 2
    set+ $2
    cpy $1
@end

@define mov. 2
    read $2
    cpy $1
@end

@define mov 2
    cpy R12
    mov. $1 $2
    read R12
@end

@define pushr. 1
    read $1
    push
@end

@define pushr 1
    cpy R12
    pushr. $1
    read R12
@end

@define popr. 1
    pop
    cpy $1
@end

@define popr 1
    cpy R12
    popr. $1
    read R12
@end

@define addup. 1
    add $1
    cpy $1
@end

@define addup 1
    cpy r12
    addup. $1
    read r12
@end

@define inc. 1
    set 1
    addup. $1
    cpy $1
@end

@define inc 1
    cpy r12
    inc. $1
    read r12
@end

@define inc_ws. 1
    @set_wordsize_byte
    addup. $1
@end

@define inc_ws 1
    cpy r12
    inc_ws. $1
    read r12
@end

@define jmp 0
    cpy PC
@end

@define label 1
    @label $1
@end

@define rawbyte 1
    @rawbytes $1
@end

@define set8 1
    @set8 $1
@end

; Takes as first argument a value to mask the SR with and as second a register to load from
@define @loadX 2
    ; Preserve and set SR
    mov. R12 SR
    set $1
    or SR
    cpy SR
    ; load
    load $2
    cpy R11
    ; Restore SR
    mov. SR R12
    read R11
@end

; Same idea as @loadX but for str
@define @strX 2
    cpy R11
    ; Preserve and set SR
    mov. R12 SR
    read SR
    cpy R12
    set $1
    or SR
    cpy SR
    ; store
    read R11
    str $2
    cpy R11
    ; Restore SR
    mov. SR R12
    read R11
@end

; load/store 8 bits
@define load8 1
    @loadX 6 $1
@end
@define str8 1
    @strX 6 $1
@end

; Load/store 16 bits
@define load16 1
    @loadX 4 $1
@end
@define str16 1
    @strX 4 $1
@end

; Load/store 32 bits
@define load32 1
    @loadX 2 $1
@end
@define str32 1
    @strX 2 $1
@end

; load/store 8 bits swifters
@define load8. 1
    tbm
    load $1
    tbm
@end
@define str8. 1
    tbm
    str $1
    tbm
@end

