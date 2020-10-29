module rom2(input clk, input enable_out,input [4:0] addr, output [7:0] out);
reg [7:0] ret = 0; 
assign out = (enable_out ? ret : 5'h0);
always @ (posedge clk)
case(addr)
  0 : ret = 65;
  1 : ret = 83;
  2 : ret = 82;
  3 : ret = 77;
  4 : ret = 20;
  5 : ret = 60;
  6 : ret = 24;
  7 : ret = 172;
  8 : ret = 63;
  9 : ret = 21;
  10 : ret = 11;
  11 : ret = 16;
  12 : ret = 10;
  13 : ret = 16;
  14 : ret = 47;
  15 : ret = 76;
  16 : ret = 59;
  17 : ret = 25;
  18 : ret = 235;
  19 : ret = 16;
  20 : ret = 251;
  21 : ret = 14;
  default: ret = 0;
endcase
endmodule
