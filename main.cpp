#include <iostream>
#include <vector>
#include <future>
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

std::vector<int> base_sieve_gen(const int base_sieve_size, const std::vector<int>& seed_prime) {
	int sieve_max = 1;
	for (int i = 0; i < base_sieve_size; i++) {
		sieve_max *= seed_prime[i];
	}
	std::vector<int> v;
	v.push_back(1);
	for (int i = base_sieve_size; seed_prime[i] < sieve_max; i++) {
		v.push_back(seed_prime[i]);
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

std::vector<int> initial_start_pos_gen(const size_t base_sieve_size, const size_t sieve_max, const std::vector<int>& seed_prime) {
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

int eratosthenes_thread(const int offset, const int base_sieve_size, const int sieve_max, const std::vector<int>& seed_prime, const std::vector<int>& initial_start_pos, std::vector<bool>& data) {
	for (int i = base_sieve_size; i < seed_prime.size(); i++) {
		// 開始場所の計算
		const long long sqprm = (long long)seed_prime[i]*seed_prime[i];
		const size_t sqprm_line = sqprm/sieve_max;
		size_t start_pos = (initial_start_pos[i]*offset)%seed_prime[i];
		start_pos += (sqprm_line/seed_prime[i])*seed_prime[i];
		for (size_t j = start_pos; j < data.size(); j+= seed_prime[i]) {
			data[j] = true;
		}
	}
	return 0;
}

int main() {
	const size_t limit = (size_t)1<<32;
	// const size_t limit = pow(10, 11);
	std::cout << limit << '\n';
	const auto seed_prime = seed_gen(limit);
	const int base_sieve_size = 3; // base_sieve_size個の素数で配列を分割する。
	const auto base_sieve = base_sieve_gen(base_sieve_size, seed_prime);
	auto f = [&](){
		int tmp = 1;
		for (int i = 0; i < base_sieve_size; i++) {
			tmp*=seed_prime[i];
		}
		return tmp;
	};
	const int sieve_max = f();

	std::vector<std::vector<bool>> data;
	data.resize(base_sieve.size());
	std::cout << "allocating..." << '\n';
	for (int i = 0; i < data.size(); i++) {
		if (base_sieve[i] <= limit%sieve_max) {
			data[i].resize(limit/sieve_max+1);
		}else {
			data[i].resize(limit/sieve_max);
		}
		for (size_t j = 0; j < data[i].size(); j++) {
			data[i][j] = false;
		}
	}
	std::cout << "allocated" << '\n';

	data[0][0] = true; // 1を非素数にセット

	const auto initial_start_pos = initial_start_pos_gen(base_sieve_size, sieve_max, seed_prime);

	auto start = std::chrono::system_clock::now();
	std::vector<std::future<int>> v;
	for (int i = 0; i < base_sieve.size(); i++) {
		v.push_back(std::async(std::launch::async, [=, &seed_prime, &initial_start_pos, &data]{
			return eratosthenes_thread(base_sieve[i], base_sieve_size, sieve_max, seed_prime, initial_start_pos, data[i]);
		}));
		// eratosthenes_thread(base_sieve[i], base_sieve_size, sieve_max, seed_prime, initial_start_pos, data[i]);
	}
	for (int i = 0; i < v.size(); i++) {
		v[i].get();
	}
	auto end = std::chrono::system_clock::now();

	size_t count = 0;
	for (size_t i = 0; i < data.size(); i++) {
		for (size_t j = 0; j < data[i].size(); j++) {
			if (data[i][j] == false) {
				count++;
			}
		}
	}
	std::cout << "There are " << count+base_sieve_size-1+base_sieve.size() << " prime numbers below " << limit << ".\n";
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	std::cout << elapsed << " milliseconds." << '\n';
	return 0;
}
