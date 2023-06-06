all: reflet-asm.bin

reflet-asm.bin: src/macro_asm_builder/* src/reflet-asm/*
	cargo build --release
	cp ./target/release/reflet-asm $@

.PHONY: install
install: reflet-asm.bin
	mkdir -p /usr/local/bin
	cp $< /usr/local/bin/reflet-asm

uninstall:
	rm -f /usr/local/bin/reflet-asm

clean:
	rm -f reflet-asm.bin

