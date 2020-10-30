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
    input [wordsize-1:0] statusRegister,
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

    //Communication with the CPU
    wire [3:0] opperand = instruction[7:4];
    reg [3:0] not_ready;
    reg hide_ready; //This is used to hide the fact that not_ready is set to 0 when we need to use the ram
    wire using_ram = instruction == `inst_pop || instruction == `inst_push || instruction == `inst_call || instruction == `inst_ret || opperand == `opp_str || opperand == `opp_load;
    assign ram_not_ready = |not_ready | (!hide_ready & using_ram);
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
    wire [1:0] reduced_behavior_bits = statusRegister[2:1];
    wire reduced_behavior = (reduced_behavior_bits != 2'b00) && ( (reduced_behavior_bits == 2'b01 && wordsize > 32) || (reduced_behavior_bits == 2'b10 && wordsize > 16) || (reduced_behavior_bits == 2'b11 && wordsize > 8) );
    reg [127:0] data_buff; //Register to store the value we fetched
    wire [127:0] data_in_wide = data_in;
    wire returning_value = instruction == `inst_pop || instruction == `inst_ret || opperand == `opp_load;
    wire [wordsize-1:0] data_out_out = ( returning_value ? ( instruction == `inst_ret ? data_buff[wordsize-1:0] + 1 : data_buff[wordsize-1:0]) : 0 ); //When we want to use walue read from ram. Note, when returning from a function, we need to go after what we put in the stack in order not to be trapped in an infinite loop
    wire [wordsize-1:0] wr_out = ( instruction == `inst_push || instruction == `inst_call || opperand == `opp_str ? workingRegister : 0 ); //when we don't need to update any register we will simply put the content of the working register into itself
    assign out = wr_out | data_out_out;
    always @ (posedge clk)
        if(!reset)
            data_buff = 0;
        else
            if(returning_value & ! fetching_instruction)
            begin
                if(reduced_behavior)
                    case(reduced_behavior_bits)
                        2'b01 : 
                        begin
                            data_buff[31:0] = data_in_wide[31:0];
                            data_buff[127:32] = 0;
                        end
                        2'b01 : 
                        begin
                            data_buff[15:0] = data_in_wide[15:0];
                            data_buff[127:16] = 0;
                        end
                        2'b01 : 
                        begin
                            data_buff[7:0] = data_in_wide[7:0];
                            data_buff[127:8] = 0;
                        end
                    endcase
                else //normal behavior
                    data_buff[wordsize-1:0] = data_in_wide;
            end

    //register to update
    assign out_reg = ( instruction == `inst_ret || instruction == `inst_call ? `pc_id : 0 ); 

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
                        if(hide_ready) //About to fetch an instruction
                            not_ready = 2;
                        else //Interfacing whith RAM
                            not_ready = 2;
                        hide_ready = !hide_ready;
                    end
                    else
                        not_ready = 2;
                end
                1 : 
                begin 
                    if(!hide_ready)
                        instruction = data_in[7:0];
                    not_ready = not_ready - 1;
                end
                default : not_ready = not_ready - 1;
            endcase
        end

endmodule
    
