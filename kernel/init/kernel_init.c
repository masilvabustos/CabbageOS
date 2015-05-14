
void kernel_init()
{
	extern uint8_t __init_data_start, __init_data_end, __data_start, __bss_start, __bss_end;

	uint8_t *src, dst;

	src = &__init_data_start;
	dst = &__data_start;
	while(src != &__init_data_end)
		*dst++ = *src++;

	for (dst = &__bss_start; dst != &__bss_end; ++dst)
		*dst = 0;

}


