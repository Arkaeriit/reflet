wordsize 8
; This small program is ment to be assembled with reflet-masm
; it asks a character from the user and print it 10 times
; To assemble it, do reflet-masm exampleIO.asm exampleIO.bin
; To run it, do reflet-sim exampleIO.rbin
; (As this is a 8 bit program, the default configuration is enought)

label start
set 2 ; preparing the address 2
cpy r1
set 0 ; putting 0 in 0x2 to ask for a char
str r1
set 3 ; retriving char
cpy r1
load r1
push ; preparing the address Ox1
set 1
cpy r1
pop  ; putting what was in 0x3 in 0x1
str r1


set 10 ; loop pointer
push 
label loop
set 0
str WR ; printing
pop
sub r1 ; Substracting one
eq r2 ; are we equal to 0 ?
push
not CR
cpy CR
setlab loop
jif

set 10 ;preparing a new line
str r1
set 0
str WR

