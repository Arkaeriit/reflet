/*--------------------------------------------\
|This module handle connection between the RAM|
|and the rest of the CPU.                     |
|Updating the stackPointer is done by the CPU.|
\--------------------------------------------*/

`include "reflet.vh"

module reflet_addr #(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input enable,
    //instructions from the CPU
    input [wordsize-1:0] workingRegister,
    input [wordsize-1:0] programCounter,
    input [wordsize-1:0] stackPointer,
    input [wordsize-1:0] otherRegister,
    input [1:0] reduced_behaviour_bits,
    output reg [7:0] instruction,
    output alignement_error,
    //ram connection
    output [wordsize-1:0] addr,
    output [wordsize-1:0] data_out,
    input [wordsize-1:0] data_in,
    output write_en,
    //output to the CPU
    output byte_mode,
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
    wire alignement_fixer_ready;


    //addr selection
    wire [5:0] pop_offset;
    wire [wordsize-1:0] addr_pop = ( instruction == `inst_pop || instruction == `inst_ret ? stackPointer - pop_offset : 0 ); //We need the -1 because the CPU updated the stack pointer
    wire [wordsize-1:0] addr_push = ( instruction == `inst_push || instruction == `inst_call ? stackPointer : 0 );
    wire [wordsize-1:0] addr_reg = ( opperand == `opp_str || opperand == `opp_load ? otherRegister : 0 );
    wire [wordsize-1:0] cpu_addr = ( !fetching_instruction ? addr_reg | addr_pop | addr_push : programCounter); //The defaut behaviour is to seek the address of the next piece of code

    //selecting the data to send
    wire [wordsize-1:0] data_wr = ( instruction == `inst_push || opperand == `opp_str || instruction == `inst_tbm ? workingRegister : 0 );
    wire [wordsize-1:0] data_pc = ( instruction == `inst_call ? programCounter : 0 );
    wire [wordsize-1:0] data_out_cpu = data_wr | data_pc;

    //data to send to the cpu
    wire [wordsize-1:0] data_in_cpu;
    wire returning_value = instruction == `inst_pop || instruction == `inst_ret || opperand == `opp_load;
    wire [wordsize-1:0] data_out_out = ( returning_value ? (instruction == `inst_ret ? data_in_cpu + 1 : data_in_cpu) : 0 ); //When we want to use walue read from ram. Note, when returning from a function, we need to go after what we put in the stack in order not to be trapped in an infinite loop
    wire [wordsize-1:0] wr_out = ( instruction == `inst_push || instruction == `inst_call || opperand == `opp_str ? workingRegister : 0 ); //when we don't need to update any register we will simply put the content of the working register into itself
    assign out = wr_out | data_out_out;

    //register to update
    assign out_reg = ( instruction == `inst_ret || instruction == `inst_call ? `pc_id : 0 ); 

    //write enable
    wire cpu_write_en = (instruction == `inst_push || instruction == `inst_call || opperand == `opp_str) & !fetching_instruction & !(|ram_not_ready);
    
    //handeling reduced behavior
    generate
        if(wordsize == 8) //no reduced behavior possible
        begin
            assign data_in_cpu = data_in;
            assign data_out = data_out_cpu;
            assign addr = cpu_addr;
            assign write_en = cpu_write_en;
            assign alignement_fixer_ready = 1;
            assign pop_offset = 1;
            assign byte_mode = 0;
        end
        else
        begin
            //Handleling the toggle of byte mode
            reg byte_mode_r;
            always @ (posedge clk)
                if(!reset)
                    byte_mode_r <= 0;
                else
                    if(instruction == `inst_tbm && !ram_not_ready)
                        byte_mode_r <= !byte_mode_r;
            assign byte_mode = byte_mode_r;

            //Controling the width of data to get
            wire [15:0] size_used = ( fetching_instruction | byte_mode ? 0 :
                                      ( reduced_behaviour_bits == 2'b00 ? (wordsize/8 - 1) :
                                        ( reduced_behaviour_bits == 2'b01 ? 2 : //TODO, make the value 2 depends on the wordsize as it is currentely broken in 32 bit CPU
                                          ( reduced_behaviour_bits == 2'b10 ? 1 : 0 ))));
            reflet_alignement_fixer #(.word_size(wordsize), .addr_size(wordsize)) alignement_fixer (
                .clk(clk),
                .size_used(size_used[$clog2(wordsize/8):0]),
                .ready(alignement_fixer_ready),
                .alignement_error(alignement_error),
                //Bus to the CPU
                .cpu_addr(cpu_addr),
                .cpu_data_out(data_out_cpu),
                .cpu_data_in(data_in_cpu),
                .cpu_write_en(cpu_write_en),
                //Bus to the RAM
                .ram_addr(addr),
                .ram_data_out(data_out),
                .ram_data_in(data_in),
                .ram_write_en(write_en));
            wire reduced_behaviour = ((reduced_behaviour_bits != 2'b00) && ( (reduced_behaviour_bits == 2'b01 && wordsize > 32) || (reduced_behaviour_bits == 2'b10 && wordsize > 16) || (reduced_behaviour_bits == 2'b11 && wordsize > 8) )) || byte_mode;
            assign pop_offset = ( reduced_behaviour && instruction != `inst_ret && instruction != `inst_call ? 
                            ( reduced_behaviour_bits == 2'b11 || byte_mode ? 1 :
                                (reduced_behaviour_bits == 2'b10 ? 2 :
                                    4))
                        : wordsize/8 );

        end
    endgenerate

    //Changing the not_ready register to let the CPU<->RAM commuticatio to occur
    always @ (posedge clk)
        if(!reset)
        begin
            not_ready <= 4'h2;
            instruction <= 8'h0;
            hide_ready <= 0;
        end
        else if(enable & alignement_fixer_ready)
        begin
            case(not_ready)
                0 : //we are ready and thus must addapt the time of not ready to engage the communication with the ram
                begin
                    if(using_ram)
                    begin
                        if(hide_ready) //About to fetch an instruction
                            not_ready <= 4'h2;
                        else //Interfacing whith RAM
                            not_ready <= 4'h2;
                        hide_ready <= !hide_ready;
                    end
                    else
                        not_ready <= 4'h2;
                end
                1 : 
                begin 
                    if(!hide_ready)
                        instruction <= data_in_cpu[7:0];
                    not_ready <= not_ready - 4'h1;
                end
                default : not_ready <= not_ready - 4'h1;
            endcase
        end

endmodule
    
