
module simu4();

    reg clk = 1;
    always #1 clk <= !clk;

    reg reset = 0;
    wire [15:0] dIn;
    wire [15:0] dOut;
    wire [15:0] addr;
    wire write_en;
    wire quit;
    
    
    reflet_cpu #(.wordsize(16)) cpu(
        .clk(clk), 
        .reset(reset), 
        .enable(1'b1),
        .quit(quit), 
        .data_in(dIn), 
        .addr(addr), 
        .data_out(dOut), 
        .write_en(write_en),
        .ext_int(4'h0));

    //The rom got the addresses between 0x00 and 0x7F
    wire [15:0] dataRom;
    rom4 rom4(
        .clk(clk), 
        .enable_out(!addr[15]), 
        .addr(addr[6:0]), 
        .dataOut(dataRom));
    //The ram got the addresses between 0x80 and 0xFF
    wire [15:0] dataRam;
    reflet_ram16 #(.addrSize(15)) ram(
        .clk(clk), 
        .reset(reset), 
        .enable(addr[15]), 
        .addr(addr[14:0]), 
        .data_in(dOut), 
        .write_en(write_en), 
        .data_out(dataRam));

    assign dIn = dataRam | dataRom;

    initial
    begin
        $dumpfile("simu4.vcd");
        $dumpvars(0, simu4);
        #10;
        reset = 1;
        #700;
        $finish;
    end

    always @ (posedge clk)
        if(quit)
            reset = 0;


endmodule

