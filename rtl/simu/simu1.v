

module simu1();

    reg clk = 0;
    always #1 clk = !clk;

    reg reset = 0;
    wire [7:0] dIn;
    wire [7:0] dOut;
    wire [7:0] addr;
    wire write_en;
    wire quit;
    
    asrm_cpu #(8) cpu(clk, reset, quit, dIn, addr, dOut, write_en);

    rom1 rom1(clk, addr[3:0], dIn);

    initial
    begin
        #10;
        reset = 1;
    end

    always @ (posedge clk)
        if(quit)
            reset = 0;

endmodule

