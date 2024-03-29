/*--------------------------------------\
|This module presents itself as a bit of|
|RAM and tests that the content of that |
|RAM contains a predefined value.       |
\--------------------------------------*/

module memory_tester #(
    parameter base_addr = 0,
    addr_size = 16,
    word_size = 16,
    array_size = 2,
    array_content = 32'hFFFFFFFF
    ) (
    input clk,
    input reset,
    //Memory bus
    input [addr_size-1:0] addr,
    input [word_size-1:0] data_in,
    output reg [word_size-1:0] data_out,
    input write_en,
    //Test result
    output content_ok
    );

    reg [word_size-1:0] mem [array_size-1:0];
    reg [word_size-1:0] ref [array_size-1:0];
    reg [array_size-1:0] arr_ok;
    wire addr_ok = addr >= base_addr && (addr-base_addr < array_size);
    wire [word_size*array_size-1:0] array_content_show = array_content;
    wire [addr_size-1:0] offset = addr - base_addr;

    // Checking that the content of the array matches the expected value
    integer i;
    always @ (posedge clk)
        if (!reset) 
        begin
            arr_ok <= 0;
            for (i=0; i<array_size; i=i+1)
            begin
                mem[i] <= ~((array_content >> (i * (word_size))) & ((1 << word_size) - 1));
                ref[i] <= (array_content >> (i * (word_size))) & ((1 << word_size) - 1);
            end
        end
        else
            for (i=0; i<array_size; i=i+1)
            begin
                arr_ok[i] <= ref[i] == mem[i];
            end
    assign content_ok = &arr_ok;

    // Basic memory interface
    always @ (posedge clk)
        if (!reset)
        begin
            data_out <= 0;
        end
        else
        begin
            if (write_en && addr_ok)
                mem[offset] <= data_in;
            data_out <= (addr_ok ? mem[offset] : 0);
        end

endmodule

