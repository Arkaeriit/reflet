
all : asbrain.asb

asbrain.asb : asbrain.aso
	asasm link asbrain.aso asbrain.asb

asbrain.aso : asbrain.asa
	asasm compile asbrain.asa

clean :
	rm -f *.aso
	rm -f *.asb

test : asbrain.asb
	asvm asbrain.asb
	asasm compile out.asa
	asasm link out.aso out.asb
	asvm out.asb

