/*---------------------------------\
|This is the top module of teh CPU.|
|The register are defined here.    |
\---------------------------------*/

`include "reflet.vh"

module reflet_cpu #(
    parameter wordsize = 16
    )(
    //main control signal
    input clk,
    input reset,
    input enable,
    //System bus connection
    input [wordsize-1:0] data_in,
    output [wordsize-1:0] addr,
    output [wordsize-1:0] data_out,
    output write_en,
    //Other connections
    output reg quit, //Set to one when the quit instruction is read
    output debug,
    input [3:0] ext_int
    );
    integer i; //loop counter

    reg [wordsize-1:0] registers [15:0]; //The registers 

    //register to change index
    wire [3:0] index_addr;
    wire [3:0] index_alu;
    wire [3:0] index_int;
    wire [3:0] index = index_addr | index_alu | index_int;

    //Content of the register to change
    wire [wordsize-1:0] content_addr, content_alu, content_int;
    reg [wordsize-1:0] content_addr_r, content_alu_r, content_int_r;
    wire [wordsize-1:0] content = content_addr_r | content_alu_r | content_int_r;
    always @ (posedge clk)
        if (!reset)
        begin
            content_alu_r <= 0;
            content_addr_r <= 0;
            content_int_r <= 0;
        end
        else
        begin
            content_alu_r <= content_alu;
            content_addr_r <= content_addr;
            content_int_r <= content_int;
        end

    //Register being used along the working register
    wire [7:0] instruction;
    wire [3:0] argument_id = instruction[3:0];
    reg [7:0] instruction_alu, instruction_int, instruction_cpu;
    wire [wordsize-1:0] other_register = registers[argument_id];

    //submodules
    wire ram_not_ready;
    wire update_pc;
    //wire [3:0] opperand = instruction[7:4];
    wire interrupt;
    wire [wordsize-1:0] int_routine;
    wire alignment_error;
    wire [3:0] used_int = {ext_int[3:1], ext_int[0] | (registers[`sr_id][7] & alignment_error)}; //External interrupt or notification for alignment error

    reflet_alu #(.wordsize(wordsize)) alu(
        .working_register(registers[`wr_id]),
        .other_register(other_register),
        .status_register(registers[`sr_id]),
        .instruction(instruction_alu),
        .out(content_alu),
        .out_reg(index_alu));

    reflet_addr #(.wordsize(wordsize)) ram_interface(
        .clk(clk),
        .reset(reset),
        .enable(enable & !quit),
        //instruction from the CPU
        .workingRegister(registers[`wr_id]),
        .programCounter(registers[`pc_id]),
        .stackPointer(registers[`sp_id]),
        .otherRegister(other_register),
        .reduced_behaviour_bits(registers[`sr_id][2:1]),
        .instruction(instruction),
        .alignment_error(alignment_error),
        //System bus
        .addr(addr),
        .data_out(data_out),
        .data_in(data_in),
        .write_en(write_en),
        //Out to the CPU
        .out(content_addr),
        .out_reg(index_addr),
        .update_pc(update_pc),
        .ram_not_ready(ram_not_ready));

    reflet_interrupt #(.wordsize(wordsize)) interrupt_ctrl (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .ext_int(used_int),
        .instruction(instruction_int),
        .working_register(registers[`wr_id]),
        .program_counter(registers[`pc_id]),
        .int_mask(registers[`sr_id][6:3]),
        .out(content_int),
        .out_reg(index_int),
        .out_routine(int_routine),
        .cpu_update(!ram_not_ready),
        .interrupt(interrupt));

    //debug signal
    assign debug = instruction == `inst_debug && !ram_not_ready;
    
    //updating registers
    always @ (posedge clk)
        if(!reset)
        begin
           registers[`wr_id] <= `wr_reset;
           registers[`sr_id] <= `sr_reset;
           registers[`pc_id] <= `pc_reset;
           registers[`sp_id] <= `sp_reset;
           for(i=`gp_start; i<=`gp_end; i=i+1)
               registers[i] <= `gp_reset;
           quit = 0;
        end
        else if(enable & !quit)
        begin
            instruction_cpu <= instruction;
            instruction_int <= instruction;
            instruction_alu <= instruction;
            if(!ram_not_ready & !quit)
            begin
                if(interrupt)
                begin
                    registers[`pc_id] <= int_routine;
                end
                else
                begin
                    case(instruction_cpu)
                        `inst_quit : quit <= 1'b1;
                        `inst_pop :
                        begin
                            registers[`sp_id] <= registers[`sp_id] - wordsize/8;
                            registers[index] <= content;
                        end
                        `inst_ret :
                        begin
                            registers[`sp_id] <= registers[`sp_id] - wordsize/8;
                            registers[index] <= content;
                        end
                        `inst_push : registers[`sp_id] <= registers[`sp_id] + wordsize/8;
                        `inst_call : 
                        begin
                            registers[`sp_id] <= registers[`sp_id] + wordsize/8;
                            registers[index] <= content;
                        end
                        default : registers[index] <= content;
                    endcase
                end
            end
            else if(update_pc)
            begin
                registers[`pc_id] <= registers[`pc_id] + 1;
            end
        end

endmodule

