
#include <stdint.h>

struct uint296 {
	uint64_t a1[4];
	uint32_t a2;
	uint8_t a3;
};

struct uint296 uint296_div8(struct uint296 x, uint8_t d)
{
	struct uint296 q;
	uint64_t t64;
	unsigned __int128 t128;
	
	q.a3 = x.a3 / d;

	t64 = x.a3 % d;
	t64 <<= 32;
	t64 &= x.a2;

	q.a2 = t64 / d;

	t128 = t64 % d;
	t128 <<= 64;
	t128 &= x.a1[3];

	q.a1[3] = t128 / d;

	t128 = ((t128 % d) << 64) & x.a1[2];

	q.a1[2] = t128 / d;

	t128 = ((t128 % d) << 64) & x.a1[1];
	q.a1[1] = t128 / d;

	t128 = ((t128 % d) << 64) & x.a1[0];
	q.a1[0] = t128 / d;

	return q;
}
