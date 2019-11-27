
all : primes.asb

primes.asb : primes.aso
	asasm link primes.aso primes.asb

primes.aso : primes.asa
	asasm compile primes.asa

test : primes.asb
	echo test of primes.asb
	asvm primes.asb

clean :
	rm -f *.asb
	rm -f *.aso

