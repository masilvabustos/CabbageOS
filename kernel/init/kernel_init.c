
#include <stdint.h>

#include <stm32f100xx.h>

void kernel_init(void);


void kernel_init()
{
	extern uint8_t __init_data_start, __init_data_end, __data_start, __bss_start, __bss_end;
	uint32_t cr, mask, set_mask, reset_mask, conf;

	uint8_t *src, *dst;

	src = &__init_data_start;
	dst = &__data_start;
	while(src != &__init_data_end)
		*dst++ = *src++;

	for (dst = &__bss_start; dst != &__bss_end; ++dst)
		*dst = 0;

	RCC_APB2ENR |= 0x10;

	GPIOC_CRH = 0x44444422;

	GPIOC_ODR = 0xffffffff;

	while(1);

}


