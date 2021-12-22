# Reflet assemblers

Two assemblers to create Reflet machine code.

This folder contains two programs, **reflet-pasm** which is a very bare-bone assembler written in C, and **reflet-masm** which is a richer assembler written mostly in Lua.

Both assemblers are case-insensitive for mnemonic but case-sensitive for anything else.

## RefletPASM
This is a pico-assembler. It can only assemble basic instructions as described in the main README.md. It would be very tedious to use it but you have total control over what instructions will be created.

If you want to insert a byte into your program without having to write the instruction for it, you can write `rawbyte` followed by a single byte in base 10.

If you want to jump to the address pointed in the working register, you can write `jmp` instead of `cpy R14`

Usage:
```
reflet-pasm <input file> <output file>
```

## RefletMASM
This is a macro-assembler. Not only can it assemble basic instructions but it can also assemble macro-instruction and manage labels. When using macro-instructions, the registers R11 and R12 might be used as temporary registers and overwritten. Furthermore, unless specified otherwise, the working registry will be modified.

To run it, the pico-assembler must be installed. 

### Usage
```
reflet-masm <input files> <options> -o <output file>
reflet-masm -help
```
The possible options are the following:

* -no-prefix: does not put the 'ASRM' prefix at the beginning of the output file.
* -no-stack-init: does not initialize the stack pointer. When not used, the stack pointer is by default initialized to just after the program.
* -set-stack-to xxx: set the stack pointer to the address given just after the flag. Incompatible with -no-stack-init.
* -start-addr xxx: tell the linker that the code should start at the given address. 
* -ignore-start: if set, the program will not start at the "start" label but at the beginning of the first input file.
* -wordsize xxx : set the wordsize to xxx (a number in bits). This overrides any information about wordsize written in the assembly file.

### Macro instructions
Macro instructions let you combine multiples instructions in one line to make your code cleaner. They are useful because the instruction size of 8 bits limits a lot what can be done with basic instructions. 

|Mnemonic|Followed by|Effect|
|--|--|--|
| set+ | A number | Put the argument number in the working register. The number can be as big as a register can hold. |
| mov | two registers | Copies the content of the second register into the first. Preserves the working register. |
| setr | a register and a number | Set the value of the given register to the given number. Preserves the working register. |
| pushr | a register | push the content of the register on the stack. |
| popr | a register | pop the top of the stack into the given register. |

### Labels
Labels can be used to jump to various parts of the program without having to calculate the address of the target instructions.

|Mnemonic|Followed by|Effect|
|--|--|--|
| label | A label | Note. the address of the following instruction as the label. |
| setlab | a label | Put the address of the label in the working register. |
| callf | a label | Call the function noted by the label. |
| goto | a label | Jumps to the label. |

The entry point of the program should be a point labeled `start`.

### Data-related macro
To write raw bytes to the output file, you can use macros that compile to `rawbyte`.

|Mnemonic|Followed by|Effect|
|--|--|--|
| rawbyte | A list of bytes in base 10 | Write in the output file the list of bytes. |
| data | a string between double quotes | Write the string to the output file |
Example:
```
label start
setlab str; We put a pointer to the strig in R1
cpy R1
callf print; We call a print function
quit

label str; We define a string
data "Hello, world!"
rawbytes 13 10 ;\r\n
rawbyte 0 ;null terminator

```

### Word size
The various macro-instructions can be compiled differently depending on the target processor's word size. Even if the code could be compatible with the various processor, using a smaller word size might not let make full use of the processor, and using a big one might create superfluous instruction. You should set the target word size by using the macro `wordsize` followed by the word size in bits in the first line of the assembly language file.

### Example
This example shows a program that adds 30 to the register R2 on a 16-bit processor in an infinite loop.
```
label start
set 0
cpy R2
label loop
set+ 30
add R2
cpy R2
setlab loop
jmp
```

