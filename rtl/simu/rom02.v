/*------------------------------------\
|This ROM test the instruction used in|
|The interface with the RAM.          |
\------------------------------------*/

module rom2(input clk, input enable_out,input [4:0] addr, output [7:0] out);
reg [7:0] ret = 0; 
assign out = (enable_out ? ret : 5'h0);
always @ (posedge clk)
case(addr)
  0 : ret = 65;   //A
  1 : ret = 83;   //S
  2 : ret = 82;   //R
  3 : ret = 77;   //M
  4 : ret = 20;   //set 4
  5 : ret = 60;   //cpy R12
  6 : ret = 24;   //set 8
  7 : ret = 172;  //lsl R12
  8 : ret = 63;   //cpy SP
  9 : ret = 21;   //set 5
  10 : ret = 11;  //push
  11 : ret = 16;  //set 0
  12 : ret = 10;  //pop
  13 : ret = 16;  //set 0
  14 : ret = 47;  //read SP
  15 : ret = 76;  //add R12
  16 : ret = 59;  //cpy R11
  17 : ret = 25;  //set 9
  18 : ret = 235; //str R11
  19 : ret = 16;  //set 0
  20 : ret = 251; //load R11
  21 : ret = 14;  //quit
  default: ret = 0;
endcase
endmodule
