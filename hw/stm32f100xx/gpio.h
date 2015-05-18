
#ifndef __stm32f100xx_gpio_h_INCLUDED__
#define __stm32f100xx_gpio_h_INCLUDED__

#define __packed__ __attribute__ ((packed))

#include <stdint.h>

typedef struct {
	int bit_0 : 1;
	int bit_1 : 1;
	int bit_2 : 1;
	int bit_3 : 1;
	int bit_4 : 1;
	int bit_5 : 1;
	int bit_6 : 1;
	int bit_7 : 1;
} bita8_t;

union gpio_cr_u {
	uint32_t cr_dword;
	struct {
		int n0_mode : 2;
		int n0_conf : 2;
		int n1_mode : 2;
		int n1_conf : 2;
	} cr_bits[4];
};

union gpio_idr_u {
	uint16_t idr_word;
};		

union gpio_odr_u {
	uint16_t odr_word;
};

union gpio_bsrr_u {
	uint16_t bsrr_word;
};

union gpio_brr_u {
	uint16_t brr_word;
};

union gpio_lckr_u {
	uint32_t lckr_dword;
	uint16_t lckr_word[2];
	bita8_t lckr_bita[4];
};

extern union gpio_cr_u		GPIOA_CRL;
extern union gpio_cr_u		GPIOA_CRH;
extern union gpio_idr_u		GPIOA_IDR;
extern union gpio_odr_u		GPIOA_ODR;
extern union gpio_bsrr_u	GPIOA_BSSR;
extern union gpio_brr_u		GPIOA_BRR;
extern union gpio_lckr_u	GPIOA_LCKR;

#endif
