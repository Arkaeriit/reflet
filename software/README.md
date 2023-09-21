# Software
This directory contains various examples coded in Reflet assembly language and some libraries in Reflet assembly.

## Examples
The Makefile contains the commands needed to assemble each example with some necessary libraries and run them with `reflet-sim`. The examples are:

* exampleIO.asm: ask the user for a char and print it 10 times.
* fibonacci.asm: print the beginning of the Fibonacci sequence.
* primes.asm: print each prime number that can fit in a register of the processor.
* libtesting.asm : some code to test that the functions in the libraries are working.

## ABI
The ABI I use is very basic. The function in the libraries follow the rules:

* Function's argument are given in the registers, starting from R1 to how man arguments are needed.
* Return value is written in R1.
* Except for the return value, registers' content is preserved.
* When calling a function, you should not be in byte mode and you should not use reduced behavior bits.

## Libraries
The folder `libs` contains some libraries to help in the writing of Reflet assembly. Each library contains various functions that can affect various registers. A function preserves any register that is not used to output the result of the function except R11 and R12. The various libraries must be assembled at the same time as the code that uses them with reflet-asm.

### basicIO 
This file contains some very basic IO functions.

* printc: print the character stored in R1.
* CR: print a new line character.

### string
This library contains functions to manipulate strings.

* strlen: compute the length of a 0-terminated string ("abcd\0" would have a length of 4) with a pointer in R1 and then stores the result in R1.
* memset: fills the memory space with an address starting at a pointer stored in R1 and whose length is in R2 with the character stored in R3.
* strFlip: flips the string in R1. For example, "abc\0" would become "cba\0"

## math
This library contains functions to do various math operations that can not be done with a single instruction. The functions assume they are working with unsigned integers.

* intMult: multiplies the content of R1 and R2 and stores the result in R1.
* intDiv: divides R1 by R2, put the quotient in R1 and the rest in R2.
* intPow: puts R1 exponent R2 in R1.

## printing 
This library contains functions to print things. It depends on the libraries math, string, and basicIO.

* (print): print the string whose pointer is stored in R1 and length stored in R2.
* print: print the 0-terminated string in R1.
* num2dec: convert the number in R1 into a decimal number in the string in R2
* printNum: print the number stored in R1
* `prints <string>`: Macros that prints the string given in its argument.
* `_prints <string>`: Same as `prints` but reuse code. Needed if you want to print the same string multiple places in the code.

