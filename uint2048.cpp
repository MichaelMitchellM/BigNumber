
#include "uint2048.hpp"

// --- constructors ---

// - standard -

uint2048::uint2048(){
	for (auto i = 0u; i < 32u; ++i) parts_[i] = 0ull;
}

uint2048::uint2048(uint64_t num){
	for (auto i = 1u; i < 32u; ++i) parts_[i] = 0ull;
	parts_[0] = num;
}

// - copy -

uint2048::uint2048(const uint2048& num){
	for (auto i = 0u; i < 32u; ++i) parts_[i] = num.parts_[i];
}

// --- functions ---

bool uint2048::highest_bit(uint16_t* index) const{
	auto res = 0ul;
	auto part = 0ull;
	
	/*
	loop down from the most significant ull and find the first one that is non-zero.
	use the intrinsic function _BitScanReverse64 to find the index of that ull's
	most significant bit.
	set the index pointer's value to:
		64 * num of ulls below + the index resulting from _BitScanReverse64
	return true

	if all ulls are found to be zero, set the index to 0 and return false.
	*/
	for (auto i = 0u; i < 32u; ++i){
		part = parts_[31u - i];
		if (part > 0u){
			// intrinsic function
			// bsr instruction
			// find index of most significant bit
			_BitScanReverse64(&res, part);
			
			// could use FMA here?
			*index = ((31u - i) * 64u) + static_cast<uint16_t>(res);
			return true;
		}
	}
	*index = 0u;
	return false;
}

uint16_t uint2048::num_bits() const{
	uint16_t res = 0u;

	if (highest_bit(&res)) return ++res;
	else return res;
}

/*
returns a bitset representation of the parts array
*/
std::bitset<2048> uint2048::to_bitset(){
	return *reinterpret_cast<std::bitset<2048>*>(parts_);
}

// --- operators ---

// - assignment -

