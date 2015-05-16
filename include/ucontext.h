
#ifndef __ucontext_h_INCLUDED__
#define __ucontext_h_INCLUDED__

#include <stddef.h>
#include <stdarg.h>
#include <signal.h>

union mcontext {
	struct mc_s {
		int 	r4;
		int 	r5;
		int 	r6;
		int 	r7;
		int 	r8;
		int 	r9;
		int 	r10;
		int 	r11;

		void* 	sp;

		int 	r0;
		int 	r1;
		int 	r2;
		int 	r3;
		int 	r12;
		void* 	lr;
		void* 	pc;
		int 	xpsr;
	} mc_s;
	int mc_v[sizeof(struct mc_s)/sizeof(int)];
};

#define MC_OFF(r) offsetof(union mcontext, mc_s.r)

typedef union mcontext mcontext_t;

#define __unused __attribute__ ((unused))

#define __ldr(r,b,off) asm volatile ("ldr %0,[%1,%2]" : : "r" (r), "r"(b), "i" (off))
#define __str(r,b,off) asm volatile ("str " #r ",[%0,%1]" : : "r"(b), "i" (off)) 
#define __ldm(m,b,r) asm volatile ("ldm" #m " " #b "," #r); 
#define __stm(m,b,r) asm volatile ("stm" #m " " #b "," #r);
#define __push(r) asm volatile ("push " #r)
#define __restore(r,b) __ldr(r,b,MC_OFF(r))
#define __save(r,b) __str(#r,b,offsetof(struct ucontext,uc_mcontext.mc_s.r))
#define __msr(s,r) asm volatile ("msr " #s "," #r)
#define __mrs(r,s) asm volatile ("mrs " #r "," #s)

/* For proper use of this function, the structure which mcp points to must be at top 
 * of the restored context stack.
 * */

static  __attribute__((naked,noreturn)) void __restore_mcontext(const union mcontext*__restrict__ mc) 
{
	register void* r0 asm ("r0");
	register int r1 asm ("r1");
	register int sp asm ("sp");
	register union mcontext* r12 asm ("r12");
	r12 = r0;
#if 0
	__restore(r3,r1);
	__restore(r4,r1);
	__restore(r5,r1);
	__restore(r6,r1);
	__restore(r7,r1);
#endif

	__ldr(r1,r12,MC_OFF(xpsr));
	__msr(apsr_nzcvq,r1);
	__ldr(r1,r12,MC_OFF(pc));

	__ldm(ia,r12!,{r4-r11});
	asm volatile ("ldr r2,[r12],#4");
	asm volatile ("mov sp,r2");
	__push({r1});
	__ldm(ia,r12!,{r0-r3});
	asm volatile ("ldr r12,[r12]");

	/* For some oscure reason we shall know the size of this function. ;) */
	asm volatile (	"__restore_mcontext_end__:\n	"
		      	"	pop {pc}		");
}

struct ucontext {
	
	struct ucontext* uc_link;
	
	stack_t uc_stack;
	
	mcontext_t uc_mcontext;
};

typedef struct ucontext ucontext_t;

#if 1
__attribute__ ((naked)) void __save_mcontext(union mcontext *__restrict__ mcp) 
{
	register void* r0 asm ("r0");
	register int r1 asm ("r1");
	register int sp asm ("sp");
	register int lr asm ("lr");

	r0 = mcp;
#if 0
	__str(sp,r0,MC_OFF(sp));
#endif
	__mrs(r1,apsr);
	__str(r1,r0,MC_OFF(xpsr));
	__str(lr,r0,MC_OFF(pc));

	__stm(ia,r0!,{r4-r11});
#if 1
	asm volatile ("str sp,[r0],#8"); // jump r0
#else
	__stm(ia,r0!,{sp});
#endif
	asm volatile ("stmia r0!,{r1-r3,r12}");
	
	asm volatile ("bx lr");	
}
#endif

int getcontext(ucontext_t *ucp)
{
	__save_mcontext(&ucp->uc_mcontext);

	return 0;
}

int setcontext(ucontext_t *ucp)
{
	__restore_mcontext(&ucp->uc_mcontext);
	return -1;
}

int __attribute__ ((naked)) swapcontext(ucontext_t *__restrict__ oucp, const ucontext_t *__restrict__ ucp)
{
	register void* lr asm ("lr");
	void* t = lr;
	__save_mcontext(&oucp->uc_mcontext);
	oucp->uc_mcontext.mc_s.r0 = 0;
	oucp->uc_mcontext.mc_s.lr = t;
	__restore_mcontext(&ucp->uc_mcontext);
}

void makecontext(ucontext_t* ucp, void(*func)(), int argc, ...)
{
	va_list ap;
	int *p, *sp;

	int register_argc = (argc<4) ? argc : 4;

	argc -= register_argc;

	sp = ucp->uc_stack.ss_sp - register_argc;
	p = &ucp->uc_mcontext.mc_s.r0;
	
	while(register_argc--)
		*p++ = va_arg(ap, int);

	va_start(ap, argc);
	while(argc--)
		*sp-- = va_arg(ap, int);

	ucp->uc_mcontext.mc_s.pc = func;
	ucp->uc_mcontext.mc_s.sp = sp;
}

	
			
	
			
#endif

