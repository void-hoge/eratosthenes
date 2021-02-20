#include <iostream>
#include <ostream>
#include "eratosthenes.hpp"

int main() {
	std::vector<std::vector<bool>> data;
	const long long limit = (long long)1 << 32;
	const int base_sieve_size = 4;
	eratosthenes_binary_array(limit, base_sieve_size, data, std::cout);
	return 0;
}
