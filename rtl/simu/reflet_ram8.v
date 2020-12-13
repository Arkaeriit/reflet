/*----------------------------------------\
|This module codes for a 8-bit ram block  |
|with the same input and output addresses.|
|Its behavior tries to mimic standard     |
|synchronous RAM.                         |
\----------------------------------------*/

module reflet_ram8 #(
    parameter addrSize = 7,
    size = 128,
    resetable = 1
    )(
    input clk,
    input reset,
    input enable,
    input [addrSize-1:0] addr,
    input [7:0] data_in,
    input write_en,
    output [7:0] data_out
    );
    integer i; //loop variable

	// Declare memory 
	reg [7:0] memory_ram [size-1:0];
    reg [7:0] data_out_array;

    //addr control
    wire usable = enable && addr < size && reset;
	
	always @(posedge clk)
		if(!reset & resetable)
        begin
			for (i=0;i<size; i=i+1)
				memory_ram[i] = 0;
        end
		else
        begin
            if(usable && write_en)
                memory_ram[addr] <= data_in;
            data_out_array <= memory_ram[addr];
        end

    assign data_out = ( usable ? data_out_array : 0 );
        
endmodule

