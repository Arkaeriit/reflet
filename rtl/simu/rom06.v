/*---------------------\
|This rom test the     |
|misaligned acces trap.|
\---------------------*/

module rom6_byte_access(input clk, input enable_out,input [4:0] addr, output [7:0] dataOut);
reg [7:0] ret; assign dataOut = (enable_out ? ret : 8'h0);
always @ (posedge clk)
case(addr)
  5'h00 : ret = 8'h0F;  //debug; start of the interupt handler
  5'h01 : ret = 8'h18;  //set 8
  5'h02 : ret = 8'h3D;  //cpy SR; Disabble alignement trap
  5'h03 : ret = 8'h02;  //retint
  5'h04 : ret = 8'h10;  //set 0
  5'h05 : ret = 8'h04;  //setint 0
  5'h06 : ret = 8'h14;  //set 4
  5'h07 : ret = 8'h31;  //cpy R1
  5'h08 : ret = 8'h18;  //set 8
  5'h09 : ret = 8'h32;  //cpy R2
  5'h0A : ret = 8'hA1;  //lsl R1
  5'h0B : ret = 8'h72;  //or R2
  5'h0C : ret = 8'h3D;  //cpy SR; The status register is set to 0x88 which mean that the interupt 1 is enabled and the misaligned trap is enabled
  5'h0D : ret = 8'h00;  //slp
  5'h0E : ret = 8'h00;  //slp
  5'h0F : ret = 8'h11;  //set 1
  5'h10 : ret = 8'hF0;  //load wr; an interupt should be raised, then the interupt manager will disable the trap 
  5'h11 : ret = 8'h00;  //slp
  5'h12 : ret = 8'h11;  //set 1
  5'h13 : ret = 8'hF0;  //load wr; no interupt expected
  5'h14 : ret = 8'h00;  //slp
  5'h15 : ret = 8'h0E;  //quit
  default: ret = 0;
endcase
endmodule

module rom6(input clk, input enable_out,input [4:0] addr, output [15:0] dataOut);
    wire [7:0] data_even;
    wire [7:0] data_odd;
    rom6_byte_access rom_even(clk, enable_out, addr, data_even);
    rom6_byte_access rom_odd(clk, enable_out, addr+5'b1, data_odd);
    assign dataOut = {data_odd, data_even};
endmodule

