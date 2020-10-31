# ASRM simulator
A simulator to test ASRM machine code. 

## Usage
The simulator should be given two arguments to start: a configuration file and a binary file. The binary file should contain proper ASRM machine code.

## IO
To test your programs, the simulator can do basic IO. To print a character, write it at address `tx_data` (default to 0x1) and then write 0 at address `tx_cmd` (default to 0x0). To get an input char, write 0 at address `rx_cmd` (default to 0x2). The input char will be written at address `rx_data` (default to 0x3).

## Configuration
The configuration file should contain information about the processor such as the word size or the ram size.
|Parameter name|Value|Default Value|
|--------------|-----|--|
|word\_size     |The size of the processor registers in bits.| 8 |
|ram\_size      |The size of the RAM in bytes.| 255 |
|log            |A file where debug log will we stored.| disabled |
|tx_cmd         |Address to print data | 0 |
|tx_data        |Address to select what data to print | 1 |
|rx_cmd         |Address used to ask for input | 2 |
|rx_data        |Address where the input is stored | 3 |

You can add comments in the config file by prefixing them with a `;`.
For example, the following configuration file will help simulate an 8-bit processor with 100 bytes of RAM.
```
;8-bit words
word_size 8
;100 bytes of ram
ram_size 100
```

