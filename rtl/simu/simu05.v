// This test bench runs test_align_fix.asm which benchmark the result of
// miss-aligned memory writes

module simu05();

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
        .interrupt_request(4'h0));

    wire [31:0] dataRom;
    rom05 rom05(
        .clk(clk), 
        .enable(addr < 32'h1000), 
        .addr(addr[31:2]), 
        .data(dataRom));

    //Fake RAM testing the result of the program
    wire [31:0] data_memtester;
    memory_tester #(
        .base_addr(1024),
        .addr_size(30),
        .array_size(4),
        .word_size(32),
        .array_content(128'h0D0C0B0A_0304EF00_01020304_ABCDEF00)
    ) tester (
        .clk(clk),
        .reset(reset),
        .addr(addr[31:2]),
        .data_in(dOut),
        .write_en(write_en),
        .data_out(data_memtester),
        .content_ok(content_ok));

    assign dIn = dataRom | data_memtester;

    integer i, j;

    initial
    begin
        $dumpfile("simu05.vcd");
        $dumpvars(0, simu05);
        for(i = 0; i<16; i=i+1)
            $dumpvars(0, cpu.registers[i]);
        for(j = 0; j<4; j=j+1)
        begin
            $dumpvars(0, tester.mem[j]);
            $dumpvars(0, tester.ref[j]);
        end
        #100;
        reset <= 1;
        #4000;
        $finish;
    end

endmodule

