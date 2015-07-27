
#pragma once

#include <random>

#include "uint2048.hpp"

/*

needs input of k for accuracy of the test

*/
bool miller_rabin_test(const uint2048& num, const unsigned accuracy, std::mt19937_64* mt_rand){

	// make sure num is odd and greater than 3

	if (!(num & 1ull) || (num < 3ull)) return false;

	uint2048 a;
	uint16_t s;
	uint2048 d;

	s = 0u;
	d = num - 1ull;

	while (!(d & 1ull)){
		d >>= 1u;
		++s;
	}

	auto k = 1u;
	for (auto k = 0u; k < accuracy; ++k){
		a = uint2048::Random(2ull, num - 2ull, mt_rand);
		uint2048 x{ 1ull };


		// right to left binary method for modular exponentiation

		//uint2048 base = a % num; // ? (a % num) will always equal a
		uint2048 base = a;
		uint2048 exp = d;

		std::cout << a.to_bitset().to_ullong() << std::endl;
		std::cout << base.to_bitset().to_ullong() << std::endl;
		std::cout << exp.to_bitset().to_ullong() << std::endl;
		printf("loop\n");

		while (exp > 0ull){
			if (exp & 1ull){
				x = (x * base) % num;
				std::cout << x.to_bitset().to_ullong() << std::endl;
			}
			exp >>= 1u;
			base = (base * base) % num;
		}


		if (x == 1ull || x == (num - 1ull)) continue;
		
		for (auto i = 0u; i < (s - 1u); ++i){
			x = (x * x) % num;
			if (x == 1ull) return false;
			if (x == (num - 1ull)) goto end_loop;
		}
		return false;
	end_loop:;
	}

	return true;
}

