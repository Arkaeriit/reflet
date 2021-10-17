CFLAGS := -g
#CFLAGS := -Os -fdata-sections -ffunction-sections -Wl,--gc-sections

all: reflet-asm.bin

INSTALL_DIR := /usr/local/bin

reflet-asm.bin: objsrc/reflet-asm.c
	gcc $< $(CFLAGS) -o $@

objsrc/reflet-asm.c: | main.nelua
	mkdir -p objsrc
	nelua --generate-code -o $@ main.nelua

clean:
	rm -f reflet-asm.bin

purge: clean
	rm -f objsrc/reflet-asm.c

full_compilation: purge
	$(MAKE) reflet-asm.bin

install: | reflet-asm
	cp -f reflet-asm.bin $(INSTALL_DIR)/reflet-asm

uninstall:
	rm -f $(INSTALL_DIR)/reflet-asm

