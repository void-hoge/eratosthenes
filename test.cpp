#include <iostream>
#include <bitset>

const size_t SIZE = 10000001;

int main() {
	std::bitset<SIZE> data;
	// true -> not prime
	// false -> prime
	data.set(0);
	data.set(1);
	for (size_t s = 2; s*s <= SIZE;) {
		for (size_t i = s*s; i < SIZE; i+=s) {
			data.set(i);
		}
		s++;
		while (data[s] == true) {
			s++;
		}
	}
	int count = 0;
	for (size_t i = 0; i < data.size(); i++) {
		if (data[i] == false) {
			count++;
		}
	}
	std::cout << "There are " << count << " prime numbers below " << SIZE-1 << ".\n";
	return 0;
}
