
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

	printf("s: %u\nd:\n", s);
	std::cout << d.to_bitset() << std::endl;

	auto k = 1u;
	for (auto k = 0u; k < accuracy; ++k){
		//a = uint2048::Random(2ull, num - 2ull, mt_rand);

		a = 18029167960214880676ull; // ! TESTING

		uint2048 x{ 1ull };

		printf("loop!\n");

		// right to left binary method for modular exponentiation

		uint2048 base = a % num; // ? (a % num) will always equal a
		uint2048 exp = d;

		while (exp > 0ull){
			if (exp & 1ull) x = (x * base) % num;
			exp >>= 1u;
			base = (base * base) % num;
		}

		printf("a:\n");
		std::cout << a.to_bitset() << std::endl;

		printf("x:\n");
		std::cout << x.to_bitset() << std::endl;

		if (x == 1ull || x == (num - 1ull)){
			//printf("continue\n");
			continue;
		}
		
		for (auto i = 0u; i < (s - 1u); ++i){
			printf("noop!\n");
			x = (x * x) % num;

			printf("x:\n");
			std::cout << x.to_bitset() << std::endl;

			if (x == 1ull){
				printf("roop\n");
				return false;
			}
			if (x == (num - 1ull)){
				printf("goto\n");
				goto end_loop;
			}
		}
		printf("boop!\n");
		return false;
	end_loop:;
	}

	return true;
}

