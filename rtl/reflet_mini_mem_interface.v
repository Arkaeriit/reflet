/*----------------------------------------\
|This module handles the most basic memory|
|interfacing. It takes as input order     |
|for addresses and content and            |
|interfaces with memory. It uses a flag   |
|to tell when it is ready.                |
\----------------------------------------*/

module reflet_mini_mem_interface #(
    parameter wordsize = 16,
    use_buffer_register = 0 // Use buffers for the output of this module. If set to 0, this module is pass-though.
    )(
    input clk,
    input reset,
    input enable,
    // Addr from the CPU
    input [wordsize-1:0] cpu_addr,
    input [wordsize-1:0] cpu_data_out,
    output [wordsize-1:0] cpu_data_in,
    input cpu_write_en,
    input cpu_read_en,
    output cpu_ready,
    // Addr to the external memory
    output [wordsize-1:0] mem_addr,
    output [wordsize-1:0] mem_data_out,
    input [wordsize-1:0] mem_data_in,
    output mem_write_en
    );

    generate
        if (use_buffer_register)
        begin
            reg [wordsize-1:0] cpu_data_in_r;
            reg [wordsize-1:0] mem_addr_r;
            reg [wordsize-1:0] mem_data_out_r;
            reg mem_write_en_r;
            reg cpu_ready_r;
            assign cpu_data_in = cpu_data_in_r;
            assign mem_addr = mem_addr_r;
            assign mem_data_out = mem_data_out_r;
            assign mem_write_en = mem_write_en_r;
            assign cpu_ready = cpu_ready_r;

            reg working, reading;
            always @ (posedge clk)
                if (!reset)
                begin
                    cpu_ready_r <= 0;
                    mem_write_en_r <= 0;
                    working <= 0;
                    reading <= 0;
                end
                else if (enable)
                begin
                    if (working)
                    begin
                        working <= 0;
                        mem_write_en_r <= 0;
                        cpu_ready_r <= 1;
                        cpu_data_in_r <= mem_data_in;
                    end
                    else if (reading)
                    begin
                        reading <= 0;
                        working <= 1;
                    end
                    else if (cpu_write_en || cpu_read_en)
                        begin
                            if (cpu_read_en)
                                reading <= 1;
                            else
                                working <= 1;

                            cpu_ready_r <= 0;
                            mem_addr_r <= cpu_addr;
                            mem_write_en_r <= cpu_write_en;
                            mem_data_out_r <= cpu_data_out;
                        end
                    else
                    begin 
                        cpu_ready_r <= 0;
                    end 
                end
        end
        else
        begin
            assign mem_addr = cpu_addr;
            assign mem_data_out = cpu_data_out;
            assign cpu_data_in = mem_data_in;
            assign mem_write_en = cpu_write_en;
            assign cpu_ready = 1;
        end
    endgenerate

endmodule
    
