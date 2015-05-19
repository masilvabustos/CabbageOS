
#include <ucontext.h>

#include <task.h>

static void __attribute__ ((naked)) __save_mcontext(int* p)
{
	register int* r4 asm("r4");
	register int  lr asm("lr");
	register int* r0 asm ("r0");

	__push("{r4}");
	r4 = p;
	
	__mrs(r0,apsr);
	__str_post(r0,r4,-4);

	__str_post(lr,r4,-4);
	__str_i(lr,r4,0);

	__stm_wb(db,r4,"{r0-r3,r12}");

	r0 = r4;
	__pop("{r4}");
	__stm_wb(db,r0,"{r4-r11}");

	__str_i(r0-4+sizeof(union mcontext),r0,-4);

	asm volatile ("bx lr");
}

static void __save_context(struct ucontext *ucp, int* sbase, int* sp) 
{
	int* dest = ucp->uc_stack.ss_sp;

	while(sbase >= sp)
		*dest-- = *sbase--;

	ucp->uc_mcontext = (union mcontext*) (dest - sizeof(union mcontext) + 1);

	__save_mcontext(dest);
}


int __attribute__ ((naked)) getcontext(struct ucontext *ucp)
{
	register int* sp asm ("sp");
	__save_context(ucp->uc_stack.ss_sp, current_task.td_ucontext.uc_stack.ss_sp, sp);

	return 0;
}


