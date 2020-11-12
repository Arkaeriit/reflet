# Reflet soft-processor
In this folder is a Verilog implementation of a Reflet soft-processor. It is not usable as-is but I plan on using it in a soft micro-controller soon.

## Architecture detail
This processor behaves as described in the main README so if some code work with the simulator, it will work with the processor.

### Compatible RAM
A module of compatible RAM is included here, it is used in the simulation file.

### Pipeline
As of now, there is no pipeline in the processor. This leads to instruction taking between 3 and 5 clock cycles to complete.

## Content of the folder
This folder contains the 6 modules making the Reflet processor, `reflet_addr` and `reflet_addr_reduced_behavior` handle the instruction interfacing with the RAM.`reflet_stack` and `reflet_interrupt` handle the interrupts. `reflet_alu` handle other instructions and `reflet_cpu` handle the registers and combine them.  In the folder simu, you will find what I used to test this. Each test module is associated with its ROM whose content is described.

