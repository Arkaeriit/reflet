wordsize 16
;This file contain tests for the various function of math.asm and printing.asm

label testString
data "Reflet is a neat ISA!"
rawbytes 10 0

label start
    ;callf testingMath
    ;debug
    callf testingPrinting
    ;debug
    quit

label testingMath
    set 13 ;testing intMult
    cpy R1
    set 15
    cpy R2
    callf intMult
    read R1
    debug
    set 15 ;testing intDiv
    cpy R1
    set 4
    cpy R2
    callf intDiv
    read R1
    debug
    read R2
    debug
    set 10 ;testing intPow
    cpy R1
    set 4
    cpy R2
    debug
    callf intPow
    read R1
    debug
    ret

label testingPrinting
    ;read SP
    ;debug
    set+ 64
    cpy R1
    callf printc
    setlab testString
    cpy R1
    callf strlen
    read R1
    debug
    ;read SP
    ;debug
    ret

