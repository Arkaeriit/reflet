;This program prints each prime numbers representable on the 
;processor's registers
;As this is not very efficient, it is more ment to be a proof of 
;concept than anything else.

;-----------------------
;Put 1 in R2 if the number in R1 is prime and put 0 otherwise
label isPrime
    pushr R3 ;loop counter
    pushr R4 ;end pointer
    pushr R5 ;loop pointer
    pushr R6 ;other end pointer
    pushr R7 ;intDiv
    set 2 ;init registers
    cpy R3
    setlab isPrimeNot
    cpy R4
    setlab isPrimeLoop
    cpy R5
    setlab isPrimeYes
    cpy R6
    setlab intDiv
    cpy R7
    label isPrimeLoop
        read R3 ;if we tried all numbers, is prime
        eq R1
        read R6
        jif
        pushr R1 ;testing R1%R3
        mov R2 R3
        read R7 ;calling intDiv
        call
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
    popr R7
    popr R6
    popr R5
    popr R4
    popr R3
    set 1
    cpy R2
    ret
    label isPrimeNot ;in case of not a prime
    popr R7
    popr R6
    popr R5
    popr R4
    popr R3
    set 0
    cpy R2
    ret

;-------------------------
;Test if the number in R1 is prime and if so, print it
label processNum
    pushr R2
    callf isPrime
    set 0
    eq R2
    setlab processNumEnd
    jif
    callf printNum ;we have a prime
    callf CR
    label processNumEnd
    popr R2
    ret

;------------------------
;Loop throught all the integers from 3 to the 
;bigest one and call processNum on them
label runNumbers
    pushr R1 ;scratch register
    pushr R2 ;The function
    pushr R3 ;loop counter
    pushr R4 ;loop pointer
    setlab processNum ;init registers
    cpy R2
    set 3
    cpy R3
    setlab runNumbersLoop
    cpy R4
    label runNumbersLoop
        mov R1 R3 ;calling the func
        read R2
        call
        set 1 ;updating R3
        add R3
        cpy R3
        set 0 ;testing for the end of the loop
        eq R3
        cmpnot
        read R4
        jif
    popr R4 ;restauring registers
    popr R3
    popr R2
    popr R1
    ret

label start
    set 2 ;The main loop starts at 3 so we manualy print 2
    cpy R1
    callf printNum
    callf CR
    callf runNumbers
    quit
    
