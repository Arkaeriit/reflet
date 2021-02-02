wordsize 16
;This file contain tests for the various function of math.asm and printing.asm
;It should be printed as 'reflet-nasm libtesting.asm math.asm printing.asm -o out.bin'
;It can then be tested as following: (TODO when the simulator will have a nice cli write a tuto here)

label testString
data "Reflet is a neat ISA!"
rawbytes 10 0

label start
    callf testingPrinting
    callf testingMath
    quit

label testingMath
    set 13 ;testing intMult
    cpy R1
    set 15
    cpy R2
    callf intMult
    read R1
    debug ;expexting 13x5 = 195 = 0xc3
    set 15 ;testing intDiv
    cpy R1
    set 4
    cpy R2
    callf intDiv
    read R1
    debug ;expecting 15/4 = 3 = 0x3
    read R2
    debug ;expecting 15%4 = 3 = 0x3
    set 10 ;testing intPow
    cpy R1
    set 4
    cpy R2
    callf intPow
    read R1
    debug ;expecting 10^4 = 10000 = 0x2710
    ret

label testingPrinting
    set+ 64
    cpy R1
    callf printc ;should print '@'
    callf CR ;should print a newline
    setlab testString
    cpy R1
    callf strlen
    read R1
    debug ;expecting the length of the string which is 22 = 0x16
    setlab testString
    cpy R1
    callf print ;should print "Reflet is a neat ISA!\n"
    set+ 200
    add SP
    cpy R2
    set+ 1234
    cpy R1
    callf printNum ;expecting '1234'
    callf CR
    set+ 34567
    cpy R1
    callf printNum ;expecting '34567'
    callf CR
    set 0
    cpy R1
    callf printNum ;expecting '0'
    callf CR
    callf getch  ;Ask for a char
    callf printc ;Print it 3 times
    callf printc
    callf printc
    callf CR
    ret

