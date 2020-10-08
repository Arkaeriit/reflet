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

### Labels
Labels can be used to jump to various parts of the program without having to calculate the address of the target instructions.
|Mnemonic|Followed by|Effect|
|--|--|--|
| label | A label | Note. the address of the following instruction as the label. |
| setlab | a label | Put the address of the label in the working register. |
| callf | a label | Call the function noted by the label. |
| goto | a label | Jumps to the label. |

The entry point of the program should be a point labeled `start`.

### Word size
The various macro-instructions can be compiled differently depending on the target processor word size. Even if the code could be compatible with the various processor, using a smaller word size might not let make full use of the processor, and using a big one might create superfluous instruction. You should set the target word size by using the macro `wordsize` followed by the word size in bits in the first line of the assembly language file.

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
