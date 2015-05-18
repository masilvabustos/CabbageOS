
#ifndef __task_h_INCLUDED__
#define __task_h_INCLUDED__

#include <ucontext.h>


struct task_descriptor {
	int 		td_counter;
	void*		td_stack_base;		
	ucontext_t 	td_ucontext;
};

struct task {

	struct task 		*tk_parent;
	struct task 		*tk_sibling;
	struct task 		*tk_child;

	struct task_descriptor 	tk_descriptor;
};

extern struct task_descriptor current_task;

#endif
