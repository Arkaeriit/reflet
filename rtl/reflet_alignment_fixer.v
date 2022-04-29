/*--------------------------------------------------\
|This module is meant to handle misaligned read or  |
|write to a memory that requires access aligned to  |
|the word size. It is meant to work on little endian|
|systems. The granularity of the operations in a    |
|byte. The input size_used tells the size of the    |
|data to transfer. For example, when size_used is   |
|equal to 0, the data is a byte wide and should be  |
|aligned to a byte. When size_used is 1, the data is|
|two bytes wide and should be aligned to 16 bits.   |
\--------------------------------------------------*/

module reflet_alignment_fixer #(
    parameter word_size = 32,
    addr_size = 32
    )(
    input clk,
    input reset,
    input [$clog2(word_size/8):0] size_used,
    output reg ready,
    output alignment_error,
    //Bus to the CPU
    input [addr_size-1:0] cpu_addr,
    input [word_size-1:0] cpu_data_out,
    output [word_size-1:0] cpu_data_in,
    input cpu_write_en,
    input cpu_read_en,
    //Bus to the RAM
    output [addr_size-1:0] ram_addr,
    output [word_size-1:0] ram_data_out,
    input [word_size-1:0] ram_data_in,
    output ram_write_en,
    output ram_read_en
    );

    //Checking alignment errors
    wire [addr_size-1:0] invalid_addr_mask = (1 << size_used) - 1;
    assign alignment_error = |(cpu_addr & invalid_addr_mask);

    //Various masks used by the system
    wire [word_size/8-1:0] byte_shift = 1 << size_used;
    wire [word_size-1:0] data_mask = (1 << byte_shift * 8) - 1; //Mask of the usable byte

    //Computing real output addr
    wire [addr_size-1:0] addr_mask = ~(word_size/8 - 1);
    assign ram_addr = cpu_addr & addr_mask;
    wire [addr_size-1:0] addr_diff = cpu_addr - ram_addr;

    //Reading shift
    assign cpu_data_in = (ram_data_in >> (addr_diff * 8)) & data_mask;

    //Writing data to RAM
    wire missaligned_access = |addr_diff; //Not erroneous data access
    wire [word_size-1:0] shifted_write = (cpu_data_out & data_mask) << (addr_diff * 8); //Data to write shifted to its right position
    wire [word_size-1:0] data_copy = ~(data_mask << (addr_diff * 8)) & ram_data_in; //Data from the bytes that will not be changed in the current write
    wire [word_size-1:0] fixed_data_out_ram = data_copy | shifted_write; //Completa data to write to RAM
    assign ram_data_out = (missaligned_access ? fixed_data_out_ram : cpu_data_out);
    assign ram_write_en = cpu_write_en & ready;
    assign ram_read_en = cpu_read_en & ready;

    //Detecting changes in inputs
    wire [word_size+addr_size-1:0] all_inputs = {cpu_addr, cpu_data_out};
    reg [word_size+addr_size-1:0] old_input;
    wire new_input = all_inputs != old_input;
    always @ (posedge clk)
        old_input <= all_inputs;
    wire ready_condition = !missaligned_access | (!cpu_write_en & !cpu_read_en) | !new_input;
    always @ (posedge clk)
        if (!reset)
            ready <= 1;
        else
            ready <= ready_condition;

endmodule

