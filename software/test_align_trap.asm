; This program performs a misaligned read and catches it in a trap that
; runs a debug operation. It is meant to run on a 16 bit processor.

@define trap_mask 0
    set 5
    cpy R1
    set 1
    lsl R1
@end

label trap
    debug
    ; Mask out the bit used to enable the trap
    trap_mask
    not WR
    and SR
    cpy SR
    retint

label start
    ; set the trap
    setlab trap
    setint 0
    trap_mask
    or SR
    cpy SR
    ; enable interrupt
    set 2
    or SR
    cpy SR
    ; invalid read
    set 1
    load WR
    ; end
    quit

