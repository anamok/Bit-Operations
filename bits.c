#ifndef COMMON_H
#include "common.h"
#endif

/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
	/* 
		XOR of some x&y will return 0s for (0,0) and (1,1), and will return 
		1s for (0,1) and (1,0). AND operation will only help determine the (1,1)
		pair. To find the (0,0) pair, we first have to perform an AND operation
		on negated x and y (~x&~y, i.e. NOR). Then, we perform the NOR
		operation on the two results, that way NOR(0&1)=0, NOR(1&1)=1, NOR(0&0)=1.
	*/
	int and, nor;
	and = ~(x & y);
	nor = ~x & ~y;
	return and & ~nor;
}

/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
	/* 
		Progress through the bits left to right and check if, as we progress through
		the number by halves, that they are all equal to 1.
	*/
	x &= x >> 16;
	x &= x >> 8;
	x &= x >> 4;
	x &= x >> 2;
	return x & 1;
}

/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *  Rating: 3  
 */
int logicalShift(int x, int n) {
	/*
		By XORing (x >> n), the normal shift, and the (((x & (1 << 31)) >> n) << 1)
		condition, we make sure to set all bits on the left side to 0.
	*/
	int bitmask;
	bitmask = (((x & (1 << 31)) >> n) << 1);
	return ((x >> n) ^ bitmask);
}

/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
	/*
		1 is returned only for x = 0. If x is a negative nonzero number,
		the sign bit must be set but for positive numbers we find the
		two's complement to spot out nonzero values. The OR operation is
		then used to compare x and ((~x + 1)) >> 31). If one side is nonzero,
		>> 31 should leave us with -1, so we add 1 to return 0. If both 
		sides are 0, result is 1.
	*/

	return ((x | (~x + 1)) >> 31) + 1;
}

/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
	/* Return the largest two's complement integer, 0x7fffffff */
	return ~(1 << 31);
}

/* 
 * twosBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: twosBits(5,3) = 0, twosBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int twosBits(int x, int n) {
	/*
		To see if x can be represented as an n-bit, two's complement
		integer, we need to cut off x's upper bits, making x the same
		length as n. Then, if the value matches the original x value, 
		return 1. To check that, we shift x shift bits to the left and back,
		and we do an XOR operation on the result and x's original value. 
	*/
	int shift;
	shift = (~n + 1) + 32;
	return !(x ^ ((x << shift) >> shift)); 
}

/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
	/*
		Following the book's guidelines, we break up the input unsigned
		number into floating point representation. First, we determine exp 
		and frac parts by utilizing bitmasks. Then we do the first check for
		denormalized and special values. For normalized values, we use the
		procedure from the slides of coding the exponent as a biased value 
		(bias=127 for signle precision) and coding the signifigand with the
		leading bit. After more checks for special cases, we make sure our
		biased frac part fits into its according 23 bits. Finally, we return
		the result if the number is positive, else we return its two's complement.
	*/
	int negSign, exp, frac, e, result;

	// Base case for zero
	if (uf == 0) return 0x0;

	// Floating point representation from book & slides
	negSign = uf & (1 << 31);
	exp = (uf >> 23) & 0xFF;
	frac = uf & 0x7FFFFF;

	// Base case for special value
	if (exp == 0xFF) return 0x80000000u;

	// Base case for zero values (denormalized)
	if (exp == 0x0) return 0;
	
	// Calculating E
	e = exp - 127;
	// Adding the leading bit
	result = frac | 0x800000;

	// More special cases for denormalized and special values
	if (e < 0) return 0;
	if (e >= 31) return 0x80000000u;

	// Normalized values single precision procedure
	if (e > 23) result <<= (e - 23);
	else result >>= (23 - e);

	// If negative number, return two's complement
	if (negSign) return ~result + 1;

	// Catching overflow
	if (result >> 31) return 0x80000000;
	else return result;
}