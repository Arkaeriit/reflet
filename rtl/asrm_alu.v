/*----------------------------------------------\
|This module contain the ALU. As of now, it only|
|does basic opperation but flags will come soon.|
\----------------------------------------------*/

`include "asrm.vh"

module asrm_alu
    #(parameter wordsize = 16
    )(
    input [wordsize-1:0] working_register,
    input [wordsize-1:0] other_register,
    input [wordsize-1:0] status_register,
    input [7:0] instruction,
    output [wordsize-1:0] out,
    output [3:0] out_reg
    );

    wire [3:0] opperand = instruction[7:4];
    wire [wordsize-1:0] defaultValue = 0;

    //Math opperations
    wire [wordsize-1:0] add_out = ( opperand == `opp_add ? working_register + other_register : defaultValue );
    wire [wordsize-1:0] sub_out = ( opperand == `opp_sub ? working_register - other_register : defaultValue );
    //Logical opperations
    wire [wordsize-1:0] and_out = ( opperand == `opp_and ? working_register & other_register : defaultValue );
    wire [wordsize-1:0] or_out  = ( opperand == `opp_or  ? working_register | other_register : defaultValue );
    wire [wordsize-1:0] xor_out = ( opperand == `opp_xor ? working_register ^ other_register : defaultValue );
    wire [wordsize-1:0] not_out = ( opperand == `opp_not ? !other_register : defaultValue );
    wire [wordsize-1:0] lsl_out = ( opperand == `opp_lsl ? defaultValue : defaultValue ); //todo
    wire [wordsize-1:0] lsr_out = ( opperand == `opp_lsr ? defaultValue : defaultValue ); //todo
    //sleep, we want to put wr into wr
    wire [wordsize-1:0] out_slp = ( instruction == `inst_slp ? working_register : defaultValue );
    wire [wordsize-1:0] out_wr = add_out | sub_out | and_out | or_out | xor_out | not_out | lsl_out | lsr_out | out_slp;

    //comparaisons
    wire cmp_eq  = ( opperand == `opp_eq ? working_register == other_register : defaultValue );
    wire cmp_les = ( opperand == `opp_les ? working_register <  other_register : defaultValue );
    wire cmp = cmp_eq | cmp_les;
    wire [wordsize-1:0] out_cmp = {status_register[wordsize-1:1], cmp};

    //wireing out and out_reg
    //We want to see if we are trying to change the working register or
    //the status register
    wire cmp_opp = (opperand == `opp_eq) || (opperand == `opp_les);
    assign out = (cmp_opp ? out_cmp : out_wr);
    assign out_reg = (cmp_opp ? `sr_id : 4'h0);

endmodule

