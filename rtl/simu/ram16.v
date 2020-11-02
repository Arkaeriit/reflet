/*-------------------------------------\
|This module codes for a ram block with|
|the same input and output addresses.  |
|The words are 16 bits wide and each   |
|byte can be addressed.                |
\-------------------------------------*/

module ram16
    #(parameter addrSize = 9
    )(
    input clk,
    input reset,
    input output_en,
    input [addrSize-1:0] addr,
    input [15:0] data_in,
    input write_rq,
    output reg [15:0] data_out
    );
    

	// Declare memory 
	reg [7:0] memory_ram [(2**addrSize)-1:0];
	
	always @(posedge clk)
		if(!reset)
        begin
			for (integer i=0;i<(2**addrSize); i=i+1)
				memory_ram[i] = 0;
            data_out = 0;
        end
		else
        begin
            if(write_rq)
            begin
                memory_ram[addr] = data_in[7:0];
                memory_ram[addr+1] = data_in[15:8];
            end
            if(output_en)
            begin
                data_out[15:8] = memory_ram[addr+1];
                data_out[7:0] = memory_ram[addr];
            end
            else
                data_out = 0;
        end

endmodule

