module rom1(input clk, input [3:0] addr, output reg [7:0] out = 0);
always @ (posedge clk)
    case(addr)
      0 : out = 65;
      1 : out = 83;
      2 : out = 82;
      3 : out = 77;
      4 : out = 20;
      5 : out = 0;
      6 : out = 0;
      7 : out = 0;
      8 : out = 53;
      9 : out = 17;
      10 : out = 69;
      11 : out = 54;
      12 : out = 85;
      13 : out = 150;
      default: out = 0;
    endcase
endmodule
