/*----------------------------------\
|This module is a stack used to keep|
|track of the interruption nesting. |
\----------------------------------*/

module reflet_stack #(
    parameter wordsize = 16,
    depth = 4
    )(
    input clk,
    input reset,
    input push,
    input [wordsize-1:0] in,
    input pop,
    output [wordsize-1:0] out
    );
    integer i; //loop counter

    reg [wordsize-1:0] data [depth-1:0];
    reg [depth-1:0] index; //point to the next updated element of data
    assign out = data[index-1];

    always @ (posedge clk)
        if(!reset)
        begin
            index = 0;
            for(i=0; i<depth; i=i+1)
                data[i] = 0;
        end
        else
        begin
            if(push)
            begin
                data[index] = in;
                index = index + 1;
            end
            else
                if(pop)
                    index = index - 1;
        end

endmodule

