
#include "isr.h"

extern int __end_of_ram;
extern void _start();

__attribute__ ((section(".isr_vector"))) isr_t isr_vector[] = {
	(isr_t) &__end_of_ram,
	_start
};
	
