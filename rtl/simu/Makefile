
all : stack_tb.vcd

stack_tb.vcd : stack_tb.vvp
	vvp stack_tb.vvp

stack_tb.vvp : stack_tb.v ../reflet_stack.v
	iverilog stack_tb.v ../reflet_stack.v -o stack_tb.vvp

stack_tb : stack_tb.vcd
	gtkwave stack_tb.vcd

clean :
	rm -f *.vvp
	rm -f *.vcd

