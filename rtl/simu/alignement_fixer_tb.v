
module alignement_fixer_tb();

    reg clk = 1;
    always #1 clk <= !clk;
    reg reset = 0;

    //reg [63:0] cpu_addr = 64'h123456789ABCDEF;
    reg [63:0] cpu_addr = 0;
    reg [3:0] size_reduction = 0;

    reflet_alignement_fixer #(.word_size(64), .addr_size(64)) fix (
        .clk(clk),
        .reset(reset),
        .cpu_addr(cpu_addr),
        .ram_data_in(64'hFEDCBA9876543210),
        .size_used(size_reduction));

    initial
    begin
        $dumpfile("alignement_fixer_tb.vcd");
        $dumpvars(0, alignement_fixer_tb);
        #10;
        size_reduction <= 1;
        #10;
        size_reduction <= 2;
        #10;
        size_reduction <= 3;
        #10;
        size_reduction <= 4;
        #10;
        $finish;
    end

endmodule

