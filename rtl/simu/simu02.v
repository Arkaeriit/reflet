// This test bench runs a basic test of some instruction.
// It executes the code written in test_inst_mini_load.asm
// and compares it against expected values.

module simu02();

    reg clk = 1;
    always #1 clk <= !clk;

    reg reset = 0;
    reg enable = 1;
    wire [7:0] dIn;
    wire [7:0] dOut;
    wire [7:0] addr;
    wire write_en;
    wire quit;
    wire debug;
    wire content_ok;    
    
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
        .interrupt_request(4'h0));

    // The rom got the addresses between 0x0000 and 0x7FFF
    wire [7:0] dataRom;
    rom02 rom02(
        .clk(clk), 
        .enable(!addr[7]), 
        .addr(addr[6:0]), 
        .data(dataRom));

    // Fake ROM testing the result of the program
    wire [7:0] data_memtester;
    memory_tester #(
        .base_addr(8'h80),
        .addr_size(8),
        .array_size(4),
        .word_size(8),
        .array_content(32'h0806_0402)
    ) tester (
        .clk(clk),
        .reset(reset),
        .addr(addr),
        .data_in(dOut),
        .write_en(write_en),
        .data_out(data_memtester),
        .content_ok(content_ok));

    assign dIn = dataRom | data_memtester;

    integer i;

    initial
    begin
        $dumpfile("simu02.vcd");
        $dumpvars(0, simu02);
        for(i = 0; i<16; i=i+1)
        begin
            $dumpvars(0, cpu.registers[i]);
        end
        #100;
        reset <= 1;
        #2000;
        $finish;
    end

endmodule

