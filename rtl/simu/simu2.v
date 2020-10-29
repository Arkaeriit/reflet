
module test2();

    reg clk = 1;
    always #1 clk = !clk;

    reg reset = 0;
    wire [7:0] dIn;
    wire [7:0] dOut;
    wire [7:0] addr;
    wire write_en;
    wire quit;
    
    
    asrm_cpu #(8) cpu(clk, reset, quit, dIn, addr, dOut, write_en);

    //The rom got the addresses between 0x00 and 0x7F
    wire [7:0] dataRom;
    rom2 rom2(clk, !addr[7], addr[4:0], dataRom);
    //The ram got the addresses between 0x80 and 0xFF
    wire [7:0] dataRam;
    ram #(7, 8) ram(clk, reset, addr[7], addr[6:0], dOut, write_en, dataRam);

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

