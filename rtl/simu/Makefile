# This makefile is used to use the various tesbenches vith icarus verilog
# And then show graphs with gtkwave

REFLET = ../reflet.vh ../reflet_addr.v ../reflet_addr_reduced_behavior.v ../reflet_alu.v ../reflet_cpu.v ../reflet_interrupt.v ../reflet_stack.v

all : stack_tb.vcd simu1.vcd simu2.vcd simu3.vcd simu4.vcd simu5.vcd

stack_tb.vcd : stack_tb.vvp
	vvp stack_tb.vvp

stack_tb.vvp : stack_tb.v ../reflet_stack.v
	iverilog stack_tb.v ../reflet_stack.v -o stack_tb.vvp

stack_tb : stack_tb.vcd
	gtkwave stack_tb.vcd

simu1 : simu1.vcd
	gtkwave simu1.vcd

simu1.vcd : simu1.vvp
	vvp simu1.vvp

simu1.vvp : simu1.v $(REFLET)
	cp ../reflet.vh ./
	iverilog simu1.v rom1.v $(REFLET) -o simu1.vvp
	rm ./reflet.vh

simu2 : simu2.vcd
	gtkwave simu2.vcd

simu2.vcd : simu2.vvp
	vvp simu2.vvp

simu2.vvp : simu2.v $(REFLET) reflet_ram8.v
	cp ../reflet.vh ./
	iverilog simu2.v rom2.v reflet_ram8.v $(REFLET) -o simu2.vvp
	rm ./reflet.vh

simu3 : simu3.vcd
	gtkwave simu3.vcd

simu3.vcd : simu3.vvp
	vvp simu3.vvp

simu3.vvp : simu3.v $(REFLET) reflet_ram16.v reflet_ram8.v
	cp ../reflet.vh ./
	iverilog simu3.v rom3.v reflet_ram16.v reflet_ram8.v $(REFLET) -o simu3.vvp
	rm ./reflet.vh

simu4 : simu4.vcd
	gtkwave simu4.vcd

simu4.vcd : simu4.vvp
	vvp simu4.vvp

simu4.vvp : simu4.v reflet_ram16.v reflet_ram8.v $(REFLET)
	cp ../reflet.vh ./
	iverilog simu4.v rom4.v $(REFLET) reflet_ram8.v reflet_ram16.v -o simu4.vvp
	rm ./reflet.vh

simu5 : simu5.vcd
	gtkwave simu5.vcd

simu5.vcd : simu5.vvp
	vvp simu5.vvp

simu5.vvp : simu5.v $(REFLET)
	cp ../reflet.vh ./
	iverilog simu5.v rom5.v $(REFLET) -o simu5.vvp
	rm ./reflet.vh

clean :
	rm -f *.vvp
	rm -f *.vcd

