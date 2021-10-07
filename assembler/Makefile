CFLAGS := -g
#CFLAGS := -Os -fdata-sections -ffunction-sections -Wl,--gc-sections

all: reflet-fpu-asm

INSTALL_DIR := /usr/local/bin

reflet-fpu-asm: objsrc/reflet-fpu-asm.c
	gcc $< $(CFLAGS) -o $@

objsrc/reflet-fpu-asm.c: | main.nelua
	mkdir -p objsrc
	nelua --generate-code -o $@ main.nelua

clean:
	rm -f reflet-fpu-asm

purge: clean
	rm -f objsrc/reflet-fpu-asm.c

full_compilation: purge
	$(MAKE) reflet-fpu-asm

install: | reflet-fpu-asm
	cp -f reflet-fpu-asm $(INSTALL_DIR)/reflet-fpu-asm

uninstall:
	rm -f $(INSTALL_DIR)/reflet-fpu-asm

