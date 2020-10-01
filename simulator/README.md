# ASRM simulator
A simulator to test ASRM machine code. 

## Usage
The simulator should be given two arguments to start: a configuration file and a binary file. The binary file should contain proper ASRM machine code.

## Configuration
The configuration file should contain information about the processor such as the word size or the ram size.
|Parameter name|value|
|--------------|-----|
|word\_size     |The size of the processor registers in bits.|
|ram\_size      |The size of the RAM in bytes.|
You can add comments in the config file by prefixing them with a `;`.
For example, the following configuration file will help simulate an 8-bit processor with 100 bytes of RAM.
```
;8-bit words
word_size 8
;100 bytes of ram
ram_size 100
```

