/*
|This file contain the module making handeling|
|the interruptions
*/

module asrm_interrupt#(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    //external interrupt signals
    input [3:0] ext_int, //ext_int[0] is the signal for the interrupt 0
    //Connection with the CPU
    input [7:0] instruction,
    input [wordsize-1:0] working_register,
    output [wordsize-1:0] out,
    output [3:0] out_reg,
    input ram_not_ready,
    output int
    );

    //Instructions handeling
    wire setint_opp = instruction[7:2];
    wire [wordsize-1:0] out_setint = ( setint_opp == `opp_setint ? working_register : 0 ); //When doing a setint, we do not want to change any registers so we do the same thing as for slp
    wire out_retint = ( instruction == `inst_retint ? /*TODO*/ : 0 );
    assign out = out_setint | out_retint;
    assign out_reg = ( instruction == `inst_retint ? `pc_id : 0 );
