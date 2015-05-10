
#include "isr.h"

extern void* _initial_sp;

__attribute__ ((section(".isr_vector"))) isr_t isr_vector[] = {
	(isr_t) _initial_sp,
	_start
};
	
