
#include <task.h>

struct cabb_task* parent_task;
struct cabb_task* current_task;
enum sched_op {
	DISPATCH
	PREEMPT
} switchop;

void sched_switch(int* sp)
{
	struct cabb_task *task;

	task = sp + (offsetof(struct cabb_task, t_ucontext.uc_mcontext.mc_s.r4));

	mother = task;
	
	sp = ready->t_ucontext.uc_stack.ss_sp;
	for(p =  )
		*sp-- = *p--
	restore_mcontext(ready->t_ucontext.uc_mcontext);
}

