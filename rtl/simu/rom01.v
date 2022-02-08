/*-------------------------\
|This ROM test the basic   |
|functionalities of the ALU|
\-------------------------*/

module rom1(input clk, input [3:0] addr, output reg [7:0] out = 0);
always @ (posedge clk)
    case(addr)
      0 : out = 65;     //A
      1 : out = 83;     //S
      2 : out = 82;     //R
      3 : out = 77;     //M
      4 : out = 20;     //set 4
      5 : out = 0;      //slp
      6 : out = 0;      //slp
      7 : out = 0;      //slp
      8 : out = 53;     //cpy R5
      9 : out = 17;     //set 1
      10 : out = 69;    //add R5
      11 : out = 54;    //cpy R6
      12 : out = 85;    //sub R5
      13 : out = 150;   //not R6
      14 : out = 8'h0E; //quit
      default: out = 0;
    endcase
endmodule
