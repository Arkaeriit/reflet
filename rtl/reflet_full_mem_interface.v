/*---------------------------\
|This modules combines       |
|reflet_alignment_fixer and  |
|reflet_mini_mem_interface in|
|order to separate the logic |
|of the memory interface and |
|the handling of instructions|
|related to memory.          |
\---------------------------*/

module reflet_full_mem_interface #(
    parameter wordsize = 16,
    use_buffer_register = 0 // Use buffers in reflet_mini_mem_interface
)(
    input clk,
    input reset,
    input enable,
    input toggle_byte_mode,
    input [1:0] reduced_behaviour_bits,
    output alignment_error,
    input force_alligned_use,
    // Interface from the CPU
    input [wordsize-1:0] cpu_addr,
    input [wordsize-1:0] cpu_data_out,
    output [wordsize-1:0] cpu_data_in,
    input cpu_write_en,
    input cpu_read_en,
    output cpu_ready,
    // Interface to the external memory
    output [wordsize-1:0] mem_addr,
    output [wordsize-1:0] mem_data_out,
    input [wordsize-1:0] mem_data_in,
    output mem_write_en
    );

    // Wires between the reduced behavior and the mini interface.
    wire [wordsize-1:0] addr;
    wire [wordsize-1:0] data_in;
    wire [wordsize-1:0] data_out;
    wire write_en;
    wire read_en;
    wire ready;
    wire alignment_fixer_ready;

    // Toggling byte mode
    reg byte_mode;
    always @ (posedge clk)
        if(!reset)
            byte_mode <= 0;
        else
            if(enable && toggle_byte_mode)
                byte_mode <= !byte_mode;

    assign cpu_ready = ready & alignment_fixer_ready;

    //handling reduced behavior
    generate
        if(wordsize == 8) //no reduced behavior possible
        begin
            assign cpu_data_in = data_in;
            assign data_out = cpu_data_out;
            assign addr = cpu_addr;
            assign write_en = cpu_write_en;
            assign read_en = cpu_read_en;
            assign alignment_fixer_ready = 1'b1;
            assign alignment_error = 1'b0;
        end
        else
        begin
            assign addr = cpu_addr;
            //Handleling the toggle of byte mode

            //Controling the width of data to get
            wire [15:0] size_used = ( force_alligned_use ? (wordsize/8 - 1) :
                                      ( byte_mode ? 0 :
                                        ( reduced_behaviour_bits == 2'b00 ? (wordsize/8 - 1) :
                                          ( reduced_behaviour_bits == 2'b01 ? 2 : //TODO, make the value 2 depends on the wordsize as it is currentely broken in 32 bit CPU
                                            ( reduced_behaviour_bits == 2'b10 ? 1 : 0 )))));
            reflet_alignment_fixer #(.word_size(wordsize), .addr_size(wordsize)) alignment_fixer (
                .clk(clk),
                .reset(reset),
                .size_used(size_used[$clog2(wordsize/8):0]),
                .ready(alignment_fixer_ready),
                .alignment_error(alignment_error),
                //Bus to the CPU
                .cpu_addr(cpu_addr),
                .cpu_data_out(cpu_data_out),
                .cpu_data_in(cpu_data_in),
                .cpu_write_en(cpu_write_en),
                .cpu_read_en(cpu_read_en),
                //Bus to the RAM
                .ram_addr(), //Not used as we want to output the raw address
                .ram_data_out(data_out),
                .ram_data_in(data_in),
                .ram_write_en(write_en),
                .ram_read_en(read_en));
            wire reduced_behaviour = ((reduced_behaviour_bits != 2'b00) && ( (reduced_behaviour_bits == 2'b01 && wordsize > 32) || (reduced_behaviour_bits == 2'b10 && wordsize > 16) || (reduced_behaviour_bits == 2'b11 && wordsize > 8) )) || byte_mode;
        end
    endgenerate

    // Mini memory interface
    reflet_mini_mem_interface #(.wordsize(wordsize), .use_buffer_register(use_buffer_register)) mini (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        // Interface from the CPU
        .cpu_addr(addr),
        .cpu_data_out(data_out),
        .cpu_data_in(data_in),
        .cpu_write_en(write_en),
        .cpu_read_en(read_en),
        .cpu_ready(ready),
        // Interface to the external memory
        .mem_addr(mem_addr),
        .mem_data_out(mem_data_out),
        .mem_data_in(mem_data_in),
        .mem_write_en(mem_write_en));
    

endmodule

