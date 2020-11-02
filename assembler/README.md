# ASRM assembler

An assembler to create ASRM machine code.

This folder contains two programs, **asrmpasm** which is a very bare-bone assembler written in C, and **asrmmasm** which is a richer assembler written mostly in Lua.

Both assemblers are case-insensitive for mnemonic but case-sensitive for anything else.

## ASRMPASM
This is a pico-assembler. It can only assemble basic instruction as described in the main README.md. It would be very tedious to use it but you have total control over what instructions will be created.

If you want to insert a byte into your program without having to write the instruction for it, you can write `rawbyte` followed by a single byte in base 10.

## ASRMMASM
This is a macro-assembler. Not only can it assemble basic instructions but it can also assemble macro-instruction and manage labels. When using macro-instructions, the registers R11 and R12 might be used as temporary registers and overwritten. Furthermore, unless specified otherwise, the working registry will be modified.

To run it, the pico-assembler must be installed. 

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

