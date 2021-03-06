
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
	uint2048 num_c;

	uint2048 p, q, n;
	uint2048 totient;
	uint2048 e;


	num_a = uint2048::Random(1024u, &r);
	num_b = uint2048::Random(1024u, &r);

	//std::cout << num_a.to_bitset().to_ullong() << std::endl;


	if (!(num_a & 1ull)){
		++num_a;
	}
	
	auto is = false;
	while (!is){
		std::cout << (is = miller_rabin_test(num_a, 10, &r)) << std::endl;
		if (is) break;
		num_a += 2ull;
	}


	std::cout << (num_a).to_bitset() << std::endl;

	printf("ding!\n");
	std::cin.ignore(1000, '\n');
	return 0;
}
