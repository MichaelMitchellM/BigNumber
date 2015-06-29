
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "num_2048.hpp"

int main(){

	num_2048 num_a, num_b;

	num_a = num_a + 179;
	num_b = num_b + 90;


	std::cout << (num_a / num_b).to_bitset().to_ullong() << std::endl;


	printf("ding!\n");
	std::cin.ignore(1000, '\n');
	return 0;
}

