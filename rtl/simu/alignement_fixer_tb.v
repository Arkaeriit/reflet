
module alignement_fixer_tb();

    reg clk = 1;
    always #1 clk <= !clk;
    reg reset = 0;

    //Cpu ctrl
    reg [63:0] cpu_addr = 0;
    reg [63:0] cpu_data_out = 0;
    reg [3:0] size_reduction = 0;
    reg cpu_write_en = 0;

    //Other links
    wire [63:0] ram_addr;
    wire [63:0] ram_data_out;
    wire [63:0] cpu_data_in;
    wire [63:0] ram_data_in;
    wire ready;
    wire ram_write_en;

    reflet_alignement_fixer #(.word_size(64), .addr_size(64)) fix (
        .clk(clk),
        .reset(reset),
        .cpu_addr(cpu_addr),
        .cpu_data_out(cpu_data_out),
        .cpu_data_in(cpu_data_in),
        .cpu_write_en(cpu_write_en),
        .ram_addr(ram_addr),
        .ram_data_in(ram_data_in),
        .ram_data_out(ram_data_out),
        .ram_write_en(ram_write_en),
        .size_used(size_reduction));

    dummyMem mem (
        .clk(clk),
        .addr(ram_addr),
        .data_in(ram_data_out),
        .data_out(ram_data_in),
        .write_en(ram_write_en));

    initial
    begin
        $dumpfile("alignement_fixer_tb.vcd");
        $dumpvars(0, alignement_fixer_tb);
        #1;
        //Testing allignement error
        size_reduction <= 3;
        cpu_addr <= 1;
        //Filling the begining of the ram
        #10;
        cpu_addr <= 0;
        size_reduction <= 3;
        reset <= 1;
        cpu_write_en <= 1;
        cpu_data_out <= 64'h1212121212121212;
        #10;
        cpu_addr <= 8;
        cpu_data_out <= 64'hABABABABABABABAB;
        #10;
        cpu_addr <= 16;
        cpu_data_out <= 64'hFDFDFDFDFDFDFDFD;
        //Testing 16 bit access
        #6;
        cpu_addr <= 0;
        cpu_write_en <= 0;
        #10;
        cpu_write_en <= 1;
        size_reduction <= 1;
        cpu_addr <= 2;
        cpu_data_out <= 64'h5678;
        #10;
        cpu_addr <= 10;
        #10;
        cpu_write_en <= 0;
        #10;
        $finish;
    end

endmodule

module dummyMem (
    input clk,
    input [63:0] addr,
    input [63:0] data_in,
    output [63:0] data_out,
    input write_en
    );

    reg [63:0] my_super_ram_0;
    reg [63:0] my_super_ram_1;
    reg [63:0] my_super_ram_2;
    reg [63:0] my_super_ram_3;
    reg [63:0] my_super_ram_4;
    wire [60:0] used_addr = addr[63:3];
    reg [63:0] data_out_of_ram;

    always @ (posedge clk)
    begin
        case(used_addr)
            61'h0: data_out_of_ram <= my_super_ram_0;
            61'h1: data_out_of_ram <= my_super_ram_1;
            61'h2: data_out_of_ram <= my_super_ram_2;
            61'h3: data_out_of_ram <= my_super_ram_3;
            61'h4: data_out_of_ram <= my_super_ram_4;
        endcase
        if(write_en)
        begin
            case(used_addr)
                61'h0: my_super_ram_0 <= data_in;
                61'h1: my_super_ram_1 <= data_in;
                61'h2: my_super_ram_2 <= data_in;
                61'h3: my_super_ram_3 <= data_in;
                61'h4: my_super_ram_4 <= data_in;
            endcase
        end
    end

    assign data_out = data_out_of_ram;

endmodule

