// Square root.

#include <stdint.h>

// To configure the square root algorithm:
// Select a bit length, and set FP_BITS to it.
// Set fixed_t to be that length, and long_fixed_t to be twice the length.
#define FP_BITS 16
typedef uint16_t fixed_t;
typedef uint32_t long_fixed_t;
// Next, select how many bits are after the binary point.
// Put this value in FP_SHIFT.
#define FP_SHIFT 4

#define FP_FORMAT(x) ((fixed_t)((x)/(float)(1<<FP_SHIFT)))
#define FP_ADD(x, y) ((x) + (y))
#define FP_MUL(x, y) (((long_fixed_t)(x) * (long_fixed_t)(y)) >> (FP_SHIFT))

// Takes 16 16x16->32 multiplies.
// The AVR docs claim that they have a 105 cycle routine for this.
// So, maybe about 2000 cycles per square root?
static fixed_t square_root(fixed_t x) {
	fixed_t mask_next = 1<<(FP_BITS-1), accum = 0;
	long_fixed_t target = ((long_fixed_t) x) << FP_SHIFT;
	uint8_t i;
	for (i = 0; i < FP_BITS; i++) {
		fixed_t mid = accum | mask_next;
		long_fixed_t sq = ((long_fixed_t) mid) * ((long_fixed_t) mid);
		if (sq <= target)
			accum |= mask_next;
		mask_next >>= 1;
	}
	return accum;
}

