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
    input [wordsize-1:0] programCounter,
    input [wordsize-1:0] stackPointer,
    input [wordsize-1:0] otherRegister,
    output reg [7:0] instruction,
    output [workingRegister-1:0] addr,
    output [workingRegister-1:0] out,
    output [3:0] out_reg
    );

    wire [3:0] opperand = instruction[7:4];
    reg [3:0] not_ready = 1;


    //addr selection
    wire [wordsize-1:0] addr_stack = ( instruction == inst_pop || instruction == inst_push || instruction == inst_ret || instruction == inst_call ? stackPointer : 0 );
    wire [wordsize-1:0] addr_reg = ( opperand == opp_str || opperand == opp_load ? otherRegister : 0 );
    wire [wordsize-1:0] addr_inst = (!not_ready ? programCounter : 0);
    addr = addr_stack | addr_reg | addr_inst;

    


