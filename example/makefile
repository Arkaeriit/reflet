
all : primes.asb helloWorld.asb

primes.asb : primes.aso
	asasm link primes.aso primes.asb

primes.aso : primes.asa
	asasm compile primes.asa

helloWorld.asb : helloWorld.aso
	asasm link helloWorld.aso helloWorld.asb

helloWorld.aso : helloWorld.asa
	asasm compile helloWorld.asa

test : primes.asb helloWorld.asb
	echo test of primes.asb
	asvm primes.asb
	echo test of helloWorld.asb
	asvm helloWorld.asb

clean :
	rm -f *.asb
	rm -f *.aso

