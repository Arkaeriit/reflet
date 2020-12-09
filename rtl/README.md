# Reflet soft-processor
In this folder is a Verilog implementation of a Reflet soft-processor. It is not usable as-is but I plan on using it in a soft micro-controller soon.

## Architecture detail
This processor behaves as described in the main README so if some code work with the simulator, it will work with the processor.

### Pipeline
As of now, there is no pipeline in the processor. This leads to instruction taking between 3 and 5 clock cycles to complete.

## Top-level module
The top-level module is `reflet_cpu`.  Its only parameter is `wordsize` which defines the wordsize in bits.
Here is a list of all its connections:
|Name |Type   |Description|
|------|------|-------|
|clk     |input|The main clock.|
|reset |input|Reset the CPU when flipped to low.|
|debug|output|Turned on for a clock cycle when the CPU executes a `debug` instruction.|
|quit|output|Is turned on from the moment the CPU executes a quit instruction and stops itself.|
|ext\_int| 4-bit input | The 4 instructions lines.|
|addr| wordsize-bit output|Select the RAM address to interact with.|
|data\_out|wordsize-bit output|Data to write to the RAM.|
|data\_in|wordsize-bit input|Data coming from the RAM.|
|write\_en|output|Is turned to 1 when needing to write to the RAM.|

## Content of the folder
This folder contains the 6 modules making the Reflet processor, `reflet_addr` and `reflet_addr_reduced_behavior` handle the instruction interfacing with the RAM.`reflet_stack` and `reflet_interrupt` handle the interrupts. `reflet_alu` handle other instructions and `reflet_cpu` handle the registers and combine them.  In the folder simu, you will find what I used to test this. Each test module is associated with its ROM whose content is described.

