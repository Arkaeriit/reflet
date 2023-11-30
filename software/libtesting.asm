;This file contain tests for the various function of math.asm and printing.asm
;It should be printed as 'reflet-nasm libtesting.asm math.asm printing.asm -o out.bin'
;It can then be tested as following: (TODO when the simulator will have a nice cli write a tuto here)

@import libs/import_all_libs.asm

@align_word
label testString
@string "Reflet is a neat ISA!"
@rawbytes 0A 0
label usedString ; This string is empty but will be filled in by the program
@rawbytes 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

label start
    callf testingPrinting
    callf testingMath
    callf testLinkedJumps
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
    set 12 ; testing clog2
    cpy R1
    callf clog2
    read R1
    debug ; expecting clog2(12) = 4
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
    cpy R3
    setlab testString
    cpy R1
    setlab usedString
    cpy R2
    callf memcpy ; We put the message in the string in R2
    mov. R1 R2
    callf print ;should print "Reflet is a neat ISA!\n"
    setlab usedString
    cpy R2
    set+ 0xABCD
    cpy R1
    callf printNum ;expecting '43981'
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

; Tests the library linked_jumps.asm
label testLinkedJumps
    debug
    callf_lr linkedFunction R10
    debug
    ret

quit ; Should not be executed
label linkedFunction
    debug
    ret_lr R10
    quit ; Should not be executed

