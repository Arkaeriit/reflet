
all : asbrain.asb

asbrain.asb : asbrain.aso
	asasm link asbrain.aso asbrain.asb

asbrain.aso : asbrain.asa
	asasm compile asbrain.asa

clean :
	rm -f *.aso
	rm -f *.asb

test : asbrain.asb
	sh -c "cd ..; make" #We ensure that we are using the latest version of both the compiler and the VM
	../asvm asbrain.asb
	../asasm compile out.asa
	../asasm link out.aso out.asb
	../asvm out.asb

