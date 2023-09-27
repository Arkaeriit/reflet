; A small program that tests the interrupts in the simulator.
; It exits after 3 interruptions which all generate a debug sign

; R09 contains the counter of interrupts

label ISR
    push
    set 1
    add R09
    debug
    cpy R09
    pop
    retint

label start
    set 0
    cpy R9
    setlab ISR
    setint 1
    set8 0x04
    cpy SR
    label loop
        set 2
        les R09
        cmpnot
        jifl loop
    quit


