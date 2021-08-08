/*
|This module is meant to handle misaligned read or write to a memory that requires access aligned to the word size. It is ment to work on little endian systems. The granularity of the operations in a byte. The input size_used tells the size of the data to transfer. For example, when size_used is equal to 1, the data is a byte wide and should be aligned to a byte. When size_used is 1, the data is two bytes wide and should be aligned to 16 bits.
    */

module reflet_alignement_fixer #(
    parameter word_size = 32,
    addr_size = 32
    )(
    input clk,
    input reset,
    input [$clog2(word_size/8):0] size_used,
    output ready,
    output allignement_error,
    //Bus to the CPU
    input [addr_size-1:0] cpu_addr,
    input [word_size-1:0] cpu_data_out,
    output [word_size-1:0] cpu_data_in,
    input cpu_write_en,
    //Bus to the RAM
    output [addr_size-1:0] ram_addr,
    output [word_size-1:0] ram_data_out,
    input [word_size-1:0] ram_data_in,
    output ram_write_en
    );

    //Checking alignment errors
    wire [addr_size-1:0] invalid_addr_mask = (1 << size_used) - 1;
    assign allignement_error = |(cpu_addr & invalid_addr_mask);

    //Various masks used by the system
    wire [word_size/8-1:0] byte_shift = 1 << size_used;
    wire [word_size-1:0] data_mask = (1 << byte_shift * 8) - 1; //Mask of the usable byte

    //Computing real output addr
    wire [addr_size-1:0] addr_mask = ~$clog2(word_size/8);
    assign ram_addr = cpu_addr & addr_mask;
    wire [addr_size-1:0] addr_diff = cpu_addr - ram_addr;

    //Reading shift
    assign cpu_data_in = (ram_data_in >> (addr_diff * 8)) & data_mask;
    
endmodule

