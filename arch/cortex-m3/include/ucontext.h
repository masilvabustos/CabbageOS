
#ifndef __ucontext_h_INCLUDED__
#define __ucontext_h_INCLUDED__

#include <stddef.h>
#include <stdarg.h>
#include <signal.h>

union mcontext {
	struct mc_s{

		int 	r4;
		int 	r5;
		int 	r6;
		int 	r7;
		int 	r8;
		int 	r9;
		int 	r10;
		int 	r11;

		int 	r0;
		int 	r1;
		int 	r2;
		int 	r3;
		int 	r12;
		short* 	lr;
		short* 	pc;
		int 	xpsr;
	} s;
	int a[sizeof(struct mc_s)/sizeof(int)];
};

#define MC_OFF(r) offsetof(union mcontext, mc_s.r)

typedef union mcontext mcontext_t;

#define __unused __attribute__ ((unused))

#define __ldr_i(r,b,off) asm volatile ("ldr %0,[%1,%2]" : "=r" (r) : "r"(b), "i" (off))
#define __str_i(r,b,off) asm volatile ("str %0,[%1,%2]" : : "r"(r), "r"(b), "i" (off)) 
#define __str_post(r,b,off) asm volatile ("str %1,[%0],%2" : "+r" (b) : "r" (r), "i"(off))
#define __ldm(m,b,rlst) asm volatile ("ldm" #m " %0," rlst :: "r"(b)); 
#define __ldm_wb(m,b,rlst) asm volatile ("ldm" #m " %0," rlst : "+r"(b)); 
#define __stm(m,b,rlst) asm volatile ("stm" #m " %0," rlst :: "r"(b))
#define __stm_wb(m,b,rlst) asm volatile ("stm" #m " %0!," rlst : "+r" (b))

#define __push(r) asm volatile ("push " r)
#define __pop(r) asm volatile ("pop " r)
#define __restore(r,b) __ldr(r,b,MC_OFF(r))
#define __save(r,b) __str(#r,b,offsetof(struct ucontext,uc_mcontext.mc_s.r))
#define __msr(s,r) asm volatile ("msr " #s ",%0": : "r"(r))
#define __mrs(r,s) asm volatile ("mrs %0," #s : "=r" (r))

/* For proper use of this function, the structure which mcp points to must be at top 
 * of the restored context stack.
 * */
#if 0
static  __attribute__((naked,noreturn)) void __restore_mcontext() 
{
	register int r1 asm ("r1");
	register int r12 asm ("r12");
	register int* r0 asm ("r0");
	register int lr asm ("lr");
	register int* sp asm ("sp");

	asm volatile ("mov r12,r0");

	__ldr_i(r0,r12,MC_OFF(xpsr));	
	__msr(apsr_nzcvq,r0);

	__ldr_i(r0,r12,MC_OFF(r12));
	__ldr_i(r1,r12,MC_OFF(pc));
	__push("{r0,r1}");
	
	__ldr_i(sp,r12,0);
	asm volatile ("add r12,r12,4");
	__ldm_wb(ia,r12,"{r4-r11}");
	__ldm_wb(ia,r12,"{r0-r3}");
	__ldr_i(lr,r12,4);

	/* For some oscure reason we shall know the size of this function. ;) */
	asm volatile (	"__restore_mcontext_end__:\n"
		      	"	pop {r12,pc}");
}
	
#endif

struct ucontext {
	
	struct ucontext* uc_link;
	
	stack_t uc_stack;
	
	mcontext_t uc_mcontext;
};

typedef struct ucontext ucontext_t;

#if 1

static void __attribute__ ((naked)) __save_mcontext(int* p)
{
	register int* r1 asm("r1");
	register int  lr asm("lr");
	//register int* r0 asm ("r0");

	__stm_wb(ia,p,"{r4-r11}");
	//r0 = p;
	__stm(ia,p,"{r0-r3,r12,lr}");
	asm volatile ("add %0,%0,#24" : "+r"(p));
	__str_post(lr,p,4);
	__mrs(r1,apsr);
	__str_i(r1,p,0);
	asm volatile ("bx lr");
}

static int __attribute__ ((naked)) getcontext(ucontext_t *ucp)
{
	extern stack_t _uc_current_stack;
	register int* sp asm("sp");
	ucp->uc_stack.ss_sp 	= (void*)sp;
	ucp->uc_stack.ss_size 	= _uc_current_stack.ss_sp - (void*)sp;
	__save_mcontext(&ucp->uc_mcontext.a[0]);
}

#endif

#if 0

int setcontext(ucontext_t *ucp)
{
	__restore_mcontext(&ucp->uc_mcontext);
	return -1;
}
#endif
#if 0
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
			
	
			
#endif

