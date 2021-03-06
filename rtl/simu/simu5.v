
module simu5();

    reg clk = 1;
    always #1 clk <= !clk;

    reg reset = 0;
    reg enable = 1;
    wire [7:0] dIn;
    wire [7:0] dOut;
    wire [7:0] addr;
    wire write_en;
    wire quit;
    reg int0 = 0;
    wire [3:0] int = {3'b000, int0};
    wire debug;
    
    
    reflet_cpu #(.wordsize(8)) cpu(
        .clk(clk), 
        .reset(reset), 
        .enable(enable),
        .quit(quit), 
        .debug(debug),
        .data_in(dIn), 
        .addr(addr), 
        .data_out(dOut), 
        .write_en(write_en),
        .ext_int(int));

    //The rom got the addresses between 0x00 and 0x7F
    rom5 rom5(
        .clk(clk), 
        .enable_out(1'b1), 
        .addr(addr[6:0]), 
        .dataOut(dIn));

    initial
    begin
        $dumpfile("simu5.vcd");
        $dumpvars();
        #10;
        reset <= 1;
        #600;
        int0 <= 1;
        #10;
        int0 <= 0;
        #100;
        int0 <= 1;
        #10;
        int0 <= 0;
        #100;
        enable <= 0;
        #10;
        int0 <= 1;
        #10;
        int0 <= 0;
        #10;
        enable <= 1;
        #100;
        $finish;
    end

endmodule

