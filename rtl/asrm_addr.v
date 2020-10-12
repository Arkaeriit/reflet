/*--------------------------------------------\
|This module handle connection between the RAM|
|and the rest of the CPU.                     |
\--------------------------------------------*/

module asrm_addr#(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input [7:0] instruction
    input [wordsize-1:0] programCounter,
    input [wordsize-1:0] stackPointer,
