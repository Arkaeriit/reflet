/*-----------------------------\
|This rom test the flow control|
\-----------------------------*/

/*
asrmmasp code:
wordsize 16

label multByTwo
read R8
cpy R10
add R10
cpy R8
ret

label start
set+ 40000
cpy SP
set 10
cpy R8
setlab multByTwo
call
read R8
cpy R1
set+ 20
eq R1
setlab nosleep
jif 
    slp
    slp
    slp
    slp
label nosleep
quit
*/


module rom3(input clk, input enable_out,input [7:0] addr, output [7:0] dataOut);
reg [7:0] ret; assign dataOut = (enable_out ? ret : 8'h0);
always @ (posedge clk)
case(addr)
  8'h0 : ret = 8'h41;  //assrmmasm runtime
  8'h1 : ret = 8'h53;
  8'h2 : ret = 8'h52;
  8'h3 : ret = 8'h4d;
  8'h4 : ret = 8'h14;
  8'h5 : ret = 8'h3c;
  8'h6 : ret = 8'h10;
  8'h7 : ret = 8'h3b;
  8'h8 : ret = 8'h10;
  8'h9 : ret = 8'h7b;
  8'ha : ret = 8'hac;
  8'hb : ret = 8'h3b;
  8'hc : ret = 8'h10;
  8'hd : ret = 8'h7b;
  8'he : ret = 8'hac;
  8'hf : ret = 8'h3b;
  8'h10 : ret = 8'h18;
  8'h11 : ret = 8'h7b;
  8'h12 : ret = 8'hac;
  8'h13 : ret = 8'h3b;
  8'h14 : ret = 8'h15;
  8'h15 : ret = 8'h7b;
  8'h16 : ret = 8'h3f;
  8'h17 : ret = 8'h14;
  8'h18 : ret = 8'h3c;
  8'h19 : ret = 8'h10;
  8'h1a : ret = 8'h3b;
  8'h1b : ret = 8'h10;
  8'h1c : ret = 8'h7b;
  8'h1d : ret = 8'hac;
  8'h1e : ret = 8'h3b;
  8'h1f : ret = 8'h10;
  8'h20 : ret = 8'h7b;
  8'h21 : ret = 8'hac;
  8'h22 : ret = 8'h3b;
  8'h23 : ret = 8'h12;
  8'h24 : ret = 8'h7b;
  8'h25 : ret = 8'hac;
  8'h26 : ret = 8'h3b;
  8'h27 : ret = 8'h1f;
  8'h28 : ret = 8'h7b;
  8'h29 : ret = 8'h8;
  8'h2a : ret = 8'h28; //mult by two : read R8
  8'h2b : ret = 8'h3a; //cpy R10
  8'h2c : ret = 8'h4a; //add R10
  8'h2d : ret = 8'h38; //cpy R8
  8'h2e : ret = 8'hd;  //ret
  8'h2f : ret = 8'h14; //start : set+ 40000
  8'h30 : ret = 8'h3c;
  8'h31 : ret = 8'h10;
  8'h32 : ret = 8'h3b;
  8'h33 : ret = 8'h19;
  8'h34 : ret = 8'h7b;
  8'h35 : ret = 8'hac;
  8'h36 : ret = 8'h3b;
  8'h37 : ret = 8'h1c;
  8'h38 : ret = 8'h7b;
  8'h39 : ret = 8'hac;
  8'h3a : ret = 8'h3b;
  8'h3b : ret = 8'h14;
  8'h3c : ret = 8'h7b;
  8'h3d : ret = 8'hac;
  8'h3e : ret = 8'h3b;
  8'h3f : ret = 8'h10;
  8'h40 : ret = 8'h7b;
  8'h41 : ret = 8'h3f; //cpy SP
  8'h42 : ret = 8'h1a; //set 10
  8'h43 : ret = 8'h38; //cpy R8
  8'h44 : ret = 8'h14; //setlab multByTwo
  8'h45 : ret = 8'h3c;
  8'h46 : ret = 8'h10;
  8'h47 : ret = 8'h3b;
  8'h48 : ret = 8'h10;
  8'h49 : ret = 8'h7b;
  8'h4a : ret = 8'hac;
  8'h4b : ret = 8'h3b;
  8'h4c : ret = 8'h10;
  8'h4d : ret = 8'h7b;
  8'h4e : ret = 8'hac;
  8'h4f : ret = 8'h3b;
  8'h50 : ret = 8'h12;
  8'h51 : ret = 8'h7b;
  8'h52 : ret = 8'hac;
  8'h53 : ret = 8'h3b;
  8'h54 : ret = 8'h1a;
  8'h55 : ret = 8'h7b;
  8'h56 : ret = 8'hc;  //call
  8'h57 : ret = 8'h28; //read R8
  8'h58 : ret = 8'h31; //cpy R1
  8'h59 : ret = 8'h14; //set+ 20
  8'h5a : ret = 8'h3c;
  8'h5b : ret = 8'h10;
  8'h5c : ret = 8'h3b;
  8'h5d : ret = 8'h10;
  8'h5e : ret = 8'h7b;
  8'h5f : ret = 8'hac;
  8'h60 : ret = 8'h3b;
  8'h61 : ret = 8'h10;
  8'h62 : ret = 8'h7b;
  8'h63 : ret = 8'hac;
  8'h64 : ret = 8'h3b;
  8'h65 : ret = 8'h11;
  8'h66 : ret = 8'h7b;
  8'h67 : ret = 8'hac;
  8'h68 : ret = 8'h3b;
  8'h69 : ret = 8'h14;
  8'h6a : ret = 8'h7b;
  8'h6b : ret = 8'hc1; //eq R1
  8'h6c : ret = 8'h14; //setlab nosleep
  8'h6d : ret = 8'h3c;
  8'h6e : ret = 8'h10;
  8'h6f : ret = 8'h3b;
  8'h70 : ret = 8'h10;
  8'h71 : ret = 8'h7b;
  8'h72 : ret = 8'hac;
  8'h73 : ret = 8'h3b;
  8'h74 : ret = 8'h10;
  8'h75 : ret = 8'h7b;
  8'h76 : ret = 8'hac;
  8'h77 : ret = 8'h3b;
  8'h78 : ret = 8'h18;
  8'h79 : ret = 8'h7b;
  8'h7a : ret = 8'hac;
  8'h7b : ret = 8'h3b;
  8'h7c : ret = 8'h13;
  8'h7d : ret = 8'h7b;
  8'h7e : ret = 8'h9;  //jif
  8'h7f : ret = 8'h0;  //slp
  8'h80 : ret = 8'h0;  //slp
  8'h81 : ret = 8'h0;  //slp
  8'h82 : ret = 8'h0;  //slp
  8'h83 : ret = 8'he;  //label nosleep : quit
  8'h84 : ret = 8'h0;
  default: ret = 0;
endcase
endmodule
