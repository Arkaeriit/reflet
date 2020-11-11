/*--------------------------------------------\
|This file contain the module making handeling|
|the interruptions.                           |
\--------------------------------------------*/

`include "asrm.vh"

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
    input [wordsize-1:0] program_counter,
    input [3:0] int_mask,
    output [wordsize-1:0] out,
    output [3:0] out_reg,
    output [wordsize-1:0] out_routine,
    input cpu_update,
    output int
    );
    integer i; //loop counter

    //Masking interrupts
    wire [3:0] int_masked = ext_int & int_mask;

    //Instructions handeling
    reg [wordsize-1:0] prev_counter_slow; //Addr for returning from interrupts
    wire [5:0] setint_opp = instruction[7:2];
    wire [wordsize-1:0] out_setint = ( setint_opp == `opp_setint ? working_register : 0 ); //When doing a setint, we do not want to change any registers so we do the same thing as for slp
    wire [wordsize-1:0] out_retint = ( instruction == `inst_retint ? prev_counter_slow : 0 );
    assign out = out_setint | out_retint;
    assign out_reg = ( instruction == `inst_retint ? `pc_id : 0 );

    //Level of interrupts
    reg [2:0] level; //4 means normal context, otherwise, take the number of the current interrupts
    wire [2:0] prev_level; //Store the previous state
    wire [2:0] target_level = ( int_masked[0] ? 0 :
                                ( int_masked[1] ? 1 : 
                                  ( int_masked[2] ? 2 :
                                    ( int_masked[3] ? 3 : 4))));
    wire new_int = target_level < level && cpu_update;
    wire quit_int = !new_int && cpu_update && instruction == `inst_retint;
    always @ (posedge clk)
        if(!reset)
            level = 4;
        else
        begin
            if(new_int)
                level = target_level;
            else
                if(quit_int)
                    level = prev_level;
        end

    //Storing interruption routines' addresses
    reg [wordsize-1:0] routines [3:0];
    wire [1:0] arg = instruction[1:0];
    always @ (posedge clk)
        if(!reset)
            for(i=0; i<4; i=i+1)
                routines[i] = 0;
        else
            if(setint_opp == `opp_setint)
                routines[arg] = working_register;
    
    //Storing the program counter and the level of interrupts
    wire [wordsize-1:0] prev_counter;
    always @ (posedge clk)
        prev_counter_slow = prev_counter;
    asrm_stack #(.wordsize(wordsize), .depth(4)) stack_counter(
        .clk(clk),
        .reset(reset),
        .push(new_int),
        .pop(quit_int),
        .in(program_counter),
        .out(prev_counter));
    asrm_stack #(.wordsize(3), .depth(4)) stack_level(
        .clk(clk),
        .reset(reset),
        .push(new_int),
        .pop(quit_int),
        .in(level),
        .out(prev_level));

    //Telling the CPU about a new instruction
    assign int = new_int;
    assign out_routine = routines[target_level]; 

endmodule

