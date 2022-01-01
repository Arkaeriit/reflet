/*---------------------------\
|This module is meant to     |
|test the memory_tester      |
|module in its simplest form.|
\---------------------------*/

module memory_tester_tb();
    
    reg clk = 0;
    always #1 clk <= !clk;
    reg reset = 0;

    wire [15:0] addr;
    wire [15:0] data_in;
    wire [15:0] data_out;
    wire wr_en;

    wire content_ok;

    memory_tester mem_test (
        .clk(clk),
        .reset(reset),
        .addr(addr),
        .data_in(data_in),
        .data_out(data_out),
        .write_en(wr_en),
        .content_ok(content_ok));

    mem_filler filler (
        .clk(clk),
        .reset(reset),
        .addr(addr),
        .data_out(data_in),
        .wr_en(wr_en));

    integer i;
    initial
    begin
        $dumpfile("memory_tester_tb.vcd");
        $dumpvars(0, memory_tester_tb);
        for(i = 0; i<2; i=i+1)
            $dumpvars(0, mem_test.mem[i]);
        #10;
        reset <= 1;
        #490;
        $finish;
    end

endmodule

/*------------------\
|This modules fills |
|a memory with ones.|
\------------------*/
module mem_filler #(
    parameter addr_size = 16,
    word_size = 16
    ) (
    input clk,
    input reset,
    // Memory buss
    output reg [addr_size-1:0] addr,
    output [word_size-1:0] data_out,
    output reg wr_en
    );

    assign data_out = ~0;

    reg [1:0] state;

    always @ (posedge clk)
        if (!reset)
        begin
            addr <= 0;
            wr_en <= 0;
            state <= 0;
        end
        else
        begin
            case(state)
                2'b00:
                    wr_en <= 1'b1;
                2'b01:
                    wr_en <= 1'b0;
                2'b10:
                    addr <= addr + 1;
            endcase
            state <= state + 1;
        end

endmodule

