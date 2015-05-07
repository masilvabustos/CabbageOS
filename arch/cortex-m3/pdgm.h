
#ifndef __PDGM_H_INCLUDED__
#define __PDGM_H_INCLUDED__

#include <stdint.h>

#include "up.h"
#include <syscalls.h>

struct pdgm_launchpad {
	uint16_t lp_trampoline[2];
	int (*lp_landing_pad)();
	struct up_context * lp_upcontext;
};

extern void __attribute__ ((noreturn)) __do_landing_pad(int, int(*)());

#define PDGM_MANGLE(name) __pdgm_##name

#define PDGM_SECTION_NAME ".pdgm_launchpad"

#define __pdgm_launchpad__ __attribute__ ((section(PDGM_SECTION_NAME),naked))

#define EXPORT_LAUNCHPAD(name, flags) __pdgm_launchpad__ int PDGM_MANGLE(name)() { \
	asm volatile ("svc %0\n	bx lr" :: "i" (SYS_LAUNCHPAD)); \
	__do_landing_pad(flags, name);\
}

#endif
