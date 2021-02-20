#include <iostream>
#include <vector>
#include <chrono>
#include <bitset>
#include <algorithm>

// const size_t SIZE = (size_t)1<<8;
const size_t SIZE = 100000;

long long extGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long d = extGCD(b, a%b, y, x);
    y -= a/b * x;
    return d;
}

int hogegcd(const int a, const int b) {
	long long x = 0, y = 0;
	extGCD(a, b, x, y);
	return y;
}

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
			std::cout << i << '\n';
			count++;
		}
	}
	std::cerr << "There are " << count << " prime numbers below " << SIZE-1 << ".\n";
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	std::cerr << elapsed << " milliseconds." << '\n';

	return 0;
}
