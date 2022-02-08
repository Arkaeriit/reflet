// This rom contains the code compiles from software/test_jump.asm
// It is ment to ensure that the simulator and the rtl core have the same
// behavior

module rom7(input clk, input enable, input [14:0] addr, output [16-1:0] data);
    reg [16-1:0] data_reg;
    always @ (posedge clk)
        case(addr)
            15'h0 : data_reg <= 16'h5341;
            15'h1 : data_reg <= 16'h4D52;
            15'h2 : data_reg <= 16'h2D3C;
            15'h3 : data_reg <= 16'h2C3B;
            15'h4 : data_reg <= 16'h3D10;
            15'h5 : data_reg <= 16'h0000;
            15'h6 : data_reg <= 16'h3C12;
            15'h7 : data_reg <= 16'h4C12;
            15'h8 : data_reg <= 16'h3E4E;
            15'h9 : data_reg <= 16'h0023;
            15'hA : data_reg <= 16'h4C13;
            15'hB : data_reg <= 16'h4E08;
            15'hC : data_reg <= 16'h3CF0;
            15'hD : data_reg <= 16'h3D2B;
            15'hE : data_reg <= 16'h3E2C;
            15'hF : data_reg <= 16'h0000;
            15'h10 : data_reg <= 16'h0F00;
            15'h11 : data_reg <= 16'h0F0F;
            15'h12 : data_reg <= 16'h000F;
            15'h13 : data_reg <= 16'h000E;
            default : data_reg <= 0;
        endcase
    assign data = ( enable ? data_reg : 0 );
endmodule
