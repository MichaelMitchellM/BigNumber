
#include "num_2048.hpp"

// --- constructors ---

// - standard -

/*
sets all unsigned long longs to zero
*/
num_2048::num_2048(){
	for (auto i = 0u; i < 32u; ++i) parts_[i] = 0ull;
}

/*
sets all but the least significant unsigned long long to zero.
the least significant is set to the 'num' argument
*/
num_2048::num_2048(uint64_t num){
	for (auto i = 1u; i < 32u; ++i) parts_[i] = 0ull;
	parts_[0] = num;
}

// - copy -

/*
copys the array from the input num_2048 into a new one
*/
num_2048::num_2048(const num_2048& num){
	for (auto i = 0u; i < 32u; ++i) parts_[i] = num.parts_[i];
}

// --- functions ---

/*
finds and returns the index of the most significant bit
*/
bool num_2048::highest_bit(uint16_t* index) const{
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

/*
returns the number of bits the current number takes up.
the number of bits is equal to the index of the highest bit + 1
or if there are no bits, it is 0
*/
uint16_t num_2048::num_bits() const{
	uint16_t res = 0u;

	if (highest_bit(&res)) return ++res;
	else return res;
}

/*
returns a bitset representation of the parts array
*/
std::bitset<2048> num_2048::to_bitset(){
	return *reinterpret_cast<std::bitset<2048>*>(parts_);
}

// --- operators ---

// - relational -

bool num_2048::operator<(const num_2048& num) const{
	uint64_t a, b;

	for (auto i = 0u; i < 32u; ++i){
		a = parts_[31u - i];
		b = num.parts_[31u - i];
		if (a < b) return true;
	}
	return false;
}

bool num_2048::operator>(const num_2048& num) const{
	return num < *this;
}

bool num_2048::operator<=(const num_2048& num) const{
	return !(num < *this);
}

bool num_2048::operator>=(const num_2048& num) const{
	return !(*this < num);
}

bool num_2048::operator==(const num_2048& num) const{
	uint64_t a, b;

	for (auto i = 0u; i < 32u; ++i){
		a = parts_[31u - i];
		b = num.parts_[31u - i];
		if (a != b) return false;
	}
	return true;
}

bool num_2048::operator==(uint64_t num) const{
	uint64_t a;

	for (auto i = 0u; i < 31u; ++i){
		a = parts_[31u - i];
		if (a != 0ull) return false;
	}
	return parts_[0] == num;
}

bool num_2048::operator!=(const num_2048& num) const{
	return !(*this == num);

}

// - arithmetic -

// addition
num_2048 num_2048::operator+(const num_2048& num) const{
	num_2048 ret;
	uint64_t a, b;
	uint64_t* c;
	uint8_t carry_flag = 0u;

	for (auto i = 0u; i < 32u; ++i){
		a = parts_[i];
		b = num.parts_[i];
		c = &(ret.parts_[i]);

		// intrinsic function
		// adcx instruction
		// *c = a + b + carry
		carry_flag = _addcarry_u64(carry_flag, a, b, c);
	}
	return ret;
}

num_2048 num_2048::operator+(uint64_t num) const{
	num_2048 ret;
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	a = parts_[0];
	b = &(ret.parts_[0]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	carry_flag = _addcarry_u64(carry_flag, a, num, b);

	if (carry_flag)
		for (auto i = 1u; i < 32u; ++i){
			a = parts_[i];
			b = &(ret.parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	return ret;
}

// ++increment
num_2048& num_2048::operator++(){
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	a = parts_[0];
	b = &(parts_[0]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	carry_flag = _addcarry_u64(carry_flag, a, 1ull, b);

	if (carry_flag)
		for (auto i = 1u; i < 32u; ++i){
			a = parts_[i];
			b = &(parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	return *this;
}

// increment++
num_2048 num_2048::operator++(int){
	num_2048 ret;
	uint64_t a;
	uint64_t* b;
	uint8_t carry_flag = 0u;

	ret = *this;
	a = parts_[0];
	b = &(parts_[0]);

	// intrinsic function
	// adcx instruction
	// *c = a + b + carry
	carry_flag = _addcarry_u64(carry_flag, a, 1ull, b);

	if (carry_flag)
		for (auto i = 1u; i < 32u; ++i){
			a = parts_[i];
			b = &(parts_[i]);

			// intrinsic function
			// adcx instruction
			// *c = a + b + carry
			carry_flag = _addcarry_u64(carry_flag, a, 0, b);

			if (!carry_flag) break;
		}
	return ret;
}

// subtraction

num_2048 num_2048::operator-(const num_2048& num) const{
	num_2048 ret;
	uint64_t a, b;
	uint64_t* c;
	uint8_t borrow_flag = 0u;

	for (auto i = 0u; i < 32u; ++i){
		a = parts_[i];
		b = num.parts_[i];
		c = &(ret.parts_[i]);

		// intrinsic function
		// sbb instruction
		// *c = a - (b + borrow)
		borrow_flag = _subborrow_u64(borrow_flag, a, b, c);
	}
	return ret;
}

num_2048 num_2048::operator-(uint64_t num) const{
	num_2048 ret;
	uint64_t a;
	uint64_t* b;
	uint8_t borrow_flag = 0u;

	a = parts_[0];
	b = &(ret.parts_[0]);

	// intrinsic function
	// sbb instruction
	// *c = a - (num + borrow)
	borrow_flag = _subborrow_u64(borrow_flag, a, num, b);

	if (borrow_flag){
		for (auto i = 1u; i < 32u; ++i){
			a = parts_[i];
			b = &(ret.parts_[i]);

			// intrinsic function
			// sbb instruction
			// *b = a - borrow
			borrow_flag = _subborrow_u64(borrow_flag, a, 0, b);

			if (!borrow_flag) break;
		}
	}
	return ret;
}

// multiplication

// division

/*
divides using binary long division
*/
num_2048 num_2048::operator/(const num_2048& divisor) const{
	num_2048 quotient;
	if (divisor > *this || divisor == 0ull) return quotient;
	num_2048 dividend, remainder;
	num_2048 temp;
	uint16_t dividend_bits, divisor_bits, remainder_bits;
	uint16_t bits_from_dividend;

	dividend = *this;
	dividend_bits = dividend.num_bits();
	divisor_bits = divisor.num_bits();
	remainder_bits = 0u;
	while ((dividend_bits + remainder_bits) >= divisor_bits){
		bits_from_dividend = divisor_bits - remainder_bits;
		if (remainder_bits){
			temp = remainder << bits_from_dividend;
			temp = temp + (dividend >> (dividend_bits - bits_from_dividend));
		}
		else temp = dividend >> (dividend_bits - bits_from_dividend);
		dividend = dividend << (2048u - (dividend_bits - bits_from_dividend));
		dividend = dividend >> (2048u - (dividend_bits - bits_from_dividend));
		quotient = quotient << bits_from_dividend;
		dividend_bits = dividend_bits - bits_from_dividend;
		while (temp < divisor){
			if (!dividend_bits) return quotient;
			temp = temp << 1u;
			temp = temp + (dividend >> (dividend_bits - 1u));
			dividend = dividend << (2048u - (dividend_bits - 1u));
			dividend = dividend >> (2048u - (dividend_bits - 1u));
			quotient = quotient << 1u;
			--dividend_bits;
		}
		++quotient;
		remainder = temp - divisor;
		remainder_bits = remainder.num_bits();
	}
	return quotient << dividend_bits;
}

// bitshift left

num_2048 num_2048::operator<<(uint16_t num) const{
	num_2048 ret;
	if (num >= 2048u) return ret;
	uint64_t a, b;
	uint64_t overflow = 0ull;
	uint8_t shift = num / 64u;

	for (auto i = 0u; i < (32u - shift); ++i) ret.parts_[i + shift] = parts_[i];
	if (num %= 64u)
		for (auto i = 0u; i < 32u; ++i){
			a = ret.parts_[i];
			b = a << num;
			ret.parts_[i] = b + overflow;
			overflow = a >> (64u - num);
		}
	return ret;
}

num_2048& num_2048::operator<<=(uint16_t num){
	if (num >= 2048u){
		for (auto i = 0u; i < 32u; ++i) parts_[i] = 0ull;
		return *this;
	}
	uint64_t a, b;
	uint64_t overflow = 0ull;
	uint8_t shift = num / 64u;

	for (auto i = 0u; i < (32u - shift); ++i) parts_[i + shift] = parts_[i];
	if (num %= 64u)
		for (auto i = 0u; i < 32u; ++i){
			a = parts_[i];
			b = a << num;
			parts_[i] = b + overflow;
			overflow = a >> (64u - num);
		}
	return *this;
}

// bitshift right

num_2048 num_2048::operator>>(uint16_t num) const{
	num_2048 ret;
	if (num >= 2048u) return ret;
	uint64_t a, b;
	uint64_t overflow = 0ull;
	uint16_t shift = num / 64u;

	for (auto i = 0u; i < (32u - shift); ++i) ret.parts_[i] = parts_[i + shift];
	if (num %= 64u)
		for (auto i = 0u; i < 32u; ++i){
			a = ret.parts_[31u - i];
			b = a >> num;
			ret.parts_[31u - i] = b + overflow;
			overflow = a << (64u - num);
		}
	return ret;
}

num_2048& num_2048::operator>>=(uint16_t num){
	if (num >= 2048u){
		for (auto i = 0u; i < 32u; ++i) parts_[i] = 0ull;
		return *this;
	}
	uint64_t a, b;
	uint64_t overflow = 0ull;
	uint16_t shift = num / 64u;

	for (auto i = 0u; i < (32u - shift); ++i) parts_[i] = parts_[i + shift];
	if (num % 64u)
		for (auto i = 0u; i < 32u; ++i){
			a = parts_[31u - i];
			b = a >> num;
			parts_[31u - i] = b + overflow;
			overflow = a << (64u - num);
		}
	return *this;
}

