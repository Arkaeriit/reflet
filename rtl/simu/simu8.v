// This test bench runs an extensive test of most instructions.

module simu8();

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
    wire content_ok;    
    
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
        .ext_int(4'h0));

    // The rom got the addresses between 0x0000 and 0x7FFF
    wire [15:0] dataRom;
    rom8 rom8(
        .clk(clk), 
        .enable(!addr[15]), 
        .addr(addr[14:1]), 
        .data(dataRom));

    // A bit of RAM needed for the code to word in the address range 0x8000 to 0x8FFF
    wire [15:0] dataRam;
    reflet_ram16 #(.addrSize(12), .size(32'h7FF)) ram(
        .clk(clk), 
        .reset(reset), 
        .enable(addr[15:12] == 4'h8), 
        .addr(addr[11:0]), 
        .data_in(dOut), 
        .write_en(write_en), 
        .data_out(dataRam));

    // Fake ROM testing the result of the program
    wire [15:0] data_memtester;
    memory_tester #(
        .base_addr(15'h4800), // 0x9000 from the processor's point of view
        .addr_size(15),
        .array_size(20),
        .word_size(16),
        .array_content(320'h0000_0000_0000_00FF_000A_FFF3_0050_005A_0003_000E_FF55_0003_00FF_0002_0000_FFCE_0064_0019_0C80_0006)
    ) tester (
        .clk(clk),
        .reset(reset),
        .addr(addr[15:1]),
        .data_in(dOut),
        .write_en(write_en),
        .data_out(data_memtester),
        .content_ok(content_ok));

    assign dIn = dataRom | dataRam | data_memtester;

    integer i;

    initial
    begin
        $dumpfile("simu8.vcd");
        $dumpvars(0, simu8);
        for(i = 0; i<16; i=i+1)
        begin
            $dumpvars(0, ram.ram[i]);
            $dumpvars(0, cpu.registers[i]);
        end
        #100;
        reset <= 1;
        #100000;
        $finish;
    end

endmodule

