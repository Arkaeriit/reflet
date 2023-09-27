// This executes test_align_trap.asm, which should do a single debug
// instruction from interrupt contexts, and then quit.

module simu07();

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
        .interrupt_request(4'h0));

    // The ROM got all the addresses
    rom07 rom07(
        .clk(clk), 
        .enable(1'b1), 
        .addr(addr[15:1]), 
        .data(dIn));

    integer i;

    initial
    begin
        $dumpfile("simu07.vcd");
        $dumpvars(0, simu07);
        for(i = 0; i<16; i=i+1)
            $dumpvars(0, cpu.registers[i]);
        #100;
        reset <= 1;
        #2000;
        $finish;
    end

endmodule

