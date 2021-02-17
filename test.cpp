#include <iostream>
#include <vector>
#include <chrono>
#include <bitset>

const size_t SIZE = 10000000001;

int main() {
	// std::bitset<SIZE> data;
	std::vector<bool> data;
	data.resize(SIZE);
	for (size_t i = 0; i < data.size(); i++) {
		data[i] = false;
	}
	// true -> not prime
	// false -> prime

	auto start = std::chrono::system_clock::now();
	data[0] = true;
	data[1] = true;
	for (size_t s = 2; s*s <= SIZE;) {
		for (size_t i = s*s; i < SIZE; i+=s) {
			data[i] = true;
		}
		s++;
		while (data[s] == true) {
			s++;
		}
	}
	auto end = std::chrono::system_clock::now();
	int count = 0;
	for (size_t i = 0; i < data.size(); i++) {
		if (data[i] == false) {
			count++;
		}
	}
	std::cout << "There are " << count << " prime numbers below " << SIZE-1 << ".\n";
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	std::cout << elapsed << " milliseconds." << '\n';
	return 0;
}
