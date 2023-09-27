// This test bench runs test_interruption.asm which tests that the interruptions
// works as planed. We expect three debug signals and then a quit.
// Furthermore, as this is a 32 bits CPU running a 16 bits program, this also
// tests the reduced behavior and how it is implemented in the assembler.

module simu06();

    reg clk = 1;
    always #1 clk <= !clk;

    reg reset = 0;
    wire enable = 1;
    wire [31:0] dIn;
    wire [31:0] dOut;
    wire [31:0] addr;
    wire write_en;
    wire quit;
    wire debug;
    wire content_ok;    
    reg int1 = 0;
    
    reflet_cpu #(.wordsize(32)) cpu(
        .clk(clk), 
        .reset(reset), 
        .enable(enable),
        .quit(quit), 
        .debug(debug),
        .data_in(dIn), 
        .addr(addr), 
        .data_out(dOut), 
        .write_en(write_en),
        .interrupt_request({2'h0, int1, 1'h0}));

    // The ROM got the addresses between 0x0000 and 0xFFF
    wire [31:0] dataRom;
    rom06 rom06(
        .clk(clk), 
        .enable(addr < 32'h1000), 
        .addr(addr[31:2]), 
        .data(dataRom));

    // A bit of RAM needed for the code to work in the address range 0x1000 to 0x1FFF
    wire [31:0] dataRam;
    reflet_ram #(.addrSize(30), .wordsize(32), .size(32'h400)) ram(
        .clk(clk), 
        .reset(reset), 
        .enable(addr >= 32'h1000), 
        .addr(addr[31:2]), 
        .data_in(dOut), 
        .write_en(write_en), 
        .data_out(dataRam));

    assign dIn = dataRom | dataRam;

    integer i;

    initial
    begin
        $dumpfile("simu06.vcd");
        $dumpvars(0, simu06);
        for(i = 0; i<16; i=i+1)
            $dumpvars(0, cpu.registers[i]);
        for(i = 0; i<4; i=i+1)
        begin
            $dumpvars(0, cpu.interrupt_ctrl.routines[i]);
            $dumpvars(0, cpu.interrupt_ctrl.level_memory[i]);
            $dumpvars(0, cpu.interrupt_ctrl.addr_memory[i]);
        end
        #100;
        reset <= 1;
        #4900;
        int1 <= 1;
        #5
        int1 <= 0;
        #4995;
        int1 <= 1;
        #5
        int1 <= 0;
        #4995;
        int1 <= 1;
        #5
        int1 <= 0;
        #4995;
        int1 <= 1;
        #5
        int1 <= 0;
        #4995;
        int1 <= 1;
        #5
        int1 <= 0;
        #4995;
        int1 <= 1;
        #5
        int1 <= 0;
        $finish;
    end

endmodule

