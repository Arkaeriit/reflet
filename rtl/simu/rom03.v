module rom03(input clk, input enable, input [15-1:0] addr, output [16-1:0] data);
    reg [16-1:0] data_reg;
    always @ (posedge clk)
        case(addr)
            15'h0 : data_reg <= 16'h5341;
            15'h1 : data_reg <= 16'h4D52;
            15'h2 : data_reg <= 16'h1C28;
            15'h3 : data_reg <= 16'h8C22;
            15'h4 : data_reg <= 16'h1C1D;
            15'h5 : data_reg <= 16'h1B0D;
            15'h6 : data_reg <= 16'h280C;
            15'h7 : data_reg <= 16'h221C;
            15'h8 : data_reg <= 16'h1D8C;
            15'h9 : data_reg <= 16'h221D;
            15'hA : data_reg <= 16'h001C;
            15'hB : data_reg <= 16'h3C22;
            15'hC : data_reg <= 16'h1E3E;
            15'hD : data_reg <= 16'h002B;
            15'hE : data_reg <= 16'h3C23;
            15'hF : data_reg <= 16'h3EE5;
            15'h10 : data_reg <= 16'h1CD0;
            15'h11 : data_reg <= 16'h1D0B;
            15'h12 : data_reg <= 16'h1E0C;
            15'h13 : data_reg <= 16'h0000;
            15'h14 : data_reg <= 16'hE900;
            15'h15 : data_reg <= 16'hE9E9;
            15'h16 : data_reg <= 16'h00E9;
            15'h17 : data_reg <= 16'h00E8;
            default : data_reg <= 0;
        endcase
    assign data = ( enable ? data_reg : 0 );
endmodule
