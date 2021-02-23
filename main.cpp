#include <iostream>
#include <ostream>
#include "eratosthenes.hpp"
#include <math.h>

int main() {
	const long long limit =
	(long long)1<<16;
	// (long long)1 << 32;
	// (long long)pow(10, 11);
	const int base_sieve_size = 5;
	voidhoge::prime_binary_array data;
	voidhoge::eratosthenes_multithread(limit, base_sieve_size, data, std::cout);
	// for (size_t i = 0; i < data.get_line_size(); i++) {
	// 	for (size_t j = 0; j < data.get_base_size(); j++) {
	// 		if (data.at(j, i).second == true) {
	// 			std::cout << data.at(j, i).first << ", ";
	// 		}
	// 	}
	// }
	// std::cout << '\n';
	return 0;
}
