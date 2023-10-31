;This macro is used to put the result of the 2 word macro in the WR.
;It is used to form set+ or setlab
@macro set_to_constant 2
    mov. R11 SR
    @set_default_sr
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

@macro set+ 1
    set_to_constant @constant $1
@end

@macro setlab 1
    set_to_constant @labref $1
@end

@macro goto 1
    setlab $1
    jmp
@end

@macro callf 1
    setlab $1
    call
@end

@macro jifl 1
    setlab $1
    jif
@end

@macro setr 2
    set+ $2
    cpy $1
@end

@macro mov. 2
    read $2
    cpy $1
@end

@macro mov 2
    cpy R12
    mov. $1 $2
    read R12
@end

@macro pushr. 1
    read $1
    push
@end

@macro pushr 1
    cpy R12
    pushr. $1
    read R12
@end

@macro popr. 1
    pop
    cpy $1
@end

@macro popr 1
    cpy R12
    popr. $1
    read R12
@end

@macro addup. 1
    add $1
    cpy $1
@end

@macro addup 1
    cpy r12
    addup. $1
    read r12
@end

@macro inc. 1
    set 1
    addup. $1
    cpy $1
@end

@macro inc 1
    cpy r12
    inc. $1
    read r12
@end

@macro inc_ws. 1
    @set_wordsize_byte
    addup. $1
@end

@macro inc_ws 1
    cpy r12
    inc_ws. $1
    read r12
@end

@macro jmp 0
    cpy PC
@end

@macro label 1
    @label $1
@end

@macro rawbyte 1
    @rawbytes $1
@end

@macro set8 1
    @set8 $1
@end

; Takes as first argument a value to mask the SR with and as second a register to load from
@macro @loadX 2
    ; Preserve and set SR
    mov. R11 SR
    @set_sr_for $1
    ; load
    load $2
    cpy R12
    ; Restore SR
    mov. SR R11
    read R12
@end

; Same idea as @loadX but for str
@macro @strX 2
    ; Preserve the value
    cpy R11
    ; In R12, put the xor of old SR and new SR
    @get_sr_for $1
    xor SR
    cpy R12
    ; Xor R12 and the current SR to get the new SR
    read SR
    xor R12
    cpy SR
    ; Read back the value and store it
    read R11
    str $2
    cpy R11
    ; Xor R12 and SR to get the old SR
    read R12
    xor SR
    cpy SR
    ; Put the value back in the WR
    read R11
@end

; load/store 8 bits
@macro load8 1
    @loadX 8 $1
@end
@macro str8 1
    @strX 8 $1
@end

; Load/store 16 bits
@macro load16 1
    @loadX 16 $1
@end
@macro str16 1
    @strX 16 $1
@end

; Load/store 32 bits
@macro load32 1
    @loadX 32 $1
@end
@macro str32 1
    @strX 32 $1
@end

; load/store 8 bits swifters
@macro load8. 1
    tbm
    load $1
    tbm
@end
@macro str8. 1
    tbm
    str $1
    tbm
@end

; WR = $1 - WR
@macro subto 1
    cc2
    add $1
@end

; WR = WR - $1
@macro sub 1
    subto $1
    cc2
@end

; No op
@macro nop 0
    read WR
@end
@macro slp 0
    nop
@end

; Sets the wordsize in bits into WR
@macro set_wordsize_bit 0
    @set_wordsize_byte
    add WR
    add WR
    add WR
@end

; Less or equal
@macro leseq 1
    cpy R12
    read $1
    les R12
    cmpnot
@end

; Greater
@macro great 1
    leseq $1
    cmpnot
@end

; Greater or equal
@macro greateq 1
    les $1
    cmpnot
@end

; Not equal
@macro neq 1
    eq $1
    cmpnot
@end

