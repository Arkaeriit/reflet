// This tests the jump instructions. This executes the code in test_jump.asm
// and we expect two debug instruction and a quit.

module simu03();

    reg clk = 1;
    always #1 clk <= !clk;

    reg reset = 0;
    reg enable = 1;
    wire [15:0] dIn;
    wire [15:0] dOut;
    wire [15:0] addr;
    wire write_en;
    wire quit;
    wire debug;
    
    
    reflet_cpu #(.wordsize(16)) cpu(
        .clk(clk), 
        .reset(reset), 
        .enable(enable),
        .quit(quit), 
        .debug(debug),
        .data_in(dIn), 
        .addr(addr), 
        .data_out(dOut), 
        .write_en(write_en),
        .interrupt_request(4'h0));

    //The rom got the addresses between 0x0000 and 0xFFFF
    rom03 rom03(
        .clk(clk), 
        .enable(1'b1), 
        .addr(addr[15:1]), 
        .data(dIn));

    initial
    begin
        $dumpfile("simu03.vcd");
        $dumpvars(0, simu03);
        #100;
        reset <= 1;
        #1000;
        $finish;
    end

endmodule

