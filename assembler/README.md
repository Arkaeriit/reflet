# Reflet FPU ASM

The Reflet FPU processor needs machine code to run. This Reflet FPU ASM let you assemble assembly language into usable machine code. 

## Usage

```
reflet-fpu-asm <input-file> <options> -o <output-file>
reflet-fpu-asm -help
```

Options:
* -float-size 16/32/64: indicate the size of floating points numbers used.

## Assembly language file

Each line can contain a line with an instruction (`add R1 R2 R3`), a line with no code (just white-space or a comment), an user-defined macro or an assembler directive.

### Comments

Comments start with a semi-colon and end at the end of the line. They are ignored by the assembler.

### Instructions

The instruction start with the mnemonic for the instruction and are then followed by the registers (R1, R2, ... R7) or instruction flags.

The mnemonic, the order of the arguments and the instruction flags are coherent with the root `README.md`.

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
* `@constant <float>` TODO
* `@rawbytes <byte 1> <byte 2> ... <byte N>` write the given bytes in the machine code.

## Compilation and installation

Driver Helper is written in [Nelua](https://nelua.io). But as everyone might not have the Nelua compiler install, I included the generated C file in this repository.

Running `make` will compile the C file. To compile from the nelua file, run `make full_compilation`.

You can then install it with `make install` or uninstall it with `make uninstall`.

