# This makefile is used to use the various tesbenches vith icarus verilog
# And then show graphs with gtkwave

REFLET       := ../reflet.vh ../reflet_addr.v ../reflet_alu.v ../reflet_cpu.v ../reflet_interrupt.v
SIM_TOOLS    := rom01.v rom02.v rom03.v rom04.v rom05.v rom06.v rom07.v rom08.v rom09.v rom10.v rom11.v reflet_ram8.v reflet_ram16.v memory_tester.v
TEST_BENCHES := simu01 simu02 simu03 simu04 simu05 simu06 simu07 simu08 simu09 simu10 simu11 memory_tester_tb
HEADERS      := ../reflet.vh

all:

define test_bench_template
$(1).vvp: $$(REFLET) $$(SIM_TOOLS) $$(HEADERS) $(1).v
	@echo "[IVERILOG] $(1).v"
	@iverilog $$(REFLET) $$(SIM_TOOLS) $(1).v -o $(1).vvp

$(1).vcd: $(1).vvp
	@echo "[VDC]      $(1).vvp"
	@vvp $$<

.PHONY: $(1)
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

