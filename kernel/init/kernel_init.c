
#include <stdint.h>
#include <task.h>

void kernel_init()
{
	extern uint8_t __data_lma__, __data_vma__, _kdata_start, _kdata_end, _kbss_start, _kbss_end;

	uint8_t *src, *dst;

	src = &__data_lma__ + (&_kdata_start - &__data_vma__) ;
	dst = &_kdata_start;
	while(dst <= &_kdata_end)
		*dst++ = *src++;

	for (dst = &_kbss_start; dst != &_kbss_end; ++dst)
		*dst = 0;

}


