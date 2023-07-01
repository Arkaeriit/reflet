; This file test comparison operations and macros

@import ./libs/import_all_libs.asm

label game_over_str
@string-0 "The comparison operations are broken. D:\n"

label yay_str
@string-nl-0 "The comparison operations are fine. :D"

label game_over
    setlab game_over_str
    cpy R1
    callf print
    quit

; Ensure that the comparison bit is set to 1
@define comp_to_1 0
    setlab game_over
    cmpnot
    jif
@end

; Ensure that the comparison bit is set to 0
@define comp_to_0 0
    cmpnot
    comp_to_1
@end

; Takes an operation, executite on more, eq, and
; less from a referen and runs each of the 3
; macros
@define run_test 4
    set 10
    cpy R1
    set 15
    $1 R1
    $2
    set 10
    $1 R1
    $3
    set 5
    $1 R1
    $4
@end

label start
    run_test eq      comp_to_0 comp_to_1 comp_to_0
    run_test les     comp_to_0 comp_to_0 comp_to_1
    run_test leseq   comp_to_0 comp_to_1 comp_to_1
    run_test great   comp_to_1 comp_to_0 comp_to_0
    run_test greateq comp_to_1 comp_to_1 comp_to_0
    run_test neq     comp_to_1 comp_to_0 comp_to_1

    setlab yay_str
    cpy R1
    callf print
    quit

