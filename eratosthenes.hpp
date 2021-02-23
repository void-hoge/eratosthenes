#ifndef ERATOSTHENES_HPP_INCLUDED
#define ERATOSTHENES_HPP_INCLUDED

#include <ostream>
#include <vector>
#include <utility>

namespace voidhoge{

class prime_binary_array {
private:
	std::vector<std::vector<bool>> data;
	std::vector<int> base_sieve;
	int sieve_max;
public:
	prime_binary_array (){};
	std::pair<long long, bool> at(const size_t base_pos, const size_t line_pos) const;
	size_t get_base_size() const;
	size_t get_line_size() const;
	void eratosthenes_multithread(const long long limit, const int base_sieve_size, std::ostream& debug);
};

} //namespace voidhoge

#endif
