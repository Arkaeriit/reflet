
module simu1();

    reg clk = 0;
    always #1 clk = !clk;

    reg reset = 0;
    wire [7:0] dIn;
    wire [7:0] dOut;
    wire [7:0] addr;
    wire write_en;
    wire quit;
    
    reflet_cpu #(.wordsize(8)) cpu(
        .clk(clk), 
        .reset(reset), 
        .quit(quit), 
        .data_in(dIn), 
        .addr(addr), 
        .data_out(dOut), 
        .write_en(write_en),
        .ext_int(4'h0));

    rom1 rom1(.clk(clk), .addr(addr[3:0]), .out(dIn));

    initial
    begin
        $dumpfile("simu1.vcd");
        $dumpvars();
        #10;
        reset = 1;
        #100;
        $finish;
    end

    always @ (posedge clk)
        if(quit)
            reset = 0;

endmodule

