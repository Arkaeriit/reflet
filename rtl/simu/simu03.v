
module simu03();

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
        .interrupt_request(4'h0));

    //The rom got the addresses between 0x0000 and 0x7FFF
    wire [15:0] dataRom;
    rom3 rom3(
        .clk(clk), 
        .enable_out(!addr[15]), 
        .addr(addr[7:0]), 
        .dataOut(dataRom));
    //The ram got the addresses between 0x8000 and 0xFFFF
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

    integer i;

    initial
    begin
        $dumpfile("simu03.vcd");
        $dumpvars(0, simu03);
        for(i = 0; i<16; i=i+1)
            $dumpvars(0, cpu.registers[i]);
        #10;
        reset = 1;
        #800;
        $finish;
    end

    always @ (posedge clk)
        if(quit)
            reset = 0;


endmodule

