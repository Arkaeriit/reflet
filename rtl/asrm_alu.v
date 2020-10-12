/*----------------------------------------------\
|This module contain the ALU. As of now, it only|
|does basic opperation but flags will come soon.|
\----------------------------------------------*/

module asrm_alu
    #(parameter wordsize = 16
    )(
    //input clk,
    //input reset,
    input [wordsize-1:0] working_register,
    input [wordsize-1:0] other_register,
    input [3:0] opperand,
    output [wordsize-1:0] out,
    output cmp
    );

    //Math opperations
    wire [wordsize-1:0] add_out = ( opperand == opp_add ? working_register + other_register : 0 );
    wire [wordsize-1:0] sub_out = ( opperand == opp_sub ? working_register - other_register : 0 );
    //Logical opperations
    wire [wordsize-1:0] and_out = ( opperand == opp_and ? working_register & other_register : 0 );
    wire [wordsize-1:0] or_out  = ( opperand == opp_or  ? working_register | other_register : 0 );
    wire [wordsize-1:0] xor_out = ( opperand == opp_xor ? working_register ^ other_register : 0 );
    wire [wordsize-1:0] not_out = ( opperand == opp_not ? !other_register : 0 );
    wire [wordsize-1:0] lsl_out = ( opperand == opp_lsl ? 0 : 0 ); //todo
    wire [wordsize-1:0] lsr_out = ( opperand == opp_lsr ? 0 : 0 ); //todo
    assign out = add_out | sub_out | and_out | or_out | xor_out | not_out | lsl_out | lsr_out;

    //comparaisons
    wire cmp_eq  = ( opperand == opp_eq ? working_register == other_register : 0 );
    wire cmp_les = ( opperand == opp_eq ? working_register <  other_register : 0 );
    assign cmp = cmp_eq | cmp_les;

endmodule

