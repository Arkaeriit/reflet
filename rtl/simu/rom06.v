module rom06(input clk, input enable, input [30-1:0] addr, output [32-1:0] data);
    reg [32-1:0] data_reg;
    always @ (posedge clk)
        case(addr)
            30'h0 : data_reg <= 32'h4D525341;
            30'h1 : data_reg <= 32'h9C0D1C28;
            30'h2 : data_reg <= 32'h221D6D8C;
            30'h3 : data_reg <= 32'h0D1D5D8C;
            30'h4 : data_reg <= 32'h0D1C281B;
            30'h5 : data_reg <= 32'h1D6D8C9C;
            30'h6 : data_reg <= 32'h1D5D8C22;
            30'h7 : data_reg <= 32'h3C221C22;
            30'h8 : data_reg <= 32'h10001E3E;
            30'h9 : data_reg <= 32'h3EE53C23;
            30'hA : data_reg <= 32'h1D0B1CD0;
            30'hB : data_reg <= 32'h1B0D1F0C;
            30'hC : data_reg <= 32'h9C0D1C28;
            30'hD : data_reg <= 32'h221D6D8C;
            30'hE : data_reg <= 32'h221D5D8C;
            30'hF : data_reg <= 32'h3C22001C;
            30'h10 : data_reg <= 32'h00551E3E;
            30'h11 : data_reg <= 32'h3EE53C23;
            30'h12 : data_reg <= 32'h1D0B1CD0;
            30'h13 : data_reg <= 32'h21E41E0C;
            30'h14 : data_reg <= 32'hE319E939;
            30'h15 : data_reg <= 32'h0D1920EB;
            30'h16 : data_reg <= 32'h0D1C281B;
            30'h17 : data_reg <= 32'h1D6D8C9C;
            30'h18 : data_reg <= 32'h1D5D8C22;
            30'h19 : data_reg <= 32'h3C221C22;
            30'h1A : data_reg <= 32'h004E1E3E;
            30'h1B : data_reg <= 32'h3EE53C23;
            30'h1C : data_reg <= 32'h1D0B1CD0;
            30'h1D : data_reg <= 32'h1C24ED0C;
            30'h1E : data_reg <= 32'h241C8C20;
            30'h1F : data_reg <= 32'hB9221D5C;
            30'h20 : data_reg <= 32'h281B0DE6;
            30'h21 : data_reg <= 32'h8C9C0D1C;
            30'h22 : data_reg <= 32'h8C221D6D;
            30'h23 : data_reg <= 32'h1C221D5D;
            30'h24 : data_reg <= 32'h1E3E3C22;
            30'h25 : data_reg <= 32'h3C23007E;
            30'h26 : data_reg <= 32'h1CD03EE5;
            30'h27 : data_reg <= 32'hE00C1D0B;
            30'h28 : data_reg <= 32'h000000E8;
            default : data_reg <= 0;
        endcase
    assign data = ( enable ? data_reg : 0 );
endmodule
