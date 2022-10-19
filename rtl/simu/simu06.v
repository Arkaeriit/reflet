
module simu06();

    reg clk = 1;
    always #1 clk <= !clk;

    reg reset = 0;
    wire [15:0] dIn;
    wire [15:0] dOut;
    wire [15:0] addr;
    wire write_en;
    wire quit;
    wire debug;
    
    
    reflet_cpu #(.wordsize(16)) cpu(
        .clk(clk), 
        .reset(reset), 
        .debug(debug),
        .enable(1'b1),
        .quit(quit), 
        .data_in(dIn), 
        .addr(addr), 
        .data_out(dOut), 
        .write_en(write_en),
        .interrupt_request(4'h0));

    //The rom
    rom6 rom6(
        .clk(clk), 
        .enable_out(!addr[15]), 
        .addr(addr[4:0]), 
        .dataOut(dIn));

    integer i;

    initial
    begin
        $dumpfile("simu06.vcd");
        $dumpvars(0, simu06);
        for(i = 0; i<16; i=i+1)
            $dumpvars(0, cpu.registers[i]);
        #10;
        reset <= 1;
        #200;
        $finish;
    end

    always @ (posedge clk)
        if(quit)
            reset = 0;


endmodule

