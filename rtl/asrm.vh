/*-------------------------------------*\
|This verilog header contain constants  |
|used to identify register, instructions|
|and opperands.                         |
\--------------------------------------*/

`ifndef asrm_vh
`define asrm_vf

 //Register index id
`define wr_id 4'h0 /*working register*/
`define sr_id 4'hD /*Status register*/
`define pc_id 4'hE /*Program counter*/
`define sp_id 4'hF /*Stack pointer*/
`define gp_start 4'h1 /*general purpuse registers*/
`define gp_end 4'hC

//Registers reset value
`define wr_reset 0 /*working register*/
`define sr_reset 1 /*Status register*/
`define pc_reset 4 /*Program counter*/
`define sp_reset 0 /*Stack pointer*/
`define gp_reset 0 /*general purpuse registers*/

//Opperands
`define opp_set  4'h1
`define opp_read 4'h2
`define opp_cpy  4'h3
`define opp_add  4'h4
`define opp_sub  4'h5
`define opp_and  4'h6
`define opp_or   4'h7
`define opp_xor  4'h8
`define opp_not  4'h9
`define opp_lsl  4'hA
`define opp_lsr  4'hB
`define opp_eq   4'hC
`define opp_les  4'hD
`define opp_str  4'hE
`define opp_load 4'hF

//Instructions
`define inst_slp  8'h00
`define inst_jmp  8'h08
`define inst_jif  8'h09
`define inst_pop  8'h0A
`define inst_push 8'h0B
`define inst_call 8'h0C
`define inst_ret  8'h0D
`define inst_quit 8'h0E

`endif

