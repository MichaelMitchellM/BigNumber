
#pragma once

#include <bitset>
#include <chrono> // for random numbers
#include <cstdint>
#include <intrin.h>
#include <iostream>
#include <random> // for random numbers
#include <utility>


/*

TODO: Vectorize all the things!

*/
class uint2048{
private:
	uint64_t parts_[32u];

public:

	// --- constructors ---
	// - standard -
	uint2048();
	uint2048(uint64_t num);

	// - copy -
	uint2048(const uint2048& num);

	// --- destructor ---
	~uint2048(){}

	// --- functions ---
	bool highest_bit(uint16_t* index) const;
	uint16_t num_bits() const;

	std::bitset<2048> to_bitset();

	/*
	generates a uint2048 with a given bit length
	*/
	static uint2048 Random(uint16_t num_bits, std::mt19937_64* mt_rand){
		uint2048 ret;
		uint8_t num_loops;
		uint8_t overflow;

		overflow = num_bits % 64u;
		num_loops = num_bits / 64u;

		for (auto i = 0u; i < num_loops; ++i)
			ret.parts_[i] = (*mt_rand)();

		if (overflow){
			auto r = (*mt_rand)();
			
			r >>= 64u - overflow;
			if (r >> (overflow - 1u) != 1u){
				auto to_add = 1ull;
				
				to_add <<= overflow - 1u;
				r += to_add;
			}
			ret.parts_[num_loops] = r;
		}
		return ret;
	}

	/*
	generates a uint2048 within given bounds
	*/
	static uint2048 Random(const uint2048& min, const uint2048& max, std::mt19937_64* mt_rand){
		uint2048 ret;
		uint16_t max_bits;

		max_bits = max.num_bits();
		ret = Random(max_bits, mt_rand);
		ret %= (max - min + 1ull);
		ret += min;
		return ret;
	}

	// --- operators ---

	// - assignment -

	friend uint2048& operator+=(uint2048& operand_a, const uint2048& operand_b);
	friend uint2048& operator+=(uint2048& operand_a, uint64_t operand_b);

	friend uint2048& operator-=(uint2048& operand_a, const uint2048& operand_b);

	friend uint2048& operator%=(uint2048& operand_dividend, const uint2048& operand_divisor);

	friend uint2048& operator<<=(uint2048& operand_a, uint16_t operand_b);
	friend uint2048& operator>>=(uint2048& operand_a, uint16_t operand_b);

	// - increment/decrement -
	
	uint2048& operator++();
	uint2048 operator++(int);

	// - arithmetic -

	friend uint2048 operator+(const uint2048& operand_a, const uint2048& operand_b);
	friend uint2048 operator+(const uint2048& operand_a, uint64_t operand_b);
	friend uint2048 operator+(uint64_t operand_a, const uint2048& operand_b);

	friend uint2048 operator-(const uint2048& operand_a, const uint2048& operand_b);
	friend uint2048 operator-(const uint2048& operand_a, uint64_t operand_b);

	friend uint2048 operator*(const uint2048& operand_a, const uint2048& operand_b);

	friend uint2048 operator/(const uint2048& dividend, const uint2048& divisor);

	friend uint2048 operator%(const uint2048& operand_dividend, const uint2048& operand_divisor);

	friend uint2048 operator&(const uint2048& operand_a, const uint2048& operand_b);
	friend uint64_t operator&(const uint2048& operand_a, uint64_t operand_b);
	
	friend uint2048 operator<<(const uint2048& operand_a, uint16_t operand_b);
	friend uint2048 operator>>(const uint2048& operand_a, uint16_t operand_b);

	// - comparison

	friend bool operator==(const uint2048& operand_a, const uint2048& operand_b);
	friend bool operator==(const uint2048& operand_a, uint64_t operand_b);
	
	friend bool operator!=(const uint2048& operand_a, const uint2048& operand_b);

	friend bool operator<(const uint2048& operand_a, const uint2048& operand_b);

	friend bool operator>(const uint2048& operand_a, const uint2048& operand_b);

	friend bool operator<=(const uint2048& operand_a, const uint2048& operand_b);

	friend bool operator>=(const uint2048& operand_a, const uint2048& operand_b);

};
