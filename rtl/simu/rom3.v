/*-----------------------------\
|This rom test the flow control|
\-----------------------------*/

/*
asrmmasp code:
wordsize 16

label multByTwo
pop
cpy R10
add R10
push
ret

label start
set+ 40000
cpy SP
set 10
push
setlab multByTwo
call
pop
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
read SP
add R1
quit
*/


module rom3(input clk, input enable_out,input [7:0] addr, output [7:0] dataOut);
reg [7:0] ret; assign dataOut = (enable_out ? ret : 8'h0);
always @ (posedge clk)
case(addr)
  0 : ret = 8'h41;
  1 : ret = 8'h53;
  2 : ret = 8'h52;
  3 : ret = 8'h4d;
  4 : ret = 8'h14;
  5 : ret = 8'h3c;
  6 : ret = 8'h10;
  7 : ret = 8'h3b;
  8 : ret = 8'h10;
  9 : ret = 8'h7b;
  10 : ret = 8'hac;
  11 : ret = 8'h3b;
  12 : ret = 8'h10;
  13 : ret = 8'h7b;
  14 : ret = 8'hac;
  15 : ret = 8'h3b;
  16 : ret = 8'h18;
  17 : ret = 8'h7b;
  18 : ret = 8'hac;
  19 : ret = 8'h3b;
  20 : ret = 8'h15;
  21 : ret = 8'h7b;
  22 : ret = 8'h3f;
  23 : ret = 8'h14;
  24 : ret = 8'h3c;
  25 : ret = 8'h10;
  26 : ret = 8'h3b;
  27 : ret = 8'h10;
  28 : ret = 8'h7b;
  29 : ret = 8'hac;
  30 : ret = 8'h3b;
  31 : ret = 8'h10;
  32 : ret = 8'h7b;
  33 : ret = 8'hac;
  34 : ret = 8'h3b;
  35 : ret = 8'h12;
  36 : ret = 8'h7b;
  37 : ret = 8'hac;
  38 : ret = 8'h3b;
  39 : ret = 8'h1f;
  40 : ret = 8'h7b;
  41 : ret = 8'h8;
  42 : ret = 8'ha;
  43 : ret = 8'h3a;
  44 : ret = 8'h4a;
  45 : ret = 8'hb;
  46 : ret = 8'hd;
  47 : ret = 8'h14;
  48 : ret = 8'h3c;
  49 : ret = 8'h10;
  50 : ret = 8'h3b;
  51 : ret = 8'h19;
  52 : ret = 8'h7b;
  53 : ret = 8'hac;
  54 : ret = 8'h3b;
  55 : ret = 8'h1c;
  56 : ret = 8'h7b;
  57 : ret = 8'hac;
  58 : ret = 8'h3b;
  59 : ret = 8'h14;
  60 : ret = 8'h7b;
  61 : ret = 8'hac;
  62 : ret = 8'h3b;
  63 : ret = 8'h10;
  64 : ret = 8'h7b;
  65 : ret = 8'h3f;
  66 : ret = 8'h1a;
  67 : ret = 8'hb;
  68 : ret = 8'h14;
  69 : ret = 8'h3c;
  70 : ret = 8'h10;
  71 : ret = 8'h3b;
  72 : ret = 8'h10;
  73 : ret = 8'h7b;
  74 : ret = 8'hac;
  75 : ret = 8'h3b;
  76 : ret = 8'h10;
  77 : ret = 8'h7b;
  78 : ret = 8'hac;
  79 : ret = 8'h3b;
  80 : ret = 8'h12;
  81 : ret = 8'h7b;
  82 : ret = 8'hac;
  83 : ret = 8'h3b;
  84 : ret = 8'h1a;
  85 : ret = 8'h7b;
  86 : ret = 8'hc;
  87 : ret = 8'ha;
  88 : ret = 8'h31;
  89 : ret = 8'h14;
  90 : ret = 8'h3c;
  91 : ret = 8'h10;
  92 : ret = 8'h3b;
  93 : ret = 8'h10;
  94 : ret = 8'h7b;
  95 : ret = 8'hac;
  96 : ret = 8'h3b;
  97 : ret = 8'h10;
  98 : ret = 8'h7b;
  99 : ret = 8'hac;
  100 : ret = 8'h3b;
  101 : ret = 8'h11;
  102 : ret = 8'h7b;
  103 : ret = 8'hac;
  104 : ret = 8'h3b;
  105 : ret = 8'h14;
  106 : ret = 8'h7b;
  107 : ret = 8'hc1;
  108 : ret = 8'h14;
  109 : ret = 8'h3c;
  110 : ret = 8'h10;
  111 : ret = 8'h3b;
  112 : ret = 8'h10;
  113 : ret = 8'h7b;
  114 : ret = 8'hac;
  115 : ret = 8'h3b;
  116 : ret = 8'h10;
  117 : ret = 8'h7b;
  118 : ret = 8'hac;
  119 : ret = 8'h3b;
  120 : ret = 8'h18;
  121 : ret = 8'h7b;
  122 : ret = 8'hac;
  123 : ret = 8'h3b;
  124 : ret = 8'h13;
  125 : ret = 8'h7b;
  126 : ret = 8'h9;
  127 : ret = 8'h0;
  128 : ret = 8'h0;
  129 : ret = 8'h0;
  130 : ret = 8'h0;
  131 : ret = 8'h2f;
  132 : ret = 8'h41;
  default: ret = 0;
endcase
endmodule
