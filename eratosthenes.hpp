#ifndef ERATOSTHENES_HPP_INCLUDED
#define ERATOSTHENES_HPP_INCLUDED

#include <ostream>
#include <vector>
#include <utility>
#include <iostream>
#include <chrono>

namespace voidhoge{

class null_stream : public std::streambuf, public std::ostream {
	char buf_[128];
protected:
	virtual int overflow(int c) {
		// // std::cout << "overflow( " << c << " )" << std::endl;
		setp( buf_, buf_ + sizeof( buf_ ) );
		return ( c == eof() ? '\0' : c );
	}

public:
	null_stream() : std::ostream(this) {}
};

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
	double eratosthenes_multithread(const long long limit, const int base_sieve_size, std::ostream& debug);
};

} //namespace voidhoge

#endif
