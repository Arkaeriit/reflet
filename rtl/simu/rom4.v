/*--------------------------\
|This rom test the effect of|
|the activation of the      |
|reduced behavior bits.     |
\--------------------------*/

module rom4(input clk, input enable_out,input [6:0] addr, output [7:0] dataOut);
reg [7:0] ret; assign dataOut = (enable_out ? ret : 7'h0);
always @ (posedge clk)
case(addr)
  7'h0 : ret = 8'h41;  //reflet-masm runtime
  7'h1 : ret = 8'h53;
  7'h2 : ret = 8'h52;
  7'h3 : ret = 8'h4d;
  7'h4 : ret = 8'h14;
  7'h5 : ret = 8'h3c;
  7'h6 : ret = 8'h10;
  7'h7 : ret = 8'h3b;
  7'h8 : ret = 8'h10;
  7'h9 : ret = 8'h7b;
  7'ha : ret = 8'hac;
  7'hb : ret = 8'h3b;
  7'hc : ret = 8'h10;
  7'hd : ret = 8'h7b;
  7'he : ret = 8'hac;
  7'hf : ret = 8'h3b;
  7'h10 : ret = 8'h15;
  7'h11 : ret = 8'h7b;
  7'h12 : ret = 8'hac;
  7'h13 : ret = 8'h3b;
  7'h14 : ret = 8'h1b;
  7'h15 : ret = 8'h7b;
  7'h16 : ret = 8'h3f;
  7'h17 : ret = 8'h14;
  7'h18 : ret = 8'h3c;
  7'h19 : ret = 8'h10;
  7'h1a : ret = 8'h3b;
  7'h1b : ret = 8'h10;
  7'h1c : ret = 8'h7b;
  7'h1d : ret = 8'hac;
  7'h1e : ret = 8'h3b;
  7'h1f : ret = 8'h10;
  7'h20 : ret = 8'h7b;
  7'h21 : ret = 8'hac;
  7'h22 : ret = 8'h3b;
  7'h23 : ret = 8'h12;
  7'h24 : ret = 8'h7b;
  7'h25 : ret = 8'hac;
  7'h26 : ret = 8'h3b;
  7'h27 : ret = 8'h1a;
  7'h28 : ret = 8'h7b;
  7'h29 : ret = 8'h3E;  
  7'h2a : ret = 8'h14; //label start : set+ 40000
  7'h2b : ret = 8'h3c;
  7'h2c : ret = 8'h10;
  7'h2d : ret = 8'h3b;
  7'h2e : ret = 8'h19;
  7'h2f : ret = 8'h7b;
  7'h30 : ret = 8'hac;
  7'h31 : ret = 8'h3b;
  7'h32 : ret = 8'h1c;
  7'h33 : ret = 8'h7b;
  7'h34 : ret = 8'hac;
  7'h35 : ret = 8'h3b;
  7'h36 : ret = 8'h14;
  7'h37 : ret = 8'h7b;
  7'h38 : ret = 8'hac;
  7'h39 : ret = 8'h3b;
  7'h3a : ret = 8'h10;
  7'h3b : ret = 8'h7b;
  7'h3c : ret = 8'h3f;
  7'h3d : ret = 8'h14; //cpy SP
  7'h3e : ret = 8'h3c; //set+ 43981 ;0xABCD
  7'h3f : ret = 8'h10;
  7'h40 : ret = 8'h3b;
  7'h41 : ret = 8'h1a;
  7'h42 : ret = 8'h7b;
  7'h43 : ret = 8'hac;
  7'h44 : ret = 8'h3b;
  7'h45 : ret = 8'h1b;
  7'h46 : ret = 8'h7b;
  7'h47 : ret = 8'hac;
  7'h48 : ret = 8'h3b;
  7'h49 : ret = 8'h1c;
  7'h4a : ret = 8'h7b;
  7'h4b : ret = 8'hac;
  7'h4c : ret = 8'h3b;
  7'h4d : ret = 8'h1d;
  7'h4e : ret = 8'h7b;
  7'h4f : ret = 8'hb;  //push
  7'h50 : ret = 8'h16; //set 6
  7'h51 : ret = 8'h3d; //cpy SR
  7'h52 : ret = 8'h10; //set 0
  7'h53 : ret = 8'ha;  //pop
  7'h54 : ret = 8'h13; //set 3
  7'h55 : ret = 8'hb;  //push
  7'h56 : ret = 8'h10; //set 0
  7'h57 : ret = 8'h3d; //cpy SR
  7'h58 : ret = 8'ha;  //pop
  7'h59 : ret = 8'h0;  //slp
  7'h5a : ret = 8'he;  //quit
  default: ret = 0;
endcase
endmodule
