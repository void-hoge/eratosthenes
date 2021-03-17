#include <iostream>
#include <ostream>
#include "eratosthenes.hpp"
#include <cmath>
#include <fstream>
#include <string>
#include <filesystem>

int main(const int argc, const char** argv) {
	if (argc != 2) {
		throw std::runtime_error("No directory name. Input after executable file");
	}

	const auto limit =
	// (long long)1<<16;
	// (long long)1 << 32;
	(long long)std::pow(10, 10);
	const int base_sieve_size = 6;
	voidhoge::prime_binary_array data;
	voidhoge::null_stream nullstrm;


	const long long single_file = (long long)std::pow(10, 8);

	std::cout << data.eratosthenes_multithread(limit, base_sieve_size, std::cout) << " prime numbers below " << limit << ".\n";


	std::string directory_name = argv[1];
	if (!std::filesystem::exists(directory_name)) {
		if (!std::filesystem::create_directories(directory_name)) {
			throw std::runtime_error("create directory failed.");
		}
	}
	for (long long i = 0; i < limit; i+=single_file) {
		std::string filename = directory_name + "/" + std::to_string(i) + "-" + std::to_string(i+single_file) + ".txt";
		std::ofstream ofs(filename);
		data.dump(i, i+single_file, " ", ofs);
		ofs.close();
	}
	return 0;
}
