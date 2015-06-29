
#pragma once

#include <bitset>
#include <cstdint>
#include <intrin.h>
#include <iostream>
#include <utility>

class num_2048{
private:
	uint64_t parts_[32u];

public:

	// --- constructors ---
	// - standard -
	num_2048();
	num_2048(uint64_t num);

	// - copy -
	num_2048(const num_2048& num);

	// --- destructor ---
	~num_2048(){}

	// --- functions ---
	bool highest_bit(uint16_t* index) const;
	uint16_t num_bits() const;

	std::bitset<2048> to_bitset();

	static num_2048 Random(){
		num_2048 ret;
		auto bits = reinterpret_cast<std::bitset<2048>*>(ret.parts_);
		for (auto i = 0u; i < 2048u; ++i) bits->set(i, rand() % 2u == 0u);
		bits->set(2047);
		return ret;
	}

	// make operators static?

	// --- operators ---
	// - relational -
	bool operator<(const num_2048& num) const;
	bool operator>(const num_2048& num) const;
	bool operator<=(const num_2048& num) const;
	bool operator>=(const num_2048& num) const;
	bool operator==(const num_2048& num) const;
	bool operator==(uint64_t num) const;
	bool operator!=(const num_2048& num) const;

	// - arithmetic -

	// addition
	num_2048 operator+(const num_2048& num) const;
	num_2048 operator+(uint64_t num) const;
	num_2048& operator++();
	num_2048 operator++(int);

	// subtraction
	num_2048 operator-(const num_2048& num) const;
	num_2048 operator-(uint64_t num) const;

	// multiplication

	// division
	num_2048 operator/(const num_2048& divisor) const;

	// bitshift left
	num_2048 operator<<(uint16_t num) const;
	num_2048& operator<<=(uint16_t num);

	// bitshift right
	num_2048 operator>>(uint16_t num) const;
	num_2048& operator>>=(uint16_t num);

};
