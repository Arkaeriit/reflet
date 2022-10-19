/*--------------------------------------------\
|This file contain the module making handeling|
|the interruptions.                           |
\--------------------------------------------*/

`include "reflet.vh"

module reflet_interrupt#(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input enable,
    //external interrupt signals
    input [3:0] interrupt_request, //interrupt_request[0] is the signal for the interrupt 0
    //Connection with the CPU
    input [7:0] instruction,
    input [wordsize-1:0] working_register,
    input [wordsize-1:0] program_counter,
    input [3:0] int_mask,
    output [wordsize-1:0] out,
    output [3:0] out_reg,
    output [wordsize-1:0] out_routine,
    input cpu_update,
    output interrupt, // Raised to one when entering a new interrupt routine
    output in_interrupt_context // Set to one all the time we are in interrupt context
    );
    integer i; //loop counter

    //Masking interrupts and ensuring they stay long enough to see the next
    //cpu update
    wire [3:0] int_masked = interrupt_request & int_mask;
    reg [3:0] int_masked_latched;
    always @ (posedge clk)
        if (!reset)
            int_masked_latched <= 0;
        else
            if (enable)
                for (i=0; i<4; i=i+1)
                begin
                if (cpu_update)
                    int_masked_latched[i] <= int_masked[i];
                else
                    if (int_masked[i])
                        int_masked_latched[i] <= 1;
                end

    //Instructions handeling
    reg [wordsize-1:0] prev_counter_slow; //Addr for returning from interrupts
    wire [5:0] setint_oppcode = instruction[7:2];
    wire [wordsize-1:0] out_setint = ( setint_oppcode == `opp_setint ? working_register : 0 ); //When doing a setint, we do not want to change any registers so we do the same thing as for slp
    wire [wordsize-1:0] out_retint = ( instruction == `inst_retint ? prev_counter_slow - 1 : 0 );
    assign out = out_setint | out_retint;
    assign out_reg = ( instruction == `inst_retint ? `pc_id : 0 );

    //Level of interrupts
    reg [2:0] level; //4 means normal context, otherwise, take the number of the current interrupts
    assign in_interrupt_context = !(level == 4);
    reg [2:0] prev_level_slow; //Store the previous state
    wire [2:0] target_level = ( int_masked_latched[0] ? 0 :
                                ( int_masked_latched[1] ? 1 : 
                                  ( int_masked_latched[2] ? 2 :
                                    ( int_masked_latched[3] ? 3 : 4))));
    wire new_int = target_level < level && cpu_update;
    wire quit_int = !new_int && cpu_update && instruction == `inst_retint;
    always @ (posedge clk)
        if(!reset)
            level <= 4;
        else if(enable)
        begin
            if(new_int)
                level <= target_level;
            else
                if(quit_int)
                    level <= prev_level_slow;
        end

    //Storing interruption routines' addresses
    reg [wordsize-1:0] routines [3:0];
    wire [1:0] arg = instruction[1:0];
    always @ (posedge clk)
        if(!reset)
            for(i=0; i<4; i=i+1)
                routines[i] <= 0;
        else
            if(setint_oppcode == `opp_setint)
                routines[arg] <= working_register;
    
    //Storing the program counter and the level of interrupts
    wire [wordsize-1:0] prev_counter;
    wire [2:0] prev_level; //Store the previous state
    reg [2:0] current_level_slow; //Store the current level with a small delay to put the right value in the stack
    reg [2:0] current_level_slow_slow;
    always @ (posedge clk)
    begin
        prev_counter_slow <= prev_counter;
        prev_level_slow <= prev_level;
        current_level_slow <= level;
        current_level_slow_slow <= current_level_slow;
    end
    reflet_stack #(.wordsize(wordsize), .depth(4)) stack_counter(
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .push(new_int),
        .pop(quit_int),
        .in(program_counter),
        .out(prev_counter));
    reflet_stack #(.wordsize(3), .depth(4)) stack_level(
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .push(new_int),
        .pop(quit_int),
        .in(current_level_slow_slow),
        .out(prev_level));

    //Telling the CPU about a new instruction
    assign interrupt = new_int;
    assign out_routine = routines[target_level]; 

endmodule

