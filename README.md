# REFLET
A RISC ISA. Reflet stands for **R**ISC **E**lementary **FLE**xible **T**inking machine.

# This repository
This repository contains a simulator for a Reflet processor, an assembler to create Reflet machine code, and a Verilog implementation of a Reflet processor.

# The architecture
Reflet is a RISC ISA. Each instruction is coded on a single byte and composed of a 4 or 5-bit operand, followed by an optional 4-bit register. This ISA can be used with a processor with words of any size superior which is 8 bits times a power of two. When the processor has a word size above 8 bits, it is little-endian.

Most operations are made between one of the 12 general-purpose registers and an implicit working register.

## Registers
A Reflet processor got 16 registers.
### The working register
R0 or WR is the working register.  This is the most important register, most operations will either copy its value into another register modify its content. Its use is never explicitly mentioned in the instructions as it is assumed that it is used most of the time. Its reset value is 0.
### General purpose registers
R1 to r12 are the 12 general-purpose registers. They are meant to store values used to interact with the working register. Their reset values are 0.
### Status register
R13 or SR is the status register. 
* The first bit is the comparison bit. It is set to 1 when a comparison instruction is realized and 0 otherwise. The comparison bit is used for conditional jumps.  
* The bits 2 and 1 are the reduced behaviors bits. When they are both set to 0, the processor behaves normally. When they are set to `b01`, if the word size of the processor is above 32 bits, the processor will act as a 32-bit processor when interfacing with memory. When they are set to `b10`, if the word size of the processor is above 16 bits, the processor will act as a 16-bit processor when interfacing with memory. When they are set to `b11`, the processor will act as an 8-bit processor when interfacing with memory.
* The bits 3 to 6 are the flags to enable interrupts. Bit 3 enables interrupt 0, the bit 4 enables interrupt 1 up to bit 6 which enable interrupt 4.

Its reset value is  0x1.

### Program counter
R14 or PC is the program counter. It contains the address of the current address. It can also be used to jump to a specific code address when forcefully modified by the user. Its reset value is 4.
### Stack pointer
R15 or SP is the stack pointer. It is updated when doing pop or push instructions and it points toward an address in RAM. Its reset value is 0 but it should be initialized if you intend to use the stack.

## Instructions
Here is a list of the instruction of Reflet processor. 
|Mnemonic|Operand|Followed by|Effect|
|--|--|--|--|
| slp | 0x00 | Nothing | Does nothing, wait for the next instruction |
| set | 0x1 | A 4 bits number | Put the number in the working register|
| read | 0x2 | A register | Copies the value in the argument register into the working register |
| cpy | 0x3 | A register | Copies the value of the working register into the argument register |
| add | 0x4 | A register | Add the value in the working directory to the value in the argument register and put the result in the working register |
| sub | 0x5 | A register | Subtract to the value in the working directory the value in the argument register and put the result in the working register |
| and | 0x6 | A register | Do a bit-wise and between the working register and the argument register |
| or | 0x7 | A register | Do a bit-wise or between the working register and the argument register |
| xor | 0x8 | A register | Do a bitwise xor between the working register and the argument register |
| not | 0x9 | A register | Put in the working register the bit-wise not of the argument register |
| lsl | 0xA | A register | Shit the bits in working register to the left n times, where n is the content of the argument register |
| lsr | 0xB | A register | Shit the bits in working register to the right n times, where n is the content of the argument register |
| eq | 0xC | A register | If the content of the working register is the same as the one in the argument registers, set all the bits of the comparison register to one. Otherwise, they are set to 0 |
| les | 0xD | A register | If the content of the working register is less than the one in the argument registers, set all the bits of the comparison register to one. Otherwise, they are set to 0 |
| str | 0xE | A register with an address | Store the value in the working register to the address given in the argument register |
| load | 0xF | A register with an address | Put in the working register the value at the address given in the argument register |
| jmp | 0x08 | Nothing | Jump to the address in the working register, does not affect the stack. |
| jif | 0x09 | Nothing | Jump to the address in the working register if the comparison register is not equal to 0, does not affect the stack |
| pop | 0x0A | Nothing | Put the content of the working register on the stack and updates the stack pointer. |
| push | 0x0B | Nothing | Put the value on top of the stack in the working register and updates the stack pointer. |
| call | 0xC | Nothing | Put the current address in the stack and jump to the address in the working register. | 
| ret | 0x0D | Nothing | Jump to the address just after the address on top of the stack. |
| quit | 0x0E | Nothing | Reset the processor or stop it. |
| debug | 0x0F | Nothing | Does not modify any registers but the processor sends a signal to tell it is executing a debug instruction. |
| cmpnot | 0x01 | Nothing | Flip the comparison bit of the status register. |
| retint | 0X02 | Nothing | Return from an interruption context. | 
| setint | b000001 | A two-bit number | Set the routine of the interruption of the given number to the address in the working register. |
| *reserved* | 0x03 | Nothing | Reserved. |

## Connection to memory
### Word size
To be able to work, a Reflet processor needs a connection to some RAM (or RAM and ROM) where values can be stored and machine code can be read. As this document describes no word size for a Reflet processor.
The Reflet processor word size should be 8 bits times a power of two. The memory should have a data bus the same size as the processor word size. Each byte of the RAM should be addressable to fetch individual instructions in each byte. 
Except in the case of overflows, Reflet machine code should work with Reflet processor of various word sizes.


### Starting address
Any byte is a valid Reflet instruction. To enable a minimal value of error-correcting, the 4 first byte of a machine code file can be reserved for the "Reflet" magic word. To enable the existence of the magic word, the program starts at the 5th byte, at address 4.

## Interruptions
A Reflet processor can react to external interruption and do special routines. There are 4 different interruptions going from 0 (the highest priority to 3, the lowest priority).

To use interrupts, you must first tell the processor what is its interrupt routine. To do so, you must use the setint instruction with the number of the instruction as an argument while the address of the interruption routine is in the working register. Then, you must enable the interruption by setting to 1 the correct bit on the status register. The interruption is now set and enabled.

If the instruction is set, when the processor will receive an interrupt signal, the content of the program counter will be stored. The address of the interruption routine will be then put in the program counter. All the other registers will stay at-is so they must be protected inside the interruption routine. To finish the interruption routine, use the retint instruction which will restore the program counter to its state before rentering the interruption. Be careful, in order not to loop back in the interruption, you must make sure that the peripheral responsible for the interruption stopped sending it.

As there are different interruptions with different levels of priority, it is possible to nest interruptions. For example, if you are in the context of interruption 2 and that interruption 1 is raised, you will shift to the context of interruption 1. When doing retint you will shift back to the context of interrupt 2. 
On the other hand, if you are in the context of interrupt 2 and interrupt 3 is raised, you will fall into the context of interruption 3 only when you use retint from the context of interrupt 2.