uint2048& operator+=(uint2048& operand_a, const uint2048& operand_b){
	uint64_t a, b;
	uint64_t* c;
	uint8_t carry_flag = 0u;

	for (auto i = 0u; i < 32u; ++i){
		a = operand_a.parts_[i];
		b = operand_b.parts_[i];
		c = &(operand_a.parts_[i]);

		// intrinsic function
		// adcx instruction
		// *c = a + b + carry
		// carry_flag is set to 1 if there is a carry bit
		carry_flag = _addcarry_u64(carry_flag, a, b, c);
	}

	return operand_a;
}
uint2048& operator+=(uint2048& operand_a, uint64_t operand_b){
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	a = operand_a.parts_[0u];
	b = &(operand_a.parts_[0u]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	// carry_flag is set to 1 if there is a carry bit
	carry_flag = _addcarry_u64(carry_flag, a, operand_b, b);

	if (carry_flag){
		for (auto i = 1u; i < 32u; ++i){
			a = operand_a.parts_[i];
			b = &(operand_a.parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	}

	return operand_a;
}

uint2048& operator-=(uint2048& operand_a, const uint2048& operand_b){
	uint64_t a, b;
	uint64_t* c;
	uint8_t borrow_flag = 0u;

	for (auto i = 0u; i < 32u; ++i){
		a = operand_a.parts_[i];
		b = operand_b.parts_[i];
		c = &(operand_a.parts_[i]);

		// intrinsic function
		// sbb instruction
		// *c = a - (b + borrow)
		// borrow_flag is set to 1 if (a < (b + borrow))
		borrow_flag = _subborrow_u64(borrow_flag, a, b, c);
	}
	return operand_a;
}

uint2048& operator%=(uint2048& operand_dividend, const uint2048& operand_divisor){
	uint2048 quotient;

	if (operand_dividend < operand_divisor) return operand_dividend;
	quotient = operand_dividend / operand_divisor;
	operand_dividend -= (operand_divisor * quotient);
	return operand_dividend;
}

uint2048& operator<<=(uint2048& operand_a, uint16_t operand_b){
	if (operand_b >= 2048u){
		for (auto i = 0u; i < 32u; ++i) operand_a.parts_[i] = 0ull;
		return operand_a;
	}
	uint64_t a, b, c;
	auto overflow = 0ull;
	auto shift = operand_b / 64u;

	if (shift){
		for (auto i = 0u; i < (32u - shift); ++i) operand_a.parts_[31u - i] = operand_a.parts_[31u - i - shift];
		for (auto i = 0u; i < shift; ++i) operand_a.parts_[i] = 0ull;
	}
	if (operand_b %= 64u){
		for (auto i = 0u; i < 32u; ++i){
			a = operand_a.parts_[i];
			b = a << operand_b;
			c = b + overflow;
			overflow = a >> (64u - operand_b);
			operand_a.parts_[i] = c;
		}
	}

	return operand_a;
}
uint2048& operator>>=(uint2048& operand_a, uint16_t operand_b){
	// if the input is greater than or equal to 2048
	//   set all unsigned long longs in parts_ to zero
	//   return reference to *this
	if (operand_b >= 2048u){
		for (auto i = 0u; i < 32u; ++i) operand_a.parts_[i] = 0ull;
		return operand_a;
	}

	uint64_t a, b, c;

	auto overflow = 0ull;
	auto shift = operand_b / 64u;

	if (shift){
		for (auto i = 0u; i < (32u - shift); ++i)
			operand_a.parts_[i] = operand_a.parts_[i + shift];
		for (auto i = 0u; i < shift; ++i)
			operand_a.parts_[31u - i] = 0ull;
	}
	if (operand_b %= 64u)
		for (auto i = 0u; i < 32u; ++i){
			a = operand_a.parts_[31u - i];
			b = a >> operand_b;
			c = b + overflow;
			overflow = a << (64u - operand_b);
			operand_a.parts_[31u - i] = c;
		}
	return operand_a;
}

// - increment / decrement -

uint2048& uint2048::operator++(){
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	a = parts_[0];
	b = &(parts_[0]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	// carry_flag is set to 1 if there is a carry bit
	carry_flag = _addcarry_u64(carry_flag, a, 1ull, b);

	if (carry_flag)
		for (auto i = 1u; i < 32u; ++i){
			a = parts_[i];
			b = &(parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			// carry_flag is set to 1 if there is a carry bit
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	return *this;
}
uint2048 uint2048::operator++(int){
	uint2048 ret;
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	ret = *this;
	a = parts_[0];
	b = &(parts_[0]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	// carry_flag is set to 1 if there is a carry bit
	carry_flag = _addcarry_u64(carry_flag, a, 1ull, b);

	if (carry_flag)
		for (auto i = 1u; i < 32u; ++i){
			a = parts_[i];
			b = &(parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			// carry_flag is set to 1 if there is a carry bit
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	return ret;
}

// - arithmetic -

uint2048 operator+(const uint2048& operand_a, const uint2048& operand_b){
	uint2048 ret;
	uint64_t a, b;
	uint64_t* c;
	uint8_t carry_flag = 0u;

	for (auto i = 0u; i < 32u; ++i){
		a = operand_a.parts_[i];
		b = operand_b.parts_[i];
		c = &(ret.parts_[i]);

		// intrinsic function
		// adcx instruction
		// *c = a + b + carry
		// carry_flag is set to 1 if there is a carry bit
		carry_flag = _addcarry_u64(carry_flag, a, b, c);
	}
	return ret;
}
uint2048 operator+(const uint2048& operand_a, uint64_t operand_b){
	uint2048 ret;
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	a = operand_a.parts_[0u];
	b = &(ret.parts_[0u]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	// carry_flag is set to 1 if there is a carry bit
	carry_flag = _addcarry_u64(carry_flag, a, operand_b, b);

	if (carry_flag){
		for (auto i = 1u; i < 32u; ++i){
			a = operand_a.parts_[i];
			b = &(ret.parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	}
		
	return ret;
}
uint2048 operator+(uint64_t operand_a, const uint2048& operand_b){
	uint2048 ret;
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	a = operand_b.parts_[0u];
	b = &(ret.parts_[0u]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	// carry_flag is set to 1 if there is a carry bit
	carry_flag = _addcarry_u64(carry_flag, a, operand_a, b);

	if (carry_flag){
		for (auto i = 1u; i < 32u; ++i){
			a = operand_b.parts_[i];
			b = &(ret.parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	}

	return ret;
}

uint2048 operator-(const uint2048& operand_a, const uint2048& operand_b){
	uint2048 ret;
	uint64_t a, b;
	uint64_t* c;
	uint8_t borrow_flag = 0u;

	for (auto i = 0u; i < 32u; ++i){
		a = operand_a.parts_[i];
		b = operand_b.parts_[i];
		c = &(ret.parts_[i]);

		// intrinsic function
		// sbb instruction
		// *c = a - (b + borrow)
		// borrow_flag is set to 1 if (a < (b + borrow))
		borrow_flag = _subborrow_u64(borrow_flag, a, b, c);
	}
	return ret;
}
uint2048 operator-(const uint2048& operand_a, uint64_t operand_b){
	uint2048 ret;
	uint64_t a;
	uint64_t* b;
	uint8_t borrow_flag = 0u;

	a = operand_a.parts_[0];
	b = &(ret.parts_[0]);

	// intrinsic function
	// sbb instruction
	// *c = a - (num + borrow)
	// borrow_flag is set to 1 if (a < (b + borrow))
	borrow_flag = _subborrow_u64(borrow_flag, a, operand_b, b);

	if (borrow_flag){
		for (auto i = 1u; i < 32u; ++i){
			a = operand_a.parts_[i];
			b = &(ret.parts_[i]);

			// intrinsic function
			// sbb instruction
			// *b = a - borrow
			// borrow_flag is set to 1 if (a < (b + borrow))
			borrow_flag = _subborrow_u64(borrow_flag, a, 0, b);

			if (!borrow_flag) break;
		}
	}
	else{
		for (auto i = 1u; i < 32u; ++i) ret.parts_[i] = operand_a.parts_[i];
	}
	return ret;
}

uint2048 operator*(const uint2048& operand_a, const uint2048& operand_b){
	uint2048 ret;

	if (operand_b == 0ull || operand_a == 0ull) return ret;
	if (operand_b == 1ull) return operand_a;
	if (operand_a == 1ull) return operand_b;

	uint2048 temp_a, temp_b;
	uint16_t num_loops;

	num_loops = operand_b.num_bits();
	temp_a = operand_b;
	temp_b = operand_a;
	for (auto i = 0u; i < num_loops; ++i){
		if (temp_a & 1ull) ret += temp_b;
		temp_a >>= 1u;
		temp_b <<= 1u;
	}
	return ret;
}

uint2048 operator/(const uint2048& operand_dividend, const uint2048& operand_divisor){
	uint2048 quotient;
	// ! need better way to handle division by 0, expections are gross
	// perhaps an error flag in the object?
	if (operand_divisor > operand_dividend || operand_divisor == 0ull) return quotient;
	uint2048 dividend, remainder;
	uint2048 offering;
	uint16_t dividend_bits, divisor_bits, remainder_bits;
	uint16_t bits_from_dividend;

	dividend = operand_dividend;
	dividend_bits = dividend.num_bits();
	divisor_bits = operand_divisor.num_bits();
	remainder_bits = 0u;

	// loop while there are more bits to divide into
	while ((dividend_bits + remainder_bits) >= divisor_bits){

		// determine how many bits are needed from the dividend
		bits_from_dividend = divisor_bits - remainder_bits;

		// if there are remainder bits
		if (remainder_bits){
			// push the remainder into the offering with trailing zeros
			// equal to the number of bits from the dividend
			offering = remainder << bits_from_dividend;

			// take the the most significant bits from the dividend
			// and add them to the offering.
			// the number of bits to take from the dividend are equal to
			// bits_from_dividend
			offering += (dividend >> (dividend_bits - bits_from_dividend));
		}
		else{
			// there are no remainder bits to add to the offering.
			// only take bits from the dividend
			offering = dividend >> (dividend_bits - bits_from_dividend);
		}

		// chop off the most significant bits from the dividend.
		// takes a number of bits equal to bits_from_dividend
		dividend <<= (2048u - (dividend_bits - bits_from_dividend));
		dividend >>= (2048u - (dividend_bits - bits_from_dividend));

		// push the bits in quotient to the left by bits_from_dividend
		quotient <<= bits_from_dividend;

		// calculate the number of bits left in the dividend
		dividend_bits = dividend_bits - bits_from_dividend;

		// loop until the offering is greater than or equal to
		// the divisor
		while (offering < operand_divisor){
			// if there are no more bits in the dividend return the quotient
			if (!dividend_bits) return quotient;

			// shift the bits in the offering 1 to the left
			offering <<= 1u;

			// add the most significant bit in the dividend to the offering
			offering += (dividend >> (dividend_bits - 1u));

			// chop off the most significant bit from the dividend
			dividend <<= (2049u - dividend_bits);
			dividend >>= (2049u - dividend_bits);

			// shift the bits in the quotient 1 to the left
			quotient <<= 1u;

			// decrement dividend_bits
			--dividend_bits;
		}

		// increment quotient
		++quotient;

		// calculate the remainder
		remainder = offering - operand_divisor;

		// get the number of bits in the remainder
		remainder_bits = remainder.num_bits();
	}

	// shift the bits in the quotient to the left by dividend_bits
	quotient <<= dividend_bits;
	return quotient;
}

uint2048 operator%(const uint2048& operand_dividend, const uint2048& operand_divisor){
	uint2048 quotient;
	uint2048 remainder;

	if (operand_dividend < operand_divisor) return operand_dividend;
	quotient = operand_dividend / operand_divisor;
	remainder = operand_dividend - (operand_divisor * quotient);
	return remainder;
}

uint2048 operator&(const uint2048& operand_a, const uint2048& operand_b){
	uint2048 ret;
	uint64_t a, b;

	for (auto i = 0u; i < 32u; ++i){
		a = operand_a.parts_[i];
		b = operand_b.parts_[i];
		ret.parts_[i] = a & b;
	}
	return ret;
}
uint64_t operator&(const uint2048& operand_a, uint64_t operand_b){
	uint64_t ret;
	uint64_t a;

	a = operand_a.parts_[0u];
	ret = a & operand_b;
	return ret;
}

uint2048 operator<<(const uint2048& operand_a, uint16_t operand_b){
	uint2048 ret;
	if (operand_b >= 2048u) return ret;
	uint64_t a, b;
	auto overflow = 0ull;
	auto shift = operand_b / 64u;

	for (auto i = 0u; i < (32u - shift); ++i) ret.parts_[i + shift] = operand_a.parts_[i];
	if (operand_b %= 64u)
		for (auto i = 0u; i < 32u; ++i){
			a = ret.parts_[i];
			b = a << operand_b;
			ret.parts_[i] = b + overflow;
			overflow = a >> (64u - operand_b);
		}
	return ret;
}
uint2048 operator>>(const uint2048& operand_a, uint16_t operand_b){
	uint2048 ret;
	if (operand_b >= 2048u) return ret;
	uint64_t a, b;
	auto overflow = 0ull;
	auto shift = operand_b / 64u;

	for (auto i = 0u; i < (32u - shift); ++i) ret.parts_[i] = operand_a.parts_[i + shift];
	if (operand_b %= 64u)
		for (auto i = 0u; i < 32u; ++i){
			a = ret.parts_[31u - i];
			b = a >> operand_b;
			ret.parts_[31u - i] = b + overflow;
			overflow = a << (64u - operand_b);
		}
	return ret;
}

// - comparison -

bool operator==(const uint2048& operand_a, const uint2048& operand_b){
	uint64_t a, b;
	for (auto i = 0u; i < 32u; ++i){
		a = operand_a.parts_[31u - i];
		b = operand_b.parts_[31u - i];
		if (a != b) return false;
	}
	return true;
}
bool operator==(const uint2048& operand_a, uint64_t operand_b){
	uint64_t a;
	for (auto i = 0u; i < 31u; ++i){
		a = operand_a.parts_[31u - i];
		if (a != 0ull) return false;
	}
	return operand_a.parts_[0] == operand_b;
}

bool operator!=(const uint2048& operand_a, const uint2048& operand_b){
	return !(operand_a == operand_b);
}

bool operator<(const uint2048& operand_a, const uint2048& operand_b){
	uint64_t a, b;
	for (auto i = 0u; i < 32u; ++i){
		a = operand_a.parts_[31u - i];
		b = operand_b.parts_[31u - i];
		if (a < b) return true;
		else if (b < a) return false;
	}
	return false;
}

bool operator>(const uint2048& operand_a, const uint2048& operand_b){
	return operand_b < operand_a;
}

bool operator<=(const uint2048& operand_a, const uint2048& operand_b){
	return !(operand_b < operand_a);
}

bool operator>=(const uint2048& operand_a, const uint2048& operand_b){
	return !(operand_a < operand_b);
}


// --- static functions ---

uint2048 gcd_mod(const uint2048& a, const uint2048& b){
	uint2048 temp_a, temp_b;
	uint2048 temp_t;

	temp_a = a;
	temp_b = b;
	while (temp_b != 0ull){
		temp_t = temp_b;
		temp_b = temp_a % temp_b;
		temp_a = temp_t;
	}
	return temp_a;
}

uint2048 gcd_sub(const uint2048& a, const uint2048& b){
	uint2048 temp_a, temp_b;

	temp_a = a;
	temp_b = b;
	while (temp_a != temp_b){
		if (temp_a > temp_b) temp_a -= temp_b;
		else temp_b -= temp_a;
	}
	return temp_a;
}

