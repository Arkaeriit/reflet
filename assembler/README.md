# Reflet assembler

An assembler to generate Reflet machine code. This assembler let you use labels, can do some basic linking, let you used predefined macros and even user-defined macros. As some of those macros do some basic data processing, they will use R11 and R12 as temporary registers.


## Usage

```
reflet-asm <input file> <options> -o <output file>
reflet-asm -help
```
The possible options are the following:
* -no-prefix: does not put the 'ASRM' prefix at the beginning of the output file.
* -no-stack-init: does not initialize the stack pointer. When not used, the stack pointer is by default initialized to just after the program.
* -set-stack-to xxx: set the stack pointer to the address given just after the flag. Incompatible with -no-stack-init.
* -start-addr xxx: tell the linker that the code should start at the given address. 
* -ignore-start: if set, the program will not start at the "start" label but at the beginning of the first input file.
* -wordsize xxx : set the wordsize to xxx (a number in bits).
* -label-dump: if set, a dump of all labels will be printed just before linkage.

## Assembly language file

Each line can contain a line with an instruction (`add R3`), a line with no code (just white-space or a comment), an user-defined macro or an assembler directive. The various fields in an instruction or macro should not be separated by commas.

### Comments

Comments start with a semi-colon and end at the end of the line. They are ignored by the assembler.

### Instructions

Instruction start with the mnemonic for the operation and then, if the instruction needs it, an argument that can be a register, a two bit number or a four bit number. Instruction are case-insensitive.

### User-defined macros

Macros are defined in the following way:
```
@define <macro_name> <number of args>
    <content>
    <content>
@end
```

In the macro's content, arguments are accessed by calling `$<argument_number>`

In assembly, macros are called by doing `<macro_name> <arg1> <arg2> ...`

Note: The first argument is accessed with $1.

### Assembler directives

Beyond the `@define` directive to declare macros, the assembler offers other directives.

* `@label <name>` defines a position in the code.
* `@labref <name>` write in the code the address of the label with the given name.
* `@align <number>` align the next instruction to the number given in bytes.
* `@constant <number>` put the value of the number (written in decimal) in the machine code.
* `@rawbytes <byte 1> <byte 2> ... <byte N>` writes the given bytes in the machine code.
* `@string ...` writes the sting following the directive (and after a complementary space) in the machine code. Comments are not supported and will be considered as part of the string.
* `@import <path>` include in the assembly the content of the given file. The path is relative to the path of the file where the import directive is.
* `rawbyte <byte>` writes a single byte of data in the machine code.

### Predefined macro

To ease the use of the assembler, some macros are predefines.

* `set+ <number>` puts the value of the number (written in decimal) in the working register.
* `mov <reg 1> <reg 2>` copies the value of `reg 2` into `reg 1`.
* `setr <reg> <number>` sets the value of the register to the given number.
* `pushr <reg>` pushes the value of the given register in the stack.
* `popr <reg>` pops the value on top of the stack in the given register.
* `label <name>` is an alias to `@label <name>`.
* `setlab <name>` sets the working register to the address of the given label.
* `callf <name>` calls the function at the given label.
* `goto <name>` jumps to the given label.
* `jmp` jumps to the address in the working register.

Most of those macro will overwrite the register R12, only setlab and set+ will overwrite R11. If you use a macro to edit the PC, it might not work and will generate unexpected behaviors.

## Compilation and installation

Driver Helper is written in [Nelua](https://nelua.io). But as everyone might not have the Nelua compiler install, I included the generated C file in this repository.

Running `make` will compile the C file. To compile from the nelua file, run `make full_compilation`.

You can then install it with `make install` or uninstall it with `make uninstall`.

## Legacy assemblers

Before writting this assembler, I had written two other that can be found in the `legacy_assemblers` folder. Note that even if they can write correct code, `reflet-masm` is not suitable to write code that is complient with alignement constrain.

