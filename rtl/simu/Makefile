# This makefile is used to use the various tesbenches vith icarus verilog
# And then show graphs with gtkwave

REFLET       := ../reflet.vh ../reflet_addr.v ../reflet_alu.v ../reflet_cpu.v ../reflet_interrupt.v ../reflet_stack.v ../reflet_alignement_fixer.v
SIM_TOOLS    := rom1.v rom2.v rom3.v rom4.v rom5.v rom6.v rom7.v reflet_ram8.v reflet_ram16.v memory_tester.v
TEST_BENCHES := stack_tb simu1 simu2 simu3 simu4 simu5 simu6 simu7 alignement_fixer_tb memory_tester_tb
HEADERS      := ../reflet.vh

all:

define test_bench_template
$(1).vvp: $$(REFLET) $$(SIM_TOOLS) $$(HEADERS) $(1).v
	@echo "[IVERILOG] $(1).v"
	@iverilog $$(REFLET) $$(SIM_TOOLS) $(1).v -o $(1).vvp

$(1).vcd: $(1).vvp
	@echo "[VDC]      $(1).vvp"
	@vvp $$<

.PHONY: $(1).vcd
$(1): $(1).vcd
	@echo "[GTKWAVE]  $(1).vcd"
	@gtkwave $$<

ALL_VCD += $(1).vcd

$(1)_clean:
	rm -f $(1).vcd
	rm -f $(1).vvp

.PHONY: $(1)_clean
ALL_CLEAN += $(1)_clean

endef

$(foreach tb, $(TEST_BENCHES), $(eval $(call test_bench_template, $(tb))))

all: $(ALL_VCD)

clean : $(ALL_CLEAN)

all : stack_tb.vcd simu1.vcd simu2.vcd simu3.vcd simu4.vcd simu5.vcd

