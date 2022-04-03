
module alignment_fixer_tb();

    reg clk = 1;
    always #1 clk <= !clk;
    reg reset = 0;

    //Cpu ctrl
    wire [63:0] cpu_addr;
    wire [63:0] cpu_data_out;
    wire [3:0] size_used;
    wire cpu_write_en;

    //Other links
    wire [63:0] ram_addr;
    wire [63:0] ram_data_out;
    wire [63:0] cpu_data_in;
    wire [63:0] ram_data_in;
    wire ready;
    wire ram_write_en;

    reflet_alignment_fixer #(.word_size(64), .addr_size(64)) fix (
        .clk(clk),
        .cpu_addr(cpu_addr),
        .cpu_data_out(cpu_data_out),
        .cpu_data_in(cpu_data_in),
        .cpu_write_en(cpu_write_en),
        .ram_addr(ram_addr),
        .ram_data_in(ram_data_in),
        .ram_data_out(ram_data_out),
        .ram_write_en(ram_write_en),
        .size_used(size_used));

    dummyMem mem (
        .clk(clk),
        .addr(ram_addr),
        .data_in(ram_data_out),
        .data_out(ram_data_in),
        .write_en(ram_write_en));

    dummyCPU cpu (
        .clk(clk),
        .reset(reset),
        .data_out(cpu_data_out),
        .addr(cpu_addr),
        .size_used(size_used),
        .write_en(cpu_write_en));

    initial
    begin
        $dumpfile("alignment_fixer_tb.vcd");
        $dumpvars(0, alignment_fixer_tb);
        //Filling the begining of the ram
        #10;
        reset <= 1;
        #300;
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

module dummyCPU (
    input clk,
    input reset,
    output reg [63:0] addr = 0,
    output reg [63:0] data_out = 0,
    output reg [3:0] size_used = 0,
    output reg write_en = 0
    );

    reg [63:0] cnt;

    always @ (posedge clk)
        if(!reset)
            cnt <= 0;
        else
            cnt <= cnt + 1;

    always @ (posedge clk)
        case(cnt)
            1: begin //Testing alignment error
                size_used <= 3;
                addr <= 1;
            end
            4: begin //Filling the begining of the ram
                addr <= 0;
                write_en <= 1;
                data_out <= 64'h1212121212121212;
            end
            5: begin
                data_out <= 64'hABABABABABABABAB;
                addr <= 8;
            end
            6: begin
                data_out <= 64'hFDFDFDFDFDFDFDFD;
                addr <= 16;
            end
            7: begin //Testing reading
                write_en <= 0;
                addr <= 0;
            end
            9: begin //Testing missalligned read
                addr <= 2;
                size_used <= 1;
                write_en <= 1;
                data_out <= 64'h6789;
            end
            11: begin
                write_en <= 0;
            end
            12: begin
                write_en <= 1;
                addr <= 10;
                data_out <= 64'h1234;
            end
            14: begin
                write_en <= 0;
            end
        endcase

endmodule

