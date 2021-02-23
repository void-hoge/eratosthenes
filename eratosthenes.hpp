#ifndef ERATOSTHENES_HPP_INCLUDED
#define ERATOSTHENES_HPP_INCLUDED

#include <ostream>
#include <vector>
#include <utility>

namespace voidhoge{

class prime_binary_array {
private:
	std::vector<std::vector<bool>> data;
	std::vector<int> base;
	int sieve_max;
public:
	prime_binary_array () {};
	prime_binary_array (const std::vector<std::vector<bool>>& data, const std::vector<int>& base, int sieve_max);
	std::pair<long long, bool> at(const size_t base_pos, const size_t line_pos) const;
	size_t get_base_size() const;
	size_t get_line_size() const;
};

void eratosthenes_multithread(const long long limit, const int base_sieve_size, prime_binary_array& result, std::ostream& debug);

} //namespace voidhoge

#endif
