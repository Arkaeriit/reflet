# Reflet simulator
A simulator to test Reflet machine code. 

## Usage
```
reflet-sim [options] <binary file to execute>
Available options:
    -r/--ram-size <size>          : Size in bytes of the available RAM.
    -w/--word-size <size>         : Size in bits of the processor's words.
    -c/--config-file <file>       : Config file used to describe the VM.
    -!/--ignore-first-line <file> : Ignore chars up to the first new line in input binary.
    -x/--extended-io              : Use extended IOs instead of basic ones.
    -l/--log-file <file>          : Use the given file as a log file.

```


## IO

### Basic IOs
By default, the simulator only have basic IOs that can only write from stdin and to stdout. This works with 4 memory addresses. To print a character, write it at address `tx_data` (default to 0x1) and then write 0 at address `tx_cmd` (default to 0x0). To get an input char, write 0 at address `rx_cmd` (default to 0x2). The input char will be written at address `rx_data` (default to 0x3).

When reading a char, if the read have been successful, 1 is written back to `rx_cmd`. If nor, 2 is written to `rx_cmd`.

The addresses can be chosen with a config file if you ever feel the need to simulate a reflet-microcontroller and need the addresses at a specific place in memory.

### Extended IOs
If you want the simulator to perform more IOs such as file operation, you can enable the extended IOs. As this is no longer useful to simulate hardware such as a microcontroller, the used addresses are no longer configurable.

The address 0x0 is the char-out address, the address 0x1 is the char-in address. The address 0x2 is the command address, where you write commands to perform some IOs. The address 0x3 is the status address, where you can see the success or failure of those commands.

|Name|Command|Effect|
|-|-|-|
|write|0x01|Write the char in char-out to stdout.|
|read|0x02|Read a char from stdin to char-in. Set status to 1 if EOF is returned and to 0 otherwise.|

## Interrupts
The simulator can handle interrupts. By default, no interrupts will append, but if you can add a line `int_X_freq xxx` to the config file with X the number of the interrupt (between 0 and 3) and xxx the number instruction between each time the interrupt is raised. This will make the desired interrupts be raised at the desired frequency.

## Configuration
The configuration file should contain information about the processor such as the word size or the ram size.

|Parameter name|Value|Default Value|
|--------------|-----|-------------|
|word\_size    |The size of the processor registers in bits.| 8 |
|ram\_size     |The size of the RAM in bytes.| 255 |
|log           |A file where debug log will be stored.| disabled |
|tx\_cmd       |Address to print data | 0 |
|tx\_data      |Address to select what data to print | 1 |
|rx\_cmd       |Address used to ask for input | 2 |
|rx\_data      |Address where the input is stored | 3 |
|int\_0\_freq  |The number of instructions between each raising of the interrupt 0 | 0 (disabled) |   
|int\_1\_freq  |The number of instructions between each raising of the interrupt 1 | 0 (disabled) |   
|int\_2\_freq  |The number of instructions between each raising of the interrupt 2 | 0 (disabled) |   
|int\_3\_freq  |The number of instructions between each raising of the interrupt 3 | 0 (disabled) |   
|extended\_io  |0 to disable extended IOs, 1 to enable it | 0 (disabled) |   

You can add comments in the config file by prefixing them with a `;`.
For example, the following configuration file will help simulate an 8-bit processor with 100 bytes of RAM.
```
;8-bit words
word_size 8
;100 bytes of ram
ram_size 100
```

Some configuration items such as RAM size or word size can be given both as command line arguments. The last arguments given to when calling the simulator will prevail.

## Shebang

Thanks to the `--ignore-first-line` command line argument, you can add a shebang before a binary file to run it inside the simulator as an executable file. The shebang needs to be somewhat like `#!/usr/bin/env -S reflet-sim --word-size <wordsize> --ram-size <ramsize> --ignore-first-line`. Note the use of `env -S` to give multiple arguments to the simulator.

Here is an example of that in action:

```
> reflet-asm -word-size 16 hello_world.asm -o hello_world.bin
> echo '#!/usr/bin/env -S reflet-sim --word-size 16 --ram-size 2000 --ignore-first-line' > hello_world
> cat hello_world.bin >> hello_world
> chmod +x hello_world 
> ./hello_world 
Hello, word!
```

### Log

To ease debug, you can make the simulator generate a log file using the command line argument or a config file. This file contains a line for each instruction executed by the computer. Be careful, those file can grow really big if used on a big program.

