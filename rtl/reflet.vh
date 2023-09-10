/*-------------------------------------*\
|This verilog header contain constants  |
|used to identify register, instructions|
|and opperands.                         |
\--------------------------------------*/

`ifndef reflet_vh
`define reflet_vf

//Register index id
`define wr_id    4'h0 /*working register*/
`define sr_id    4'hD /*Status register*/
`define pc_id    4'hE /*Program counter*/
`define sp_id    4'hF /*Stack pointer*/
`define gp_start 4'h1 /*general purpuse registers*/
`define gp_end   4'hC

//Registers reset value
`define wr_reset 0 /*working register*/
`define sr_reset 0 /*Status register*/
`define pc_reset 4 /*Program counter*/
`define sp_reset 0 /*Stack pointer*/
`define gp_reset 0 /*general purpuse registers*/

//Opperands
`define opp_read   4'h0
`define opp_cpy    4'h1
`define opp_set    4'h2
`define opp_add    4'h3
`define opp_and    4'h4
`define opp_or     4'h5
`define opp_xor    4'h6
`define opp_not    4'h7
`define opp_lsl    4'h8
`define opp_lsr    4'h9
`define opp_eq     4'hA
`define opp_les    4'hB
`define opp_str    4'hC
`define opp_load   4'hF

`define opp_setint      6'b111011
`define opp_getint      6'b111100
`define opp_getintstack 6'b111101
`define opp_setintstack 6'b111110
`define opp_softint     6'b111111

//Instructions
`define inst_jif    8'hE0
`define inst_call   8'hE1
`define inst_ret    8'hE2
`define inst_pop    8'hE3
`define inst_push   8'hE4
`define inst_cc2    8'hE5
`define inst_cmpnot 8'hE6
`define inst_tbm    8'hE7
`define inst_quit   8'hE8
`define inst_debug  8'hE9
`define inst_atom   8'hEA
`define inst_retint 8'hEB

`endif

