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
    wire [wordsize-1:0] content_addr;
    wire [wordsize-1:0] content_alu;
    wire [wordsize-1:0] content_int;
    wire [wordsize-1:0] content = content_alu | content_addr | content_int;

    //submodules
    wire ram_not_ready;
    wire [7:0] instruction;
    wire [3:0] opperand = instruction[7:4];
    wire [3:0] argument_id = instruction[3:0];
    wire int;
    wire [wordsize-1:0] int_routine;

    reflet_alu #(.wordsize(wordsize)) alu(
        .working_register(registers[`wr_id]),
        .other_register(registers[argument_id]),
        .status_register(registers[`sr_id]),
        .instruction(instruction),
        .out(content_alu),
        .out_reg(index_alu));

    reflet_addr #(.wordsize(wordsize)) ram_interface(
        .clk(clk),
        .reset(reset),
        //instruction from the CPU
        .workingRegister(registers[`wr_id]),
        .programCounter(registers[`pc_id]),
        .stackPointer(registers[`sp_id]),
        .otherRegister(registers[argument_id]),
        .reduced_behaviour_bits(registers[`sr_id][2:1]),
        .instruction(instruction),
        //System bus
        .addr(addr),
        .data_out(data_out),
        .data_in(data_in),
        .write_en(write_en),
        //Out to the CPU
        .out(content_addr),
        .out_reg(index_addr),
        .ram_not_ready(ram_not_ready));

    reflet_interrupt #(.wordsize(wordsize)) interrupt(
        .clk(clk),
        .reset(reset),
        .ext_int(ext_int),
        .instruction(instruction),
        .working_register(registers[`wr_id]),
        .program_counter(registers[`pc_id]),
        .int_mask(registers[`sr_id][6:3]),
        .out(content_int),
        .out_reg(index_int),
        .out_routine(int_routine),
        .cpu_update(!ram_not_ready),
        .int(int));

    //computing how much the stack pointer should evolve when using it
    integer default_increase = wordsize/8; //Progression when handling addressis or with no reduced behavious
    integer increase11 = 1; //A separate value for each conbinaison of reduced behaviour
    wire [7:0] increase01 = ( wordsize > 32 ? 4 : default_increase );
    wire [7:0] increase10 = ( wordsize > 16 ? 2 : default_increase );
    wire [1:0] reduced_behavior = registers[`sr_id][2:1];
    wire [7:0] increase_data = ( reduced_behavior == 2'b00 ? default_increase :
                                    ( reduced_behavior == 2'b01 ? increase01 :
                                        ( reduced_behavior == 2'b10 ? increase10 :
                                            increase11)));

    //debug signal
    assign debug = instruction == `inst_debug && !ram_not_ready;
    
    //updating reegisters
    always @ (posedge clk)
        if(!reset)
        begin
           registers[`wr_id] = `wr_reset;
           registers[`sr_id] = `sr_reset;
           registers[`pc_id] = `pc_reset;
           registers[`sp_id] = `sp_reset;
           for(i=`gp_start; i<=`gp_end; i=i+1)
               registers[i] = `gp_reset;
           quit = 0;
        end
        else
        begin
            if(!ram_not_ready & !quit)
            begin
                if(int)
                begin
                    registers[`pc_id] = int_routine;
                end
                else
                begin
                    case(instruction)
                        `inst_quit : quit = 1;
                        `inst_pop :
                        begin
                            registers[`sp_id] = registers[`sp_id] - increase_data;
                            registers[index] = content;
                        end
                        `inst_ret :
                        begin
                            registers[`sp_id] = registers[`sp_id] - default_increase;
                            registers[index] = content;
                        end
                        `inst_push : registers[`sp_id] = registers[`sp_id] + increase_data;
                        `inst_call : 
                        begin
                            registers[`sp_id] = registers[`sp_id] + default_increase;
                            registers[index] = content;
                        end
                        default : registers[index] = content;
                    endcase
                    if(index != `pc_id) //When changing the pc, no need to increment it
                        registers[`pc_id] = registers[`pc_id] + 1;
                end
            end
        end

endmodule

