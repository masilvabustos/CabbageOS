
#include <task.h>
#include <stdint.h>

struct signal_vectors {
	uint32_t 	 sv_onstackv;
	uint32_t	 sv_siginfov;
	uint32_t 	 sv_maskv[32];
	stack_t		 sv_stack;
	union {
		void   (*sv_sigaction)(int,struct siginfo*,void*);
		void   (*sv_handler)(int);
	};
};

struct task_state {
	clock_t		ts_slice;
	int		ts_type;
	uint8_t		ts_data[];
};

struct task_context;

typedef int	mcontext_t[16];

static volatile mcontext_t *current_link;
//struct task_context	*task_link;
struct task_state	*task_state;


struct task_context {
	struct task_context	*tc_link;
	struct task_state	*tc_state;
	ucontext_t		 tc_ucontext;
};	


extern __swapmcontext(mcontext_t *, mcontext_t *);

void do_task_switch()
{
	mcontext_t mcontext, *saved_link;
	

	switch(switch_action){

	case TASK_SWITCH_ACTION_PREEMPTION:
		
		break;

	case TASK_SWITCH_ACTION_DISPATCH:
		saved_link = current_link;
		__swapmcontext(&mcontext, &task->ts_mcontext);
		current_link = saved_link;
		
	case TASK_SWITCH_ACTION_YIELD:
		__swapmcontext(&current_task->ts_mcontext, current_link);
	}
}

void restore_context()
{
	
	struct cabb_task *task;

	task = sp + (offsetof(struct cabb_task, t_ucontext.uc_mcontext.mc_s.r4));

	
	sp = ready->t_ucontext.uc_stack.ss_sp;
	for(p =  )
		*sp-- = *p--
	restore_mcontext(ready->t_ucontext.uc_mcontext);
}

