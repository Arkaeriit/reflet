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
* -word-size xxx : set the wordsize to xxx (a number in bits).
* -label-dump: if set, a dump of all labels will be printed just before linkage.
* -D name=definition: defines the macro `name` with te=he current definition. It will be a flat define macro.

## Assembly language file

Each line can contain a line with an instruction (`add R3`), a line with no code (just white-space or a comment), an user-defined macro or an assembler directive. The various fields in an instruction or macro should not be separated by commas.

### Comments

Comments start with a semi-colon and end at the end of the line. They are ignored by the assembler.

### Instructions

Instruction start with the mnemonic for the operation and then, if the instruction needs it, an argument that can be a register, a two bit number or a four bit number. Instruction are case-insensitive.

### User-defined macros

Macros are defined in the following way:
```
@macro <macro_name> <number of args>
    <content>
    <content>
@end
```

In the macro's content, arguments are accessed by calling `$<argument_number>`

In assembly, macros are called by doing `<macro_name> <arg1> <arg2> ...`

Note: The first argument is accessed with $1.

### Flat defines

User-defined macros are only expanded if they are the first element in a line. If you want to have a word be a synonym for any placed words, you can use flat defined as follow:

```
@define two 2
@define r4_two R4 two

set two ; Will be expanded to `set 2`
setr r4_five ; Will be expanded st `setr R4 2`
```

If you want to do some compile-time computation in your flat defines, you can use the `@define-math` directive as follow:

```
@define-math four 3 + 1
@define-math height four * 2

set height ; Will be expanded to `set 8`
```

You can use the value of words defined in `@define-math` in other `@define-math`, but not the ones defined in `@define`. `@define-math` uses [Math-Parse](https://github.com/Arkaeriit/math-parse) under the hood, you can use all it's integer functions.

### Sections

By default, the content in the generated binary file follows the order of the content in the source files. But you might want to have more flexibility in where to place code or data. You can do so with the `@section <section name>` directive to declare the position of a section. Then, you can put content at that section by placing it between the directives `@in-section <section name>` and `@end-section`.

Nesting section is not recommended and might result in unexpected behaviors. 

### Assembler directives

Beyond the directives to declare macros and sections, the assembler offers other directives.

* `@label <name>` defines a position in the code.
* `@labref <name>` write in the code the address of the label with the given name.
* `@align <number>` align the next instruction to the number given in bytes.
* `@pad-until <address>` Add padding until the given address is reached.
* `@constant <number>` put the value of the number in the machine code.
* `@rawbytes <byte 1> <byte 2> ... <byte N>` writes the given bytes (in hexadecimal) in the machine code.
* `@string "..."` writes the strings in the quotes following the directive in the machine code.
* `@import <path>` include in the assembly the content of the given file. The path is relative to the path of the file where the import directive is.
* `@set8 <number>` sets the 8 bit number into the WR.
* `@set_sr_for <size in bits>` sets the SR to a value suitable to move the given size.
* `@get_sr_for <size in bits>` sets the WR to a value suitable for the SR to move the given size.
* `@set_default_sr` sets the SR to a value suitable to move the wordsize of the processor.

As alternatives to `@string`, there is also `@string-nl`, `@string-0`, and `@string-nl-0` which add respectively a new line, a null byte, and a new line followed by a null byte after the string.

### Predefined macro

To ease the use of the assembler, some macros are predefined.

* `set+ <number>` puts the value of the number in the working register.
* `mov <reg 1> <reg 2>` copies the value of `reg 2` into `reg 1`.
* `setr <reg> <number>` sets the value of the register to the given number. Do not preserve the working register.
* `pushr <reg>` pushes the value of the given register in the stack.
* `popr <reg>` pops the value on top of the stack in the given register.
* `label <name>` is an alias to `@label <name>`.
* `set8 <number>` is an alias to `@set8 <number>`.
* `setlab <name>` sets the working register to the address of the given label.
* `callf <name>` calls the function at the given label.
* `goto <name>` jumps to the given label.
* `jmp` jumps to the address in the working register.
* `jifl <name>` jump  to the given label if the comparison bit is set to 1.
* `@set_wordsize_byte` sets the number of bytes in a word in the working register.
* `@align_word` align to the size of the word.
* `load<bit number> <reg>` calls `load <reg>` after setting the status register to only interact with the given numbers of bits.
* `str<bit number> <reg>` calls `str <reg>` after setting the status register to only interact with the given numbers of bits.
* `rawbyte <byte>` writes a single byte (in hexadecimal) of data in the machine code.
* `addup <reg>` increase the target register with the value in the working register.
* `inc <reg>` increase by one the value in the target register.
* `inc_ws <reg>` increase by the word size in bytes the value in the target register.
* `subto <reg>` put in the working register `<reg> - wr`.
* `sub <reg>` put in the working register `wr - <reg>`.
* `nop` and `slp` both do nothing.
* `set_wordsize_bit` set the number of bits in a word into the working register.
* `leseq <reg>` set the comparison bit if the working register is smaller or equal to the target register.
* `great <reg>` set the comparison bit if the working register is greater than the target register.
* `greateq <reg>` set the comparison bit if the working register is greater or equal to the target register.
* `neq <reg>` set the comparison bit if the working register is different from the target register.

Most of those macro will overwrite the register R12, only `load<X>`, `str<X>`, `setlab`, `setr`, and `set+` will overwrite R11. If you use a macro to edit the PC, it might not work and will generate unexpected behaviors.

General register manipulation macros (`mov`, `pushr`, `popr`, `addup`, `inc`, and `inc_ws`) all preserve the value of the working resister by saving it to R12. If you don't want to do so for performance reason, you can use the macros suffixed with a dot (`mov.`, `pushr.`, ...). Note that `setr` never preserves the working register.

`load<X>` and `str<X>` macros are available to work on 8, 16, or 32 bits (`load8`, `load16`, ...). They reset the status register after the store/load. If you want swifter operations on 8 bits, you can use `load8.` and `str8.` with use the `tbm` instruction. They are faster and don't use R11 or R12, but the use of `tbm` meas you can't use them in interrupt context.

## Numbers

Unless specified otherwise (like in  `@rawbytes` or `rawbyte`), number are considered to be in hexadecimal when prefixed with `0x` and in decimal otherwise.

## Registers

Registers name are case insensitive. They can either be named (`SP`, `wr`) or be written with 'R' followed by their numbers in decimal or hexadecimal (`r2`, `R0`, `r11`, `RA`). Leading zeros in registers number are not supported.

## Compilation and installation

The Reflet assembler is written in Rust.

Running `make` will compile it with cargo.

You can then install to `/usr/local/bin` it with `make install` or uninstall it with `make uninstall`.

## Legacy assemblers

Before writing this assembler, I had written two other that can be found in the `legacy_assemblers` folder. They were written for a previous version of the ISA so they will not write correct code.

