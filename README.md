# REFLET
A RISC ISA. Reflet stands for **R**ISC **E**lementary **FLE**xible **T**hinking machine. To see a Reflet processor in action, see my implementation of a Reflet processor in a microcontroller here: https://github.com/Arkaeriit/Reflet-microcontroller. 

# This repository
This repository contains a simulator for a Reflet processor, an assembler to create Reflet machine code, and a Verilog implementation of a Reflet processor.

# The architecture
Reflet is a RISC ISA. Each instruction is coded on a single byte and composed of a 4 or 8-bit opcode, followed by an optional 4-bit register. This ISA can be used with a processor with words of any size superior which is 8 bits times a power of two. When the processor has a word size above 8 bits, it is little-endian.

Most operations are made between one of the 12 general-purpose registers and an implicit working register.

## Registers
A Reflet processor got 16 registers.

### The working register
R0 or WR is the working register.  This is the most important register, most operations will either copy its value into another register modify its content. Its use is never explicitly mentioned in the instructions as it is assumed that it is used most of the time. Its reset value is 0. This is very similar to the accumulator in other ISA with the exception than any register (including the working register) can be used as operand in the same way.

### General purpose registers
R1 to R12 are the 12 general-purpose registers. They are meant to store values used to interact with the working register. Their reset values are 0.

### Status register
R13 or SR is the status register. 

* The first bit is the comparison bit. It is set to 1 when a comparison instruction is realized and 0 otherwise. The comparison bit is used for conditional jumps.  
* The bits 1 to 4 are the flags to enable interrupts. Bit 1 enables interrupt 0, bit 2 enables interrupt 1, up to bit 4 which enables interrupt 3.
* When bit 5 is set to 1, interrupt 0 is raised when invalid memory access is made.
* The bits 8 to 15 are the reduced behaviors bits. When they are all set to 0, the processor behaves normally. When they are set to `b00000001`, if the word size of the processor is above 8 bits, the processor will act as a 8-bit processor when interfacing with memory. When they are set to `b00000010`, if the word size of the processor is above 16 bits, the processor will act as a 16-bit processor when interfacing with memory. To summarize, The processor will act as a `8 * (2 ^ (reduced-behavior - 1))` bits processor when interacting with memory. Those bits are not taken into account when interacting with the stack such as with `pop` and `push` instructions.

Its reset value is 0.

### Program counter
R14 or PC is the program counter. It contains the address of the current address. It can also be used to jump to a specific code address when forcefully modified by the user. Its reset value is 4.

### Stack pointer
R15 or SP is the stack pointer. It is updated when doing pop or push instructions and it points toward an address in RAM. Its reset value is 0 but it should be initialized if you intend to use the stack.

## Instructions
Here is a list of the instruction of Reflet processor. 

|Mnemonic     |Opcode   |Operand                     |Effect|
|-------------|---------|----------------------------|--|
| read        | 0x0     | A register                 | Copies the value in the argument register into the working register |
| cpy         | 0x1     | A register                 | Copies the value of the working register into the argument register |
| set         | 0x2     | A 4 bits number            | Put the number in the working register|
| add         | 0x3     | A register                 | Add the value in the working register to the value in the argument register and put the result in the working register |
| and         | 0x4     | A register                 | Do a bit-wise and between the working register and the argument register |
| or          | 0x5     | A register                 | Do a bit-wise or between the working register and the argument register |
| xor         | 0x6     | A register                 | Do a bitwise xor between the working register and the argument register |
| not         | 0x7     | A register                 | Put in the working register the bit-wise not of the argument register |
| lsl         | 0x8     | A register                 | Shit the bits in working register to the left n times, where n is the content of the argument register |
| lsr         | 0x9     | A register                 | Shit the bits in working register to the right n times, where n is the content of the argument register |
| eq          | 0xA     | A register                 | If the content of the working register is the same as the one in the argument registers, sets the comparison bit of the status register to 1. Otherwise, sets it to 0 |
| les         | 0xB     | A register                 | If the content of the working register is less than the one in the argument registers, sets the comparison bit of the status register to 1. Otherwise, sets it to 0 |
| str         | 0xC     | A register with an address | Store the value in the working register to the address given in the argument register |
| load        | 0xD     | A register with an address | Put in the working register the value at the address given in the argument register |
| jif         | 0xE0    | Nothing                    | Jump to the address in the working register if the comparison register is not equal to 0, does not affect the stack |
| call        | 0xE1    | Nothing                    | Put the current address in the stack and jump to the address in the working register. | 
| ret         | 0xE2    | Nothing                    | Jump to the address just after the address on top of the stack. |
| pop         | 0xE3    | Nothing                    | Put the content of the working register on the stack and updates the stack pointer. |
| push        | 0xE4    | Nothing                    | Put the value on top of the stack in the working register and updates the stack pointer. |
| cc2         | 0xE5    | Nothing                    | Put in the working register the opposite in two-complement of the working register. |
| cmpnot      | 0xE6    | Nothing                    | Flip the comparison bit of the status register. |
| tbm         | 0xE7    | Nothing                    | Toggle byte mode. Toggle the memory accesses from the size specified by the status register to 8 bit and back. |
| quit        | 0xE8    | Nothing                    | Reset the processor or stop it. |
| debug       | 0xE9    | Nothing                    | Does not modify any registers but the processor sends a signal to tell it is executing a debug instruction. |
| atom        | 0xEA    | Nothing                    | Set the content of the address in the working register to 1 and set the status bit to whether or not the content of this address was 0.
| retint      | 0xEB    | Nothing                    | Return from an interruption context. | 
| setint      | b111011 | A two-bit number           | Set the routine of the interruption of the given number to the address in the working register. |
| getint      | b111100 | A two-bit number           | Get the routine of the interruption of the given number and put it in the working register. |
| getintstack | b111101 | A two-bit number           | Get the address at the given place in the interrupt stack. |
| setintstack | b111110 | A two-bit number           | Get the address at the given place in the interrupt stack. |
| softint     | b111111 | A two-bit number           | Triggers the interrupt of the given number. |

