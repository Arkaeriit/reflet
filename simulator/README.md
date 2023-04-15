# Reflet simulator
A simulator to test Reflet machine code. 

## Usage
```
reflet-sim [options] <binary file to execute>
Available options:
    -r/--ram-size <size>    : Size in bytes of the available RAM.
    -w/--word-size <size>   : Size in bits of the processor's words.
    -c/--config-file <file> : Config file used to describe the VM.
```


## IO
To test your programs, the simulator can do basic IO. To print a character, write it at address `tx_data` (default to 0x1) and then write 0 at address `tx_cmd` (default to 0x0). To get an input char, write 0 at address `rx_cmd` (default to 0x2). The input char will be written at address `rx_data` (default to 0x3).

## Interrupts
The simulator can handle interrupts. By default, no interrupts will append, but if you can add a line `int_X_freq xxx` to the config file with X the number of the interrupt (between 0 and 3) and xxx the number instruction between each time the interrupt is raised. This will make the desired interrupts be raised at the desired frequency.

## Configuration
The configuration file should contain information about the processor such as the word size or the ram size.

|Parameter name|Value|Default Value|
|--------------|-----|--|
|word\_size     |The size of the processor registers in bits.| 8 |
|ram\_size      |The size of the RAM in bytes.| 255 |
|log            |A file where debug log will be stored.| disabled |
|tx\_cmd         |Address to print data | 0 |
|tx\_data        |Address to select what data to print | 1 |
|rx\_cmd         |Address used to ask for input | 2 |
|rx\_data        |Address where the input is stored | 3 |
|int\_0\_freq    |The number of instructions between each raising of the interrupt 0 | 0 (disabled) |   
|int\_1\_freq    |The number of instructions between each raising of the interrupt 1 | 0 (disabled) |   
|int\_2\_freq    |The number of instructions between each raising of the interrupt 2 | 0 (disabled) |   
|int\_3\_freq    |The number of instructions between each raising of the interrupt 3 | 0 (disabled) |   

You can add comments in the config file by prefixing them with a `;`.
For example, the following configuration file will help simulate an 8-bit processor with 100 bytes of RAM.
```
;8-bit words
word_size 8
;100 bytes of ram
ram_size 100
```

Some configuration items such as RAM size or word size can be given both as command line arguments. The last arguments given to when calling the simulator will prevail.

