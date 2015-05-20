
#ifndef __signal_h_INCLUDED__
#define __signal_h_INCLUDED__

#include <stddef.h>

struct sig_stack {

	void* ss_sp;
	size_t ss_size;
	int flags;
};

typedef struct sig_stack stack_t;

#endif
