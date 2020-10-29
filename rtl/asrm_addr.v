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
    output reg [7:0] instruction,
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
    reg [3:0] not_ready;
    reg hide_ready; //This is used to hide the fact that not_ready is set to 0 when we need to use the ram
    assign ram_not_ready = |not_ready | (!hide_ready & using_ram);
    wire using_ram = instruction == `inst_pop || instruction == `inst_push || instruction == `inst_call || instruction == `inst_ret || opperand == `opp_str || opperand == `opp_load;
    wire fetching_instruction = (not_ready == 2 || not_ready == 1 || not_ready == 0) & !hide_ready;


    //addr selection
    wire [wordsize-1:0] addr_pop = ( instruction == `inst_pop || instruction == `inst_ret ? stackPointer - 1 : 0 ); //We need the -1 because the CPU updated the stack pointer
    wire [wordsize-1:0] addr_push = ( instruction == `inst_push || instruction == `inst_call ? stackPointer : 0 );
    wire [wordsize-1:0] addr_reg = ( opperand == `opp_str || opperand == `opp_load ? otherRegister : 0 );
    assign addr = ( !fetching_instruction ? addr_reg | addr_pop | addr_push : programCounter); //The defaut behaviour is to seek the address of the next piece of code

    //selecting the data to send
    wire [wordsize-1:0] data_wr = ( instruction == `inst_push || opperand == `opp_str ? workingRegister : 0 );
    wire [wordsize-1:0] data_pc = ( instruction == `inst_call ? programCounter : 0 );
    assign data_out = data_wr | data_pc;

    //data to send to the cpu
    reg [wordsize-1:0] data_buff; //Register to store the value we fetched
    wire returning_value = instruction == `inst_pop || instruction == `inst_ret || opperand == `opp_load;
    wire [wordsize-1:0] data_out_out = ( returning_value ? data_buff : 0 ); //When we want to use walue read from ram
    wire [wordsize-1:0] wr_out = ( instruction == `inst_push || instruction == `inst_call || opperand == `opp_str ? workingRegister : 0 ); //when we don't need to update any register we will simply put the content of the working register into itself
    assign out = wr_out | data_out_out;
    always @ (posedge clk)
        if(!reset)
            data_buff = 0;
        else
            if(returning_value & ! fetching_instruction)
                data_buff = data_in;

    //register to update
    assign out_reg = 0; //All the instructions handeled by this module are 

    //write enable
    assign write_en = (instruction == `inst_push || instruction == `inst_call || opperand == `opp_str) & !fetching_instruction;

    //Changing the not_ready register to let the CPU<->RAM commuticatio to occur
    always @ (posedge clk)
        if(!reset)
        begin
            not_ready = 2;
            instruction = 0;
            hide_ready = 0;
        end
        else
        begin
            case(not_ready)
                0 : //we are ready and thus must addapt the time of not ready to engage the communication with the ram
                begin
                    if(using_ram)
                    begin
                        not_ready = 5;
                        hide_ready = !hide_ready;
                    end
                    else
                        not_ready = 2;
                end
                1 : 
                begin 
                    if(!hide_ready)
                        instruction = data_in;
                    not_ready = not_ready - 1;
                end
                default : not_ready = not_ready - 1;
            endcase
        end

endmodule
    
