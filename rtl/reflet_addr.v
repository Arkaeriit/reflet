/*--------------------------------------------\
|This module handle connection between the RAM|
|and the rest of the CPU.                     |
|Updating the stackPointer is done by the CPU.|
\--------------------------------------------*/

`include "reflet.vh"

`define STATE_UPDATE_REGS       3'd0
`define STATE_GET_INSTRUCTION   3'd1
`define STATE_SAVE_INSTRUCTION  3'd2
`define STATE_COMPUTE           3'd3
`define STATE_ADDR_READ_PREPARE 3'd4
`define STATE_ADDR_WRITE        3'd5
`define STATE_ADDR_READ         3'd6
`define STATE_UPDATE_BUFF_REGS  3'd7

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
    output update_pc,
    output ram_not_ready
    );

    reg [2:0] state;
    reg [wordsize-1:0] read_from_mem_tmp;
    wire alignement_fixer_ready;

    //addr selection
    wire [wordsize-1:0] addr_pop = ( instruction == `inst_pop || instruction == `inst_ret ? stackPointer - wordsize/8 : 0 ); //We need the -wordsize/8 because the CPU updated the stack pointer
    wire [wordsize-1:0] addr_push = ( instruction == `inst_push || instruction == `inst_call ? stackPointer : 0 );
    wire [wordsize-1:0] addr_reg = ( opperand == `opp_str || opperand == `opp_load ? otherRegister : 0 );
    wire [wordsize-1:0] cpu_addr = ( (state == `STATE_ADDR_WRITE || state == `STATE_ADDR_READ_PREPARE || state == `STATE_ADDR_READ) ? addr_reg | addr_pop | addr_push : programCounter); //The defaut behaviour is to seek the address of the next piece of code

    //State branch selection
    wire [7:0] next_instruction = data_in_cpu[7:0];
    wire next_branch_read = next_instruction == `inst_pop || next_instruction == `inst_ret || next_instruction[7:4] == `opp_load;
    wire next_branch_write = next_instruction == `inst_push || next_instruction == `inst_call || next_instruction[7:4] == `opp_str;
    wire [3:0] next_branch_state = ( next_branch_read ? `STATE_ADDR_READ_PREPARE :
                                     ( next_branch_write ? `STATE_ADDR_WRITE : `STATE_COMPUTE ));

    //Communication with the CPU
    assign update_pc = state == `STATE_UPDATE_BUFF_REGS;
    assign ram_not_ready = state != `STATE_UPDATE_REGS;

    //selecting the data to send
    wire [3:0] opperand = instruction[7:4];
    wire [wordsize-1:0] data_wr = ( instruction == `inst_push || opperand == `opp_str ? workingRegister : 0 );
    wire [wordsize-1:0] data_pc = ( instruction == `inst_call ? programCounter : 0 );
    wire [wordsize-1:0] data_out_cpu = data_wr | data_pc;

    //data to send to the cpu
    wire [wordsize-1:0] data_in_cpu;
    wire returning_value = instruction == `inst_pop || instruction == `inst_ret || opperand == `opp_load;
    wire [wordsize-1:0] data_out_out = ( returning_value ? (instruction == `inst_ret ? read_from_mem_tmp + 1 : read_from_mem_tmp) : 0 ); //When we want to use walue read from ram. Note, when returning from a function, we need to go after what we put in the stack in order not to be trapped in an infinite loop
    wire [wordsize-1:0] wr_out = ( instruction == `inst_push || instruction == `inst_call || opperand == `opp_str || instruction == `inst_tbm ? workingRegister : 0 ); //when we don't need to update any register we will simply put the content of the working register into itself
    assign out = wr_out | data_out_out;

    //register to update
    assign out_reg = ( instruction == `inst_ret || instruction == `inst_call ? `pc_id : 0 ); 

    //write enable
    wire cpu_write_en = state == `STATE_ADDR_WRITE;
    
    //handeling reduced behavior
    generate
        if(wordsize == 8) //no reduced behavior possible
        begin
            assign data_in_cpu = data_in;
            assign data_out = data_out_cpu;
            assign addr = cpu_addr;
            assign write_en = cpu_write_en;
            assign alignement_fixer_ready = 1;
            assign byte_mode = 0;
        end
        else
        begin
            assign addr = cpu_addr;
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
            wire force_alligned_use = instruction == `inst_pop || instruction == `inst_ret || instruction == `inst_push || instruction == `inst_call;
            wire [15:0] size_used = ( state == `STATE_UPDATE_REGS ? 0 :
                                      ( force_alligned_use ? (wordsize/8 - 1) :
                                        ( byte_mode ? 0 :
                                          ( reduced_behaviour_bits == 2'b00 ? (wordsize/8 - 1) :
                                            ( reduced_behaviour_bits == 2'b01 ? 2 : //TODO, make the value 2 depends on the wordsize as it is currentely broken in 32 bit CPU
                                              ( reduced_behaviour_bits == 2'b10 ? 1 : 0 ))))));
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
                .ram_addr(), //Not used as we want to output the raw address
                .ram_data_out(data_out),
                .ram_data_in(data_in),
                .ram_write_en(write_en));
            wire reduced_behaviour = ((reduced_behaviour_bits != 2'b00) && ( (reduced_behaviour_bits == 2'b01 && wordsize > 32) || (reduced_behaviour_bits == 2'b10 && wordsize > 16) || (reduced_behaviour_bits == 2'b11 && wordsize > 8) )) || byte_mode;
        end
    endgenerate

    always @ (posedge clk)
        if (!reset)
        begin
            state <= `STATE_GET_INSTRUCTION;
        end
        else if(enable & alignement_fixer_ready)
        begin
            case(state)
                `STATE_UPDATE_REGS: begin
                    state <= `STATE_GET_INSTRUCTION;
                end
                `STATE_GET_INSTRUCTION: begin
                    state <= `STATE_SAVE_INSTRUCTION;
                end
                `STATE_SAVE_INSTRUCTION: begin
                    instruction <= next_instruction;
                    state <= next_branch_state;
                end
                `STATE_COMPUTE: begin
                    state <= `STATE_UPDATE_BUFF_REGS;
                end
                `STATE_ADDR_WRITE: begin
                    state <= `STATE_UPDATE_BUFF_REGS;
                end
                `STATE_ADDR_READ_PREPARE: begin
                    state <= `STATE_ADDR_READ;
                end
                `STATE_ADDR_READ: begin
                    state <= `STATE_UPDATE_BUFF_REGS;
                    read_from_mem_tmp <= data_in_cpu;
                end
                `STATE_UPDATE_BUFF_REGS: begin
                    state <= `STATE_UPDATE_REGS;
                end
            endcase
        end

endmodule
    
