
#include <signal.h>

struct sigaction sigaction_tbl[];

void sig_deliver_signal()
{

	
}

void sig_dispatch_signal()
{

}

void sig_dispatch_handler()
{
	
}

extern uint8_t* __sig_restore_context, __sig_restore_context_end;

void sig_sched_handler(ucontext_t *ucp, void (*handler)(int))
{
	int* sp = ucp->uc_mcontext.mc_s.sp;
	uint8_t* p, d;

	d = (uint8_t*) sp;
	p = __sig_restore_context_end;
	while (p >= __sig_restore_context)
		*d-- = *p--;

	sp = d;

	*sp-- = 0; /* PSR */
	*sp-- = handler | 1; /* PC */
	*sp-- = d; /* LR */
	*sp-- = ucp->uc_mcontext.mc_s.r12;
	*sp-- = 0;
	*sp-- = 0;
	*sp-- = 0;
	*sp-- = signal;	
}

