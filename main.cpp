#include <iostream>
#include <vector>
#include <chrono>

std::vector<int> seed_gen(const size_t limit) {
	const size_t size = sqrt(limit);
	std::vector<bool> data;
	data.resize(size);
	for (int i = 0; i < data.size(); i++) {
		data[i] = false;
	}
	data[0] = true;
	data[1] = true;
	for (size_t s = 2; s*s <= size;) {
		for (size_t i = s*s; i < size; i+=s) {
			data[i] = true;
		}
		s++;
		while (data[s] == true) {
			s++;
		}
	}
	std::vector<int> result;
	for (size_t i = 0; i < data.size(); i++) {
		if (data[i] == false) {
			result.push_back(i);
		}
	}
	return result;
}

std::vector<int> base_sieve_gen(const int base_sieve_size, const std::vector<int>& seed) {
	int sieve_max = 1;
	for (int i = 0; i < base_sieve_size; i++) {
		sieve_max *= seed[i];
	}
	std::vector<int> v;
	v.push_back(1);
	for (int i = base_sieve_size; seed[i] < sieve_max; i++) {
		v.push_back(seed[i]);
	}
	return v;
}

int main() {
	const size_t limit = (size_t)1<<40; // およそ1兆
	const auto seed_prime = seed_gen(limit);
	const int base_sieve_size = 3; // base_sieve_size個の素数で配列を分割する。
	const auto base_sieve = base_sieve_gen(base_sieve_size, seed_prime);
	return 0;
}
