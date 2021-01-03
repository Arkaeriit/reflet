;This program prints each prime numbers representable on the 
;processor's registers

;-----------------------
;Put 1 in R2 if the number in R1 is prime and put 0 otherwise
label isprime
    pushr R3 ;loop counter
    pushr R4 ;end pointer
    pushr R5 ;loop pointer
    pushr R6 ;other end pointer
    set 2 ;init registers
    cpy R3
    setlab isPrimeNot
    cpy R4
    setlab isPrimeLoop
    cpy R5
    setlab isPrimeYes
    cpy R6
    label isPrimeLoop
        read R3 ;if we tried all numbers, is prime
        eq R1
        read R6
        jif
        pushr R1 ;testing R1%R3
        mov R2 R3
        callf intDiv
        popr R1
        set 0
        eq R2
        read R4
        jif
        set 1 ;incrementing R3
        add R3
        cpy R3
        read R5 ;going back
        jmp
    label isPrimeYes ;in case of a prime
    popr R6
    popr R5
    popr R4
    popr R3
    set 1
    cpy R2
    ret
    label isPrimeNot ;in case of not a prime
    popr R6
    popr R5
    popr R4
    popr R3
    set 0
    cpy R2
    ret

label start
    set 4
    cpy R1
    callf isprime
    read R2
    debug
    set 8
    cpy R1
    callf isprime
    read R2
    debug
    set 13
    cpy R1
    callf isprime
    read R2
    debug
    quit
    
