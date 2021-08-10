/*-------------------------------------\
|This module codes for a ram block with|
|the same input and output addresses.  |
|The words are 16 bits wide and accesse|   |
|should be aligned to 16 bits.         |
\-------------------------------------*/

//Compute the size 

module reflet_ram16 #(
    parameter addrSize = 9,
    size = 2**addrSize,
    resetable = 1
    )(
    input clk,
    input reset,
    input enable,
    input [addrSize-1:0] addr,
    input [15:0] data_in,
    input write_en,
    output reg [15:0] data_out
    );
    
    wire [addrSize-1:0] used_addr = addr[addrSize-1:1];
    reg [15:0] ram[size-1:0];

    generate
        if(resetable)
        begin
            integer i;
            always @ (posedge clk)
                if(!reset)
                begin
                    for(i=0; i<size; i=i+1)
                        ram[i] <= 0;
                    data_out <= 0;
                end
                else
                begin
                    if(write_en)
                        ram[used_addr] <= data_in;
                    data_out <= ram[used_addr];
                end
        end
        else
            always @ (posedge cl)
            begin
                if(write_en)
                    ram[used_addr] <= data_in;
                    data_out <= ram[used_addr];
            end
        endgenerate

endmodule

