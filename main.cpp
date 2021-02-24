#include <iostream>
#include <ostream>
#include "eratosthenes.hpp"
#include <math.h>

int main() {
	const long long limit =
	// (long long)1<<16;
	(long long)1 << 32;
	// (long long)pow(10, 11);
	const int base_sieve_size = 5;
	voidhoge::prime_binary_array data;
	voidhoge::null_stream nullstrm;
	data.eratosthenes_multithread(limit, base_sieve_size, nullstrm);
	return 0;
}
