/*--------------------------------------------\
|This module handle connection between the RAM|
|and the rest of the CPU.                     |
|Updating the stackPointer is done by the CPU.|
|A big part of its purpose is to make sure    |
|that all memory access are aligned.          |
\--------------------------------------------*/

`include "reflet.vh"

module reflet_addr #(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input enable,
    //instructions from the CPU
    input [wordsize-1:0] workingRegister,
    input [wordsize-1:0] programCounter,
    input [wordsize-1:0] stackPointer,
    input [wordsize-1:0] otherRegister,
    input [1:0] reduced_behaviour_bits,
    output reg [7:0] instruction,
    output alignment_error,
    //ram connection
    output [wordsize-1:0] addr,
    output [wordsize-1:0] data_out,
    input [wordsize-1:0] data_in,
    output write_en,
    //output to the CPU
    output [wordsize-1:0] out,
    output [3:0] out_reg,
    output reg ram_not_ready
    );

    reg instruction_ok;

    // Address selection
    wire [3:0] opperand = instruction[7:4];
    wire [wordsize-1:0] addr_pop = ( instruction == `inst_pop || instruction == `inst_ret ? stackPointer - wordsize/8 : 0 ); //We need the -wordsize/8 because the CPU updated the stack pointer
    wire [wordsize-1:0] addr_push = ( instruction == `inst_push || instruction == `inst_call ? stackPointer : 0 );
    wire [wordsize-1:0] addr_reg = ( opperand == `opp_str || opperand == `opp_load ? otherRegister : 0 );
    wire [wordsize-1:0] cpu_addr = ( instruction_ok ? addr_reg | addr_pop | addr_push : programCounter ); //The default behavior is to seek the address of the next piece of code
    assign addr = cpu_addr;

    // Talking back to the CPU
    assign out_reg = ( instruction == `inst_ret || instruction == `inst_call ? `pc_id : 0 ); 
    wire [wordsize-1:0] data_in_cpu;
    wire returning_value = instruction == `inst_pop || instruction == `inst_ret || opperand == `opp_load;
    wire [wordsize-1:0] data_in_usable = ( returning_value ? (instruction == `inst_ret ? data_in_cpu + 1 : data_in_cpu) : 0 ); //When we want to use value read from ram. Note, when returning from a function, we need to go after what we put in the stack in order not to be trapped in an infinite loop
    wire [wordsize-1:0] wr_out = ( instruction == `inst_push || instruction == `inst_call || opperand == `opp_str || instruction == `inst_tbm ? workingRegister : 0 ); //when we don't need to update any register we will simply put the content of the working register into itself
    assign out = wr_out | data_in_usable;

    // Sending data to RAM
    wire [wordsize-1:0] data_out_wr = ( instruction == `inst_push || opperand == `opp_str ? workingRegister : 0 );
    wire [wordsize-1:0] data_out_pc = ( instruction == `inst_call ? programCounter : 0 );
    wire [wordsize-1:0] data_out_cpu = data_out_wr | data_out_pc;



    // Reduced behavior mode
    reg byte_mode;
    wire [$clog2(wordsize/8):0] size_used = 
        ( byte_mode || !instruction_ok || reduced_behaviour_bits == 2'b11 || wordsize <= 8 ? 0 : 
          ( reduced_behaviour_bits == 2'b10 || wordsize <= 16 ? 1 :
            ( reduced_behaviour_bits == 2'b01 || wordsize <= 32 ? 2 :
              ( reduced_behaviour_bits == 2'b00 || wordsize <= 64 ? 3 :
                ( $clog2(wordsize/8) )))));

    always @ (posedge clk)
        if (!reset)
            byte_mode <= 0;
        else if(enable && (instruction == `inst_tbm) && (!ram_not_ready))
            byte_mode <= !byte_mode;

    // Taking action
    wire inst_read = opperand == `opp_load || instruction == `inst_pop || instruction == `inst_ret;
    wire inst_write = opperand == `opp_str || instruction == `inst_push || instruction == `inst_call;
    reg inst_mem, readying_ram, inst_mem_r, read_ready_reset, read_ready_latched;
    wire read_ready, write_ready;
    wire read_request = inst_mem & !inst_mem_r;
    reg [1:0] delay; // Delay used to wait for the next instruction to be fetched
    always @ (posedge clk)
        if (enable)
            inst_mem_r <= inst_mem;

    // As I don't want to assume when the read is finished, I used a latch to
    // register it.
    always @ (posedge clk)
        if (!reset)
            read_ready_latched <= 0;
        else if (enable)
            if (read_ready_reset)
                read_ready_latched <= 0;
            else if(read_ready)
                read_ready_latched <= 1;


    always @ (posedge clk)
        if (!reset)
        begin
            read_ready_reset <= 0;
            instruction_ok <= 0;
            inst_mem <= 0;
            ram_not_ready <= 1;
            readying_ram <= 0;
            delay <= 1;
        end
        else if (enable)
        begin
            if (instruction_ok)
            begin
                read_ready_reset <= 0;
                if (inst_mem)
                begin
                    if (inst_read & read_ready)
                    begin
                        readying_ram <= 1;
                        instruction_ok <= 0;
                    end
                    else if (inst_write & write_ready)
                    begin
                        readying_ram <= 1;
                        instruction_ok <= 0;
                    end
                end
                else
                begin
                    if (inst_read | inst_write)
                        inst_mem <= 1;
                    else
                    begin
                        readying_ram <= 1;
                        instruction_ok <= 0;
                    end
                end
            end
            else
            begin
                inst_mem <= 0;
                if (readying_ram)
                begin
                    ram_not_ready <= 0;
                    readying_ram <= 0;
                end
                else if (!ram_not_ready)
                begin
                    delay <= 1;
                    ram_not_ready <= 1;
                end
                else if (&delay)
                    delay <= 0;
                else if (|delay)
                    delay <= delay + 1;
                else
                begin
                    if (read_ready_latched)
                    begin
                        read_ready_reset <= 1;
                        instruction <= data_in_cpu;
                        instruction_ok <= 1;
                    end
                end
            end
        end

    reflet_mem_reader #(wordsize) reader (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .size_used(size_used),
        .addr(cpu_addr),
        .data_in_ram(data_in),
        .data_in_cpu(data_in_cpu),
        .read_request(delay == 2'b01 || read_request),
        .read_ready(read_ready));

    reflet_mem_writer #(wordsize) writer (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .size_used(size_used),
        .addr(cpu_addr),
        .data_in_ram(data_in),
        .data_out_cpu(data_out_cpu),
        .data_out_ram(data_out),
        .write_request(inst_write && read_ready && instruction_ok),
        .write_ready(write_ready));

    reflet_mem_shift_mask #(wordsize) rmsm (
        .addr(addr),
        .size_used(size_used),
        .mask(),
        .shift(),
        .alignment_error(alignment_error));

    assign write_en = write_ready;

