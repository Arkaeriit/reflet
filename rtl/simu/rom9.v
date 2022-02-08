/*
 * This rom is made from the proram test_inst_mini_str.asm.
 * It is meant to test some basic instructions.
 */
module rom9(input clk, input enable, input [7-1:0] addr, output [8-1:0] data);
    reg [8-1:0] data_reg;
    always @ (posedge clk)
        case(addr)
            7'h0 : data_reg <= 8'h41;
            7'h1 : data_reg <= 8'h53;
            7'h2 : data_reg <= 8'h52;
            7'h3 : data_reg <= 8'h4D;
            7'h4 : data_reg <= 8'h14;
            7'h5 : data_reg <= 8'h31;
            7'h6 : data_reg <= 8'h18;
            7'h7 : data_reg <= 8'hA1;
            7'h8 : data_reg <= 8'h32;
            7'h9 : data_reg <= 8'h33;
            7'hA : data_reg <= 8'h10;
            7'hB : data_reg <= 8'hE2;
            7'hC : data_reg <= 8'h11;
            7'hD : data_reg <= 8'h42;
            7'hE : data_reg <= 8'h32;
            7'hF : data_reg <= 8'h17;
            7'h10 : data_reg <= 8'hE2;
            7'h11 : data_reg <= 8'h11;
            7'h12 : data_reg <= 8'h42;
            7'h13 : data_reg <= 8'h32;
            7'h14 : data_reg <= 8'h14;
            7'h15 : data_reg <= 8'h3C;
            7'h16 : data_reg <= 8'h16;
            7'h17 : data_reg <= 8'hAC;
            7'h18 : data_reg <= 8'h3C;
            7'h19 : data_reg <= 8'h14;
            7'h1A : data_reg <= 8'h7C;
            7'h1B : data_reg <= 8'hE2;
            7'h1C : data_reg <= 8'h11;
            7'h1D : data_reg <= 8'h42;
            7'h1E : data_reg <= 8'h32;
            7'h1F : data_reg <= 8'hF3;
            7'h20 : data_reg <= 8'h0F;
            7'h21 : data_reg <= 8'h11;
            7'h22 : data_reg <= 8'h43;
            7'h23 : data_reg <= 8'h33;
            7'h24 : data_reg <= 8'hF3;
            7'h25 : data_reg <= 8'h0F;
            7'h26 : data_reg <= 8'h11;
            7'h27 : data_reg <= 8'h43;
            7'h28 : data_reg <= 8'h33;
            7'h29 : data_reg <= 8'hF3;
            7'h2A : data_reg <= 8'h0F;
            7'h2B : data_reg <= 8'h11;
            7'h2C : data_reg <= 8'h43;
            7'h2D : data_reg <= 8'h33;
            7'h2E : data_reg <= 8'h0E;
            default : data_reg <= 0;
        endcase
    assign data = ( enable ? data_reg : 0 );
endmodule
