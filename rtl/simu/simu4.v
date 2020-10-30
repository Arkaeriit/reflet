
module simu4();

    reg clk = 1;
    always #1 clk = !clk;

    reg reset = 0;
    wire [15:0] dIn;
    wire [15:0] dOut;
    wire [15:0] addr;
    wire write_en;
    wire quit;
    
    
    asrm_cpu #(16) cpu(clk, reset, quit, dIn, addr, dOut, write_en);

    //The rom got the addresses between 0x00 and 0x7F
    wire [7:0] dataRom;
    rom4 rom4(clk, !addr[15], addr[7:0], dataRom);
    //The ram got the addresses between 0x80 and 0xFF
    wire [15:0] dataRam;
    ram16 #(15) ram(clk, reset, addr[15], addr[14:0], dOut, write_en, dataRam);

    assign dIn = dataRam | {8'h0, dataRom};

    initial
    begin
        #10;
        reset = 1;
    end

    always @ (posedge clk)
        if(quit)
            reset = 0;


endmodule

