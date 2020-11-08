
module simu2();

    reg clk = 1;
    always #1 clk = !clk;

    reg reset = 0;
    wire [7:0] dIn;
    wire [7:0] dOut;
    wire [7:0] addr;
    wire write_en;
    wire quit;
    
    
    asrm_cpu #(.wordsize(8)) cpu(
        .clk(clk), 
        .reset(reset), 
        .quit(quit), 
        .data_in(dIn), 
        .addr(addr), 
        .data_out(dOut), 
        .write_en(write_en),
        .extint(4'h0));

    //The rom got the addresses between 0x00 and 0x7F
    wire [7:0] dataRom;
    rom2 rom2(
        .clk(clk), 
        .enable_out(!addr[7]), 
        .addr(addr[4:0]), 
        .out(dataRom));
    //The ram got the addresses between 0x80 and 0xFF
    wire [7:0] dataRam;
    ram #(.addrSize(7), .contentSize(8)) ram(
        .clk(clk), 
        .reset(reset), 
        .output_en(addr[7]), 
        .addr(addr[6:0]), 
        .data_in(dOut), 
        .write_rq(write_en), 
        .data_out(dataRam));

    assign dIn = dataRam | dataRom;

    initial
    begin
        #10;
        reset = 1;
    end

    always @ (posedge clk)
        if(quit)
            reset = 0;


endmodule

