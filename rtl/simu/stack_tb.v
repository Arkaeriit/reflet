/*--------------------------\
|This module is ment to test|
|reflet_stack.                |
\--------------------------*/

module stack_tb();

    reg clk = 0;
    always #1 clk = !clk;
    reg reset = 0;

    reg push = 0;
    reg pop = 0;
    reg [7:0] in = 0;
    wire [7:0] out;

    reflet_stack #(.wordsize(8), .depth(10)) stack(
        .clk(clk),
        .reset(reset),
        .push(push),
        .in(in),
        .pop(pop),
        .out(out));

    initial
    begin
        #10;
        reset = 1;
        in = 8'hAA;
        #10;
        push = 1;
        #2;
        push = 0;
        #2;
        pop = 1;
        #2;
        pop = 0;
        in = 8'hBB;
        push = 1;
        #4;
        push = 0;
        #10;
        pop = 1;
        #2;
        pop = 0;
        #2;
        pop = 1;
        #2;
        pop = 0;
    end

endmodule

