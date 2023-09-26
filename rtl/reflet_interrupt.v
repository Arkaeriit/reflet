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

    // Instruction decoding
    wire [5:0] oppcode = instruction[7:2];
    wire [1:0] arg = instruction[1:0];

    // Handling softint
    wire [3:0] softint_request = (oppcode == `opp_softint ?
        (4'h1 << arg) : 4'h0);

    //Masking interrupts and ensuring they stay long enough to see the next
    //cpu update
    wire [3:0] int_masked = (interrupt_request | softint_request) & int_mask;
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

    //Level of interrupts
    reg [2:0] level; //4 means normal context, otherwise, take the number of the current interrupts
    assign in_interrupt_context = !(level == 4);
    reg [2:0] prev_level_slow; //Store the previous state
    wire [2:0] target_level = ( int_masked_latched[0] ? 0 :
                                ( int_masked_latched[1] ? 1 : 
                                  ( int_masked_latched[2] ? 2 :
                                    ( int_masked_latched[3] ? 3 : 4))));
    wire new_int = target_level < level;

    // Two memories used to store the address before we went into an interrupt
    // context and the level of interrupt we were in. They are indexed by
    // interrupt context. Level 4 correspond to normal context. If we are in
    // level 2 and we get a level 0 interrupt, we will put the current address
    // at index 0 of the memory for addresses and 2 at index 0 of the memory
    // for level.
    reg [2:0] level_memory [3:0]; // TODO: as 0 will never be stored here, I can make it 1 bit thiner and assume a +1
    reg [wordsize-1:0] addr_memory [3:0];

    wire [2:0] previous_level = level_memory[level];
    wire [wordsize-1:0] previous_addr = addr_memory[level];
    
    // Interracting with the memories and current level
    always @ (posedge clk)
        if (!reset)
        begin
            level <= 4;
        end
        else if (enable)
        begin
            if (new_int & cpu_update)
            begin
                level <= target_level;
                addr_memory[target_level] <= program_counter;
                level_memory[target_level] <= level;
            end
            else if (instruction == `inst_retint)
            begin
                if (cpu_update)
                begin
                    level <= previous_level;
                end
            end
            else if (oppcode == `opp_setintstack)
            begin
                addr_memory[arg] <= working_register;
            end
        end

    wire [wordsize-1:0] addr_memory_out =
        ( instruction == `inst_retint ? previous_addr :
          ( oppcode == `opp_getintstack ? addr_memory[arg] :
            ( oppcode == `opp_setintstack ? working_register : 0 )));

    //Storing interruption routines' addresses
    reg [wordsize-1:0] routines [3:0];
    reg [wordsize-1:0] getint_out;
    always @ (posedge clk)
        if (!reset)
            getint_out <= 0;
        else if(enable)
        begin
            if(oppcode == `opp_setint && cpu_update)
            begin
                routines[arg] <= working_register;
                getint_out <= 0;
            end
            else if (oppcode == `opp_getint)
                getint_out <= routines[arg];
            else
                getint_out <= 0;
        end
    

    // Outputs to main CPU
    assign out = getint_out | addr_memory_out;
    assign out_reg = ( instruction == `inst_retint ? `pc_id : 0 );
    assign interrupt = new_int;
    assign out_routine = routines[target_level]; 

endmodule

