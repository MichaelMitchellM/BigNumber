
#pragma once

#include <random>
#include <vector>

#include "uint2048.hpp"

// ! TODO needs input of k for accuracy of the test

bool miller_rabin_test(const uint2048& num, const unsigned accuracy, std::mt19937_64* mt_rand){

	// make sure num is odd and greater than 3
	if (!(num & 1ull) || (num <= 3ull)) return false;

	std::vector<uint64_t> test =
	{
		3ull, 5ull, 7ull, 11ull,
		13ull, 17ull, 19ull, 23ull, 29ull,
		31ull, 37ull, 41ull, 43ull, 47ull,
		53ull, 59ull, 61ull, 67ull, 71ull,
		73ull, 79ull, 83ull, 89ull, 97ull,
		101ull, 103ull, 107ull, 109ull, 113ull,
		127ull, 131ull, 137ull, 139ull, 149ull,
		151ull, 157ull, 163ull, 167ull, 173ull,
		179ull, 181ull, 191ull, 193ull, 197ull,
		199ull, 211ull, 223ull, 227ull, 229ull
	};

	for (auto n : test){
		if (num % n == 0ull) return false;
	}

	uint2048 a;
	uint16_t s;
	uint2048 d;
	uint2048 x;

	s = 0u;
	d = num - 1ull;

	// while d is even
	while (!(d & 1ull)){
		d >>= 1u;
		++s;
	}

	auto k = 1u;
	for (auto k = 0u; k < accuracy; ++k){
		a = uint2048::Random(2ull, num - 2ull, mt_rand);
		x = 1ull;

		// right to left binary method for modular exponentiation

		auto base = a;
		auto exp = d;

		while (exp > 0ull){
			if (exp & 1ull) x = (x * base) % num;
			exp >>= 1u;
			base = (base * base) % num;
		}

		if (x == 1ull || x == (num - 1ull)) continue;
		
		for (auto i = 0u; i < (s - 1u); ++i){
			x = (x * x) % num;
			if (x == 1ull) return false;
			if (x == (num - 1ull)) goto loop_end;
		}
		return false;
	loop_end:;
	}

	return true;
}
