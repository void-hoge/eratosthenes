#include <iostream>
#include <vector>
#include <future>
#include <chrono>
#include <algorithm>
#include <numeric>

std::vector<int> seed_gen(const long long limit) {
	const long long size = sqrt(limit)+1;
	std::vector<bool> data;
	data.resize(size);
	for (int i = 0; i < data.size(); i++) {
		data[i] = false;
	}
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
	for (long long i = 0; i < data.size(); i++) {
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
	for (int i = base_sieve_size; i < seed_prime.size(); i++) {
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
	for (int i = base_sieve_size; i < seed_prime.size(); i++) {
		// 開始場所の計算
		long long start_pos = (initial_start_pos[i]*offset)%seed_prime[i];
		// std::cout << seed_prime[i] << " " << start_pos << '\n';
		// std::cout << "\t" << start_pos << '\n';
		if (start_pos*sieve_max+offset == seed_prime[i]) {
			start_pos += seed_prime[i];
		}
		// std::cout << "\t" << start_pos*sieve_max+offset << '\n';
		// std::cout << "\t" << (start_pos*sieve_max+offset)/seed_prime[i] << '\n';

		for (long long j = start_pos; j < data.size(); j+= seed_prime[i]) {
			data[j] = true;
		}
	}
	const auto end = std::chrono::system_clock::now();
	return end - start;
}

int main() {
	// const long long limit = (long long)1<<20;
	const long long limit = pow(10, 11);
	std::cerr << limit << '\n';
	// データ構造の準備
	const int base_sieve_size = 4; // base_sieve_size個の素数で配列を分割する。1だと奇数だけを探索
	const auto seed_prime = seed_gen(limit);
	const auto base_sieve = base_sieve_gen(base_sieve_size, seed_prime);
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
	std::vector<std::vector<bool>> data;
	data.resize(base_sieve.size());
	std::cerr << "allocating memories..." << '\n';
	for (int i = 0; i < data.size(); i++) {
		if (base_sieve[i] <= limit%sieve_max) {
			data[i].resize(limit/sieve_max+1);
		}else {
			data[i].resize(limit/sieve_max);
		}
	}
	long long datasize = 0;
	for (int i = 0; i < data.size(); i++) {
		datasize += data[i].size()/8;
	}
	std::cerr << "about " << datasize/(pow(1024, 3)) << "GB allocated." << '\n';

	data[0][0] = true; // 1は素数ではない。

	auto start = std::chrono::system_clock::now();
	std::vector<std::future<std::chrono::system_clock::duration>> threads;
	for (int i = 0; i < base_sieve.size(); i++) {
		threads.push_back(std::async(std::launch::async, [&, i]{
			return eratosthenes_thread(base_sieve[i], base_sieve_size, sieve_max, seed_prime, initial_start_pos, data[i]);
		}));
	}

	// int t = 9;
	// eratosthenes_thread(base_sieve[t], base_sieve_size, sieve_max, seed_prime, initial_start_pos, data[t]);
	for (int i = 0; i < threads.size(); i++) {
		std::cerr << "thread " << i << " : " <<
		 std::chrono::duration_cast<std::chrono::milliseconds>(threads[i].get()).count() <<
		" milliseconds" << '\n';
	}
	auto end = std::chrono::system_clock::now();

	// 素数のカウント
	long long count = 0;
	std::vector<long long> result;
	for (long long i = 0; i < data.size(); i++) {
		for (long long j = 0; j < data[i].size(); j++) {
			if (data[i][j] == false) {
				result.push_back(j*sieve_max+base_sieve[i]);
				count++;
			}
		}
	}
	count += base_sieve_size;
	std::cerr << "There are " << count << " prime numbers below " << limit << ".\n";
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	std::cerr << elapsed << " milliseconds" << '\n';
	return 0;
}
