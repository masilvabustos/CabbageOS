
#include "isr.h"

extern int _initial_sp;
extern void _start();

__attribute__ ((section(".isr_vector"))) isr_t isr_vector[] = {
	(isr_t) &_initial_sp,
	_start
};
	
