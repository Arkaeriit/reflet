/*--------------------------------------------\
|This module is ment to offload from asrm_addr|
|the task of changing how the ram is handeled |
|depending on the reduced behavior bits.      |
\--------------------------------------------*/

module asrm_addr_reduced_behavior #(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input fetching_instruction,
    input [7:0] instruction,
    input [1:0] reduced_behavior_bits,
    output [5:0] pop_offset,
    //ram connection
    input [wordsize-1:0] data_in,
    output [wordsize-1:0] data_out,
    //asrm_addr connection
    input [wordsize-1:0] data_out_cpu,
    output [wordsize-1:0] data_in_cpu
    );

    //handling data_in
    reg [127:0] data_in_buff; //Register to store the value we fetched
    wire [127:0] data_in_wide = data_in;
    wire reduced_behavior = (reduced_behavior_bits != 2'b00) && ( (reduced_behavior_bits == 2'b01 && wordsize > 32) || (reduced_behavior_bits == 2'b10 && wordsize > 16) || (reduced_behavior_bits == 2'b11 && wordsize > 8) );
    always @ (posedge clk)
        if(!reset)
            data_in_buff = 0;
        else
            if(/*returning_value &*/ !fetching_instruction)
            begin
                if(reduced_behavior)
                    case(reduced_behavior_bits)
                        2'b01 : 
                        begin
                            data_in_buff[31:0] = data_in_wide[31:0];
                            data_in_buff[127:32] = 0;
                        end
                        2'b10 : 
                        begin
                            data_in_buff[15:0] = data_in_wide[15:0];
                            data_in_buff[127:16] = 0;
                        end
                        2'b11 : 
                        begin
                            data_in_buff[7:0] = data_in_wide[7:0];
                            data_in_buff[127:8] = 0;
                        end
                    endcase
                else //normal behavior
                    data_in_buff[wordsize-1:0] = data_in_wide;
            end

    assign data_in_raw = data_in;
    assign data_in_reduced = data_in_buff[wordsize-1:0];

    //handling data_out
    wire [127:0] data_out_wide = data_out_cpu;
    reg [127:0] data_out_buff;
    always @ (posedge clk)
        if(!reset)
            data_out_buff = 0;
        else
            if(/*returning_value &*/ !fetching_instruction)
            begin
                if(reduced_behavior)
                    case(reduced_behavior_bits)
                        2'b01 : 
                        begin
                            data_out_buff[31:0] = data_out_wide[31:0];
                            data_out_buff[127:32] = data_in_wide[127:32];
                        end
                        2'b10 : 
                        begin
                            data_out_buff[15:0] = data_out_wide[15:0];
                            data_out_buff[127:16] = data_in_wide[127:16];
                        end
                        2'b11 : 
                        begin
                            data_out_buff[7:0] = data_out_wide[7:0];
                            data_out_buff[127:8] = data_in_wide[127:8];
                        end
                    endcase
                else //normal behavior
                    data_out_buff[wordsize-1:0] = data_out_wide;
            end

    //Assigning data
    assign data_out = ( instruction == `inst_call ? data_out_cpu : data_out_buff[wordsize-1:0] ); 
    assign data_in_cpu = ( instruction == `inst_ret ? data_in : data_in_buff );
    assign pop_offset = ( reduced_behavior  && instruction != `inst_ret && instruction != `inst_call ? 
                            ( reduced_behavior_bits == 2'b01 ? 4 :
                                (reduced_behavior_bits == 2'b10 ? 2 :
                                    1))
                        : wordsize/8 );

endmodule