There is no no-op instructions, but a few instructions have no effects such as `read WR`, `cpy WR`, or `and WR`. As `read WR` is encoded as 0x00, it makes for a fine no-op instruction.

## Connection to memory
### Word size
To be able to work, a Reflet processor needs a connection to some RAM (or RAM and ROM) where values can be stored and machine code can be read. This document describes no word size for a Reflet processor.
The Reflet processor word size should be 8 bits times a power of two. The memory should have a data bus the same size as the processor word size.
As the instruction size is always 8 bits regardless of the CPU's word size, Reflet machine code could work with Reflet processor of any word size.

### Alignment
To ease the utilization of memory, the memory access should be aligned to the size of the word. For example, when trying to manipulate 8-bit values, any address is valid. But when trying to manipulate 32-bit values, access should be aligned on 32 bits.

The access to data smaller than the word size, a module of the CPU handles this assuming that the memory output data from an address aligned to the word size. The address outputed by the CPU should be masked before addressing the memory. For example, in the case of a 16-bit CPU. 

* The CPU wants to read 8 bytes at address 0x3.
* The CPU output 0x3 as the address.
* Outside the CPU, the address is masked to 0x2 to be aligned to 16 bits.
* The memory output 0xABCD, the data at address 0x2.
* Inside of the CPU, the data is masked and shifted to be 0xAB, the data at address 0x3. 

When an invalid memory access is made (for example, trying to read 16 bits from address 0x3), it is possible to raise an interrupt signal to trap it. Be careful, when exiting this interruption, the program will jump back to the instruction that raised the interruption. You should disable traping or editing the working register in this case to prevent an infinite loop.

### Starting address
Any byte is a valid Reflet instruction. To enable a minimal value of error-correcting, the 4 first bytes of a machine code file can be reserved for the "ASRM" magic word. To allow the existence of the magic word, the program starts at the 5th byte, at address 4.

## Interruptions
A Reflet processor can react to external interruptions and do special routines. There are 4 different interruptions going from 0 (the highest priority) to 3, (the lowest priority).

To use interrupts, you must first tell the processor what is its interrupt routine. To do so, you must use the setint instruction with the number of the instruction as an argument while the address of the interruption routine is in the working register. Then, you must enable the interruption by setting to 1 the correct bit on the status register. The interruption is now set and enabled.

If the instruction is set, when the processor will receive an interrupt signal, the content of the program counter will be stored. The address of the interruption routine will be then put in the program counter. All the other registers will stay unchanged so they must be protected inside the interruption routine. To finish the interruption routine, use the retint instruction which will restore the program counter to its state before rentering the interruption. Be careful, in order not to loop back in the interruption, you must make sure that the peripheral responsible for the interruption stopped sending it.

As there are different interruptions with different levels of priority, it is possible to nest interruptions. For example, if you are in the context of interruption 2 and that interruption 1 is raised, you will shift to the context of interruption 1. When doing retint you will shift back to the context of interrupt 2. 
On the other hand, if you are in the context of interrupt 2 and interrupt 3 is raised, you will fall into the context of interrupt 3 only when you use retint from the context of interrupt 2.

Except for the program counter, the registers are not banked when entering an interrupt context. Thus some care should be given to preserve them. 

The only noticeable impact of being in interrupt context is that the byte mode is ignored. The number of byte used during memory access is influenced by the reduced behavior bits of the status register but the setup made with the `tbm` instruction are not taken into account. Once leaving the interrupt context, the byte mode is left as it was before entering the interrupt context.

