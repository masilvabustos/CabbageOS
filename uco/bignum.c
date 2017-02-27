
#include <stdint.h>

#define __PACKED__ __attribute__ ((packed))

typedef unsigned __int128 uint128_t;

struct u296 {
	uint64_t b0_255[4];
	unsigned long int b256_295 : 40;
} __PACKED__;

uint8_t __div_u296_u8(struct u296 *x, uint8_t d)
{
        uint128_t r;
	int i;
	
	r = x->b256_295;

	x->b256_295 = r / d;

	for (i=3; i>= 0; --i) {
		r %= d;
		r <<= 64;
		r |= x->b0_255[i];
		
		x->b0_255[i] = r/d;
	}

	r %= d;

        return r;
}


void __mul_u296_u8(struct u296 *x, uint8_t f)
{
        uint128_t p;
	int i;
	
	p = 0;

	for (i=0; i<4; ++i) {
		p += (uint128_t) x->b0_255[i] * f;
		x->b0_255[i] = p;
		p >>= 64;
        };

	p += x->b256_295 * f;
	x -> b256_295 = p;

}


#include <stdio.h>

void printu296(struct u296 x)
{
	printf("%02hhx:%08x:%016lx:%016lx:%016lx:%016lx\n", (uint8_t) (x.b256_295 >> 32), (uint32_t) x.b256_295, x.b0_255[3], x.b0_255[2], x.b0_255[1], x.b0_255[0] );
}

int main()
{
	struct u296 x;
	unsigned __int128 p;
	unsigned int f = 253;
	
	printf("sizeof(u296) = %lu\n", sizeof(struct u296)*8);

	x = (struct u296) {{0xabcd111212211, 0xaaabbbcccaa3322, 0x3342351ddcc42344, 0x232adad1a3232323}, 0xabcd};

	printu296(x);
	p = (uint128_t) x.b0_255[0];
	
	__mul_u296_u8(&x, f);
	printu296(x);
	
	
	//	p *= f;
	//printf("esperado: %016lx:%016lx\n", (uint64_t) (p >> 64), (uint64_t) p); 



	__div_u296_u8(&x, f);

	printu296(x);
	
	return 0;
}
