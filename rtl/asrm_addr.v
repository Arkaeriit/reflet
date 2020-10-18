/*--------------------------------------------\
|This module handle connection between the RAM|
|and the rest of the CPU.                     |
|Updating the stackPointer is done by the CPU.|
\--------------------------------------------*/

`include "asrm.vh"

module asrm_addr#(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input [7:0] instruction,
    input [wordsize-1:0] programCounter,
    input [wordsize-1:0] stackPointer,
    input [wordsize-1:0] otherRegister,
    output [workingRegister-1:0] addr,
    output [workingRegister-1:0] out,
    output [3:0] out_reg
    );

    wire [3:0] opperand = instruction[7:4];

    //addr selection
    wire [wordsize-1:0] addr_stack = ( instruction == inst_pop || instruction == inst_push || instruction == inst_ret || instruction == inst_call ? stackPointer : 0 );
    wire [wordsize-1:0] addr_reg = ( opperand == opp_str || opperand == opp_load ? otherRegister : 0 );
    addr = addr_stack | addr_reg;

    


