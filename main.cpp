#include <iostream>
#include <ostream>
#include "eratosthenes.hpp"
#include <math.h>

int main() {
	std::vector<std::vector<bool>> data;
	// const long long limit = (long long)1 << 34;
	const long long limit = (long long)pow(10, 11);
	const int base_sieve_size = 6;
	eratosthenes_binary_array(limit, base_sieve_size, data, std::cout);
	return 0;
}
