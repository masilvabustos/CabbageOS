
#include <ucontext.h>

#include <task.h>


#if 0
static void __save_context(struct ucontext *ucp, int* sbase, int* sp) 
{
	int* dest = ucp->uc_stack.ss_sp;

	while(sbase >= sp)
		*dest-- = *sbase--;

	ucp->uc_mcontext = (union mcontext*) (dest - sizeof(union mcontext) + 1);

	__save_mcontext(dest);
}
#endif


