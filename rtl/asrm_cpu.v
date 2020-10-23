/*---------------------------------\
|This is the top module of teh CPU.|
|The register are defined here.    |
\---------------------------------*/

`include "asrm.vh"

module asrm_cpu#(
    parameter wordsize = 16
)(
    //main control signal
    input clk,
    input reset,
    //System bus connection
    input [wordsize-1:0] data_in,
    output [wordsize-1:0] addr,
    output [wordsize-1:0] data_out,
    output write_en
)

   reg [wordsize-1:0] registers [3:0]; //The registers 

   //reset values
   always @ (posedge clk)
       if(!reset)
       begin
           registers[00] = 0;
           registers[01] = 0;
           registers[02] = 0;
           registers[03] = 0;
           registers[04] = 0;
           registers[05] = 0;
           registers[06] = 0;
           registers[07] = 0;
           registers[08] = 0;
           registers[09] = 0;
           registers[10] = 0;
           registers[11] = 0;
           registers[12] = 0;
           registers[13] = 1;
           registers[14] = 4;
           registers[15] = 0;
       end
    
    //register to change index
    wire [3:0] index_addr;
    wire [3:0] index_alu;
    wire [3:0] index = index_addr | index_alu;

    //Content of the register to change
    wire [wordsize-1:0] content_addr;
    wire [wordsize-1:0] content_alu;
    wire [wordsize-1:0] content = content_alu | content_addr;

    //submodules
    wire ram_not_ready;
    wire [7:0] instruction;
    wire [3:0] opperand = instruction[7:4];
    wire [3:0] argument_id = instruction[3:0];

    asrm_alu #(wordsize) alu(
        registers[wr_id],
        registers[argument_id],
        registers[rs_id],
        opperand,
        content_alu,
        index_alu);

    asrm_addr #(wordsize) ram_interface(
        clk,
        reset,
        registers[pc_id],
        registers[sp_id],
        registers[argument_id],
        instruction,
        addr,
        data_out,
        data_in,
        write_en,
        content_addr,
        index_addr,
        ram_not_ready);
    
    //updating reegisters
    always @ (posedge clk)
        if(reset) //The reset behavious is handeled above
        begin
            if(index != pc_id) //When changing the pc, no need to increment it
                registers[pc_id] = registers[pc_id] + 1;
            if(instruction == inst_pop)
                registers[sp_id] = registers[sp_id] - 1
            if(instruction == inst_push)
                registers[sp_id] = registers[sp_id] + 1
            registers[index] = content
        end
                
endmodule

