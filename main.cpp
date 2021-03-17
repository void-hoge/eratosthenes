#include <iostream>
#include <ostream>
#include "eratosthenes.hpp"
#include <cmath>
#include <fstream>

int main() {
	const auto limit =
	// (long long)1<<16;
	// (long long)1 << 32;
	(long long)std::pow(10, 8);
	const int base_sieve_size = 3;
	voidhoge::prime_binary_array data;
	voidhoge::null_stream nullstrm;

	std::ofstream ofs("prime.txt");

	std::cout << data.eratosthenes_multithread(limit, base_sieve_size, std::cout) << " prime numbers below " << limit << ".\n";
	data.dump(0, limit, " ", ofs);
	ofs.close();
	return 0;
}