endmodule


/*---------------------------------\
|This module reads from memory and |
|re-arrange the data if the access |
|is miss-aligned. It starts reading|
|on rising edges of read_request.  |
\---------------------------------*/
module reflet_mem_reader #(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input enable,
    input [$clog2(wordsize/8):0] size_used,
    input [wordsize-1:0] addr,
    input [wordsize-1:0] data_in_ram,
    output reg [wordsize-1:0] data_in_cpu,
    input read_request,
    output reg read_ready
    );

    wire [wordsize-1:0] mask;
    wire [$clog2(wordsize):0] shift;
    reflet_mem_shift_mask #(wordsize) rmsm (
        .addr(addr),
        .size_used(size_used),
        .mask(mask),
        .shift(shift),
        .alignment_error());

    reg read_request_r;
    wire new_request = read_request & !read_request_r;
    reg new_request_r;

    always @ (posedge clk)
        if (!reset)
        begin
            read_request_r <= 0;
            new_request_r <= 0;
            read_ready <= 0;
        end
        else if(enable)
        begin
            read_request_r <= read_request;
            new_request_r <= new_request;
            if (new_request_r)
            begin
                data_in_cpu <= (data_in_ram & mask) >> shift;
                read_ready <= 1;
            end
            else
                read_ready <= 0;
        end

endmodule


/*------------------------------------------------\
|This module writes to RAM after having rearranged|
|the data if the access is not aligned.           |
|Its starts its operation is rising edges of      |
|write_request and assumes that the data in the   |
|target address is in data_in_ram. The output     |
|write_ready can be used as a write_en order.     |
\------------------------------------------------*/
module reflet_mem_writer #(
    parameter wordsize = 16
    )(
    input clk,
    input reset,
    input enable,
    input [$clog2(wordsize/8):0] size_used,
    input [wordsize-1:0] addr,
    input [wordsize-1:0] data_in_ram,
    input [wordsize-1:0] data_out_cpu,
    output reg [wordsize-1:0] data_out_ram,
    input write_request,
    output reg write_ready
    );

    wire [wordsize-1:0] mask;
    wire [$clog2(wordsize):0] shift;
    reflet_mem_shift_mask #(wordsize) rmsm (
        .addr(addr),
        .size_used(size_used),
        .mask(mask),
        .shift(shift),
        .alignment_error());

    wire [wordsize-1:0] data_to_write = (data_in_ram & ~mask) | ((data_out_cpu << shift) & mask);

    reg write_request_r;
    wire new_request = write_request & !write_request_r;

    always @ (posedge clk)
        if (!reset)
        begin
            write_request_r <= 0;
            write_ready <= 0;
        end
        else if(enable)
        begin
            write_request_r <= write_request;
            if (new_request)
            begin
                data_out_ram <= data_to_write;
                write_ready <= 1;
            end
            else
                write_ready <= 0;
        end

endmodule


/*------------------------------------\
|This helper module compute the       |
|masks and shifts needed to           |
|realign memory access. It also       |
|tells if the realignment is possible.|
\------------------------------------*/
module reflet_mem_shift_mask #(
    parameter wordsize = 16
)(
    input [wordsize-1:0] addr,
    input [$clog2(wordsize/8):0] size_used,
    output [wordsize-1:0] mask,
    output [$clog2(wordsize):0] shift,
    output alignment_error
    );

    // Checking alignment errors
    wire [wordsize-1:0] invalid_addr_mask = (1 << size_used) - 1;
    assign alignment_error = |(addr & invalid_addr_mask);

    // Getting the alignment offset
    wire [wordsize-1:0] off_mask = (wordsize / 8) - 1;
    wire [wordsize-1:0] bytes_off_from_align = addr & off_mask;
    wire [wordsize-1:0] off_from_align = bytes_off_from_align * 8;
    assign shift = off_from_align[$clog2(wordsize):0];

    // Masking usable data
    wire [wordsize-1:0] bits_used = (1 << size_used) * 8;
    wire [wordsize-1:0] size_mask = (1 << bits_used) - 1;
    assign mask = size_mask << shift;

endmodule

