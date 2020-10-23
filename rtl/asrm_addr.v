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
    //instructions from the CPU
    input [wordsize-1:0] workingRegister,
    input [wordsize-1:0] programCounter,
    input [wordsize-1:0] stackPointer,
    input [wordsize-1:0] otherRegister,
    output reg [7:0] instruction = 0,
    //ram connection
    output [wordsize-1:0] addr,
    output [wordsize-1:0] data_out,
    input [wordsize-1:0] data_in,
    output write_en,
    //output to the CPU
    output [wordsize-1:0] out,
    output [3:0] out_reg,
    output ram_not_ready
    );

    wire [3:0] opperand = instruction[7:4];
    reg [3:0] not_ready = 1;
    assign ram_not_ready = |not_ready;


    //addr selection
    wire [wordsize-1:0] addr_stack = ( instruction == `inst_pop || instruction == `inst_push || instruction == `inst_ret || instruction == `inst_call ? stackPointer : 0 );
    wire [wordsize-1:0] addr_reg = ( opperand == `opp_str || opperand == `opp_load ? otherRegister : 0 );
    assign addr = ( not_ready ? addr_reg | addr_stack : programCounter); //The defaut behaviour is to seek the address of the next piece of code

    //selecting the data to send
    wire [wordsize-1:0] data_wr = ( instruction == `inst_push || opperand == `opp_str ? workingRegister : 0 );
    wire [wordsize-1:0] data_pc = ( instruction == `inst_call ? programCounter : 0 );
    assign data_out = data_wr | data_pc;

    //data to send to the cpu
    wire [wordsize-1:0] data_out_out = ( instruction == `inst_pop || instruction == `inst_ret || opperand == `opp_load ? data_in : 0 ); //When we want to use walue read from ram
    wire [wordsize-1:0] wr_out = ( instruction == `inst_push || instruction == `inst_call || opperand == `opp_str ? workingRegister : 0 ); //when we don't need to update any register we will simply put the content of the working register into itself
    assign out = wr_out | data_out_out;

    //register to update
    assign out_reg = 0; //All the instructions handeled by this module are 

    //write enable
    assign write_en = instruction == `inst_push || instruction == `inst_call || opperand == `opp_str;

    //Changing the not_ready register to let the CPU<->RAM commuticatio to occur
    always @ (posedge clk)
    begin
        if(!not_ready & reset) //we are ready and thus must addapt the time of not ready to engage the communication with the ram
        begin
            if(instruction == `inst_pop || instruction == `inst_push || instruction == `inst_ret || `inst_call || opperand == `opp_str || opperand == `opp_load)
                not_ready = 1;
        end
        if(not_ready & reset)
            not_ready = not_ready - 1;
    end

    //updating the instruction or the working dirrectory
    always @ (posedge clk)
        if(!reset)
        begin
            not_ready = 1;
            instruction = 0;
        end
        else
        begin
            if(not_ready) //ram-related instruction
            begin
            end
            else //fetchin new oppetations
                instruction = data_in;
        end

endmodule
    
