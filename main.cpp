
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "primality_tests.hpp"
#include "uint2048.hpp"


int main(){
	std::seed_seq s{ 1u, (unsigned)std::chrono::system_clock::now().time_since_epoch().count() };
	std::mt19937_64 r{ s };

	uint2048 num_a, num_b;

	num_a = 13450560741990389923ull;

	num_b = 47ull;

	uint2048 d = num_a - 1ull;

	while (!(d & 1ull)){
		d >>= 1u;
	}

	uint2048 base = 2ull;
	uint2048 exp = d;
	uint2048 mod = num_a;
	uint2048 res = 1ull;

	/*
	while (exp > 0ull){
		if (exp & 1ull) res = (res * base) % mod;
		exp >>= 1u;
		base = (base * base) % mod;
	}
	*/

	num_a = uint2048::Random(128u, &r);
	num_b = uint2048::Random(100u, &r);


	std::cout << num_a.to_bitset() << std::endl;
	std::cout << num_b.to_bitset() << std::endl;

	std::cout << "division\n";
	std::cout << (num_a / num_b).to_bitset() << std::endl;

	std::cout << "modulus\n";
	std::cout << (num_a % num_b).to_bitset() << std::endl;



	auto is = false;
	//while (!is){
		//std::cout << num_a.to_bitset().to_ullong() << std::endl;
		//std::cout << (is = miller_rabin_test(num_a, 10, &r)) << std::endl;
		++num_a;
	//}

	printf("ding!\n");
	std::cin.ignore(1000, '\n');
	return 0;
}

