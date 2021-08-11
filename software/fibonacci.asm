;This program prints each numbers of the fibonachi sequence

;------------------------
;Loop throught all the integers from 3 to the 
;bigest one and call processNum on them
label fibo
    pushr R1 ;scratch register
    pushr R2 ;last number
    pushr R3 ;previous numbre
    pushr R4 ;loop pointer
    pushr R5 ;printNum
    pushr R6 ;CR
    set 1
    cpy R2
    set 1
    cpy R3
    setlab fiboLoop
    cpy R4
    setlab printNum
    cpy R5
    setlab CR
    cpy R6
    label fiboLoop 
        mov R1 R2 ;printing number
        read R5
        call    ;printNum
        read R6
        call    ;CR
        read R2 ;updating R2 and R3
        add R3
        cpy R2
        mov R3 R1
        read R3 ;testing for overflow
        les R2
        read R4 ;looping back if needed
        jif
    popr R6 ;restauring registers
    popr R5
    popr R4
    popr R3
    popr R2
    popr R1
    ret

label start
    set 1
    cpy R1
    callf printNum
    callf CR
    callf fibo
    quit

