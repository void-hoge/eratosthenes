#ifndef ERATOSTHENES_HPP_INCLUDED
#define ERATOSTHENES_HPP_INCLUDED

#include <ostream>
#include <vector>

void eratosthenes_binary_array(const long long limit, const int base_sieve_size, std::vector<std::vector<bool>>& data, std::ostream& debug);

#endif
