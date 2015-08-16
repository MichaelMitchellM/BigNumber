
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

	/*
	default constructor
	sets all unsigned long longs to zero
	*/
	uint2048();

	/*
	sets all but the least significant unsigned long long to zero.
	the least significant is set to the 'num' argument
	*/
	uint2048(uint64_t num);

	// - copy -

	/*
	copys the array from the input uint2048 into a new one
	*/
	uint2048(const uint2048& num);

	// --- destructor ---
	~uint2048(){}

	// --- functions ---

	/*
	finds and returns the index of the most significant bit
	*/
	bool highest_bit(uint16_t* index) const;

	/*
	returns the number of bits the current number takes up.
	the number of bits is equal to the index of the highest bit + 1
	or if there are no bits, it is 0
	*/
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

// --- static functions ---

/*
gcd_mod

finds the greatest common divisor of two uint2048s.
uses the Euclidean Algorithm with modulus.
*/
uint2048 gcd_mod(const uint2048& a, const uint2048& b);

/*
gcd_sub

finds the greatest common divisor of two uint2048s.
uses the Euclidean Algorithm with subtraction.
*/
uint2048 gcd_sub(const uint2048& a, const uint2048& b);
