; This program is meant to test the use of fancy interrupt use such as changing
; the return value on the fly.

label first_int_which_will_be_bypassed 
    softint 1
    set8 0xAA
    retint

label int_that_changes_the_return_addr
    setlab debug_and_quit
    setintstack 2
    retint


label start
    setlab first_int_which_will_be_bypassed
    setint 2
    setlab int_that_changes_the_return_addr
    setint 1
    set 0xc
    cpy SR

    softint 2
    quit ; We will not run that one

label debug_and_quit
    debug
    quit

