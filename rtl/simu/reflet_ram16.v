/*-------------------------------------\
|This module codes for a ram block with|
|the same input and output addresses.  |
|The words are 16 bits wide and each   |
|byte can be addressed.                |
|It is made by combining two 8-bits    |
|modules in parallel, one for even     |
|addresses and one for odd ones.       |
\-------------------------------------*/

//Compute the size 
`define size_even  (size/2 + size%2)
`define size_odd   (size/2)

module reflet_ram16 #(
    parameter addrSize = 9,
    size = 2**addrSize,
    resetable = 1
    )(
    input clk,
    input reset,
    input enable,
    input [addrSize-1:0] addr,
    input [15:0] data_in,
    input write_en,
    output [15:0] data_out
    );
    
    //Translate the addresses
    wire [addrSize-1:0] addr_even = addr[addrSize-1:1] + addr[0];
    wire [addrSize-1:0] addr_odd = addr[addrSize-1:1];

    //Combine the output of both block to generate the global data_out
    wire [7:0] data_out_even;
    wire [7:0] data_out_odd;
    assign data_out = (addr[0] ? {data_out_even, data_out_odd} : {data_out_odd, data_out_even});

    //Spliting tha data_in into two parts and give each one to a blocl
    wire [7:0] data_in_even = (addr[0] ? data_in[15:8] : data_in[7:0]);
    wire [7:0] data_in_odd = (!addr[0] ? data_in[15:8] : data_in[7:0]);

    reflet_ram8 #(
        .addrSize(addrSize),
        .size(`size_even),
        .resetable(resetable))
    ram_even (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .addr(addr_even),
        .data_in(data_in_even),
        .write_en(write_en),
        .data_out(data_out_even));

    reflet_ram8 #(
        .addrSize(addrSize),
        .size(`size_odd),
        .resetable(resetable))
    ram_odd (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .addr(addr_odd),
        .data_in(data_in_odd),
        .write_en(write_en),
        .data_out(data_out_odd));

endmodule

