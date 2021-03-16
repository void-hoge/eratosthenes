#include "eratosthenes.hpp"

#include <future>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <math.h>

std::vector<int> seed_gen(const long long limit) {
	const long long size = sqrt(limit)+1;
	std::vector<bool> data(size, false);
	data[0] = true;
	data[1] = true;
	for (long long s = 2; s*s <= size;) {
		for (long long i = s*s; i < size; i+=s) {
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

std::vector<int> base_sieve_gen(const int base_sieve_size, const std::vector<int>& seed_prime) {
	int sieve_max = 1;
	for (int i = 0; i < base_sieve_size; i++) {
		sieve_max *= seed_prime[i];
	}
	std::vector<int> v;
	for (int i = 1; i < sieve_max; i++) {
		if (std::gcd(sieve_max, i) == 1) {
			v.push_back(i);
		}
	}
	return v;
}

int extgcd(int a, int b, int &x, int &y) {
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	int d = extgcd(b, a%b, y, x);
	y -= a/b * x;
	return d;
}

std::vector<int> initial_start_pos_gen(const long long base_sieve_size, const long long sieve_max, const std::vector<int>& seed_prime) {
	// sieve_max*x - seed_prime*y = -1を満たすyの配列を返す。
	// つまり、あまり1のスレッドにおける各素数の開始位置を返す。
	std::vector<int> v;
	for (int i = 0; i < base_sieve_size; i++) {
		v.push_back(0);
	}
	for (size_t i = base_sieve_size; i < seed_prime.size(); i++) {
		int x = 0, y = 0;
		int d = extgcd(sieve_max, -seed_prime[i], x, y);
		if (d > 0) {
			x = seed_prime[i]-x;
			y = sieve_max-y;
		}
		v.push_back(x);
	}
	return v;
}

std::chrono::system_clock::duration eratosthenes_thread(const int offset, const int base_sieve_size, const int sieve_max, const std::vector<int>& seed_prime, const std::vector<int>& initial_start_pos, std::vector<bool>& data) {
	const auto start = std::chrono::system_clock::now();
	for (size_t i = base_sieve_size; i < seed_prime.size(); i++) {
		// 開始場所の計算
		long long start_pos = ((long long)initial_start_pos[i]*offset)%seed_prime[i];
		if (start_pos*sieve_max+offset < (long long)seed_prime[i]*seed_prime[i]) {
			start_pos += seed_prime[i];
		}

		for (size_t j = start_pos; j < data.size(); j+= seed_prime[i]) {
			data[j] = true;
		}
	}
	const auto end = std::chrono::system_clock::now();
	return end - start;
}

double voidhoge::prime_binary_array::eratosthenes_multithread(const long long limit, const int base_sieve_size, std::ostream& debug) {
	debug << limit << '\n';
	// データ構造の準備
	const auto seed_prime = seed_gen(limit);
	base_sieve = base_sieve_gen(base_sieve_size, seed_prime);
	auto f = [&](){
		int tmp = 1;
		for (int i = 0; i < base_sieve_size; i++) {
			tmp*=seed_prime[i];
		}
		return tmp;
	};
	const int sieve_max = f();
	const auto initial_start_pos = initial_start_pos_gen(base_sieve_size, sieve_max, seed_prime);

	// メモリの確保
	data.resize(base_sieve.size());
	debug << "allocating memories..." << '\n';
	for (size_t i = 0; i < data.size(); i++) {
		if (base_sieve[i] <= limit%sieve_max) {
			data[i].resize(limit/sieve_max+1);
		}else {
			data[i].resize(limit/sieve_max);
		}
		for (size_t j = 0; j < data[i].size(); j++) {
			data[i][j] = false;
		}
	}
	long long datasize = 0;
	for (size_t i = 0; i < data.size(); i++) {
		datasize += data[i].size();
	}
	debug << "about " << datasize/8/(pow(1024, 3)) << " GB (" << datasize << " bits) allocated." << '\n';
	debug << "compression ratio: " << (double)limit/datasize << '\n';

	data[0][0] = true; // 1は素数ではない。

	const auto start = std::chrono::system_clock::now();
	std::vector<std::future<std::chrono::system_clock::duration>> threads;

	const unsigned int num_of_threads
	// = std::thread::hardware_concurrency(); // ハードウェアのスレッド数だけ立ち上げ
	= ~0; // 全部同時に立ち上げ
	for (size_t i = 0; i < base_sieve.size(); i += num_of_threads) {
		for (size_t j = 0; j < num_of_threads && i+j < base_sieve.size(); j++) {
			const int a = i+j;
			threads.push_back(std::async(std::launch::async, [&, a]{
				return eratosthenes_thread(base_sieve[a], base_sieve_size, sieve_max, seed_prime, initial_start_pos, data[a]);
			}));
		}
		for (size_t j = 0; j < threads.size(); j++) {
			debug << "thread " << i << " " << j << " "<< i+j << " : " <<
			 std::chrono::duration_cast<std::chrono::milliseconds>(threads[j].get()).count() <<
			" milliseconds" << '\n';
		}
		threads.clear();
	}

	const auto end = std::chrono::system_clock::now();

	// 素数のカウント
	long long count = 0;
	debug << "counting..." << '\n';
	for (size_t i = 0; i < data.size(); i++) {
		for (size_t j = 0; j < data[i].size(); j++) {
			if (data[i][j] == false) {
				count++;
			}
		}
	}
	count += base_sieve_size;
	debug << count << " prime numbers below " << limit << ".\n";
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	debug << elapsed << " milliseconds" << '\n';
	return elapsed;
}

std::pair<long long, bool> voidhoge::prime_binary_array::at(const size_t base_pos, const size_t line_pos) const {
	if (base_sieve.empty() == true) {
		return std::make_pair(0, false);
	}
	if (base_pos < 0) {
		return std::make_pair(0, false);
	}else if(base_pos >= base_sieve.size()) {
		return std::make_pair(0, false);
	}
	if (line_pos < 0) {
		return std::make_pair(0, false);
	}else if(line_pos >= data[base_pos].size()) {
		return std::make_pair(0, false);
	}
	return std::make_pair(sieve_max*line_pos+base_sieve[base_pos], !data[base_pos][line_pos]);
}

size_t voidhoge::prime_binary_array::get_base_size() const {
	if (base_sieve.empty() == true) {
		return 0;
	}
	return base_sieve.size();
}

size_t voidhoge::prime_binary_array::get_line_size() const {
	if (data.empty()) {
		return 0;
	}
	return data[0].size();
}
