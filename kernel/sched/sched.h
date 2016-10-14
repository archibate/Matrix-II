#ifndef	_KERNEL_SCHED_SCHED_H
#define	_KERNEL_SCHED_SCHED_H

#ifndef _KERNEL_MM_MM_H
#include <kernel/mm/mm.h>
#endif

typedef u32 pid_t;

enum task_state {
	TASK_UNINIT = 0,
	TASK_SLEEPING = 1,
	TASK_RUNNABLE = 2,
	TASK_ZOMBIE = 3
};

struct context {
	r_t gs, fs, es, ds,
	    edi, esi, ebp, new_esp, ebx, edx, ecx, eax,
	    ret_addr, unused,
	    eip, cs, efl, esp, ss;
};

struct task_mm {
	pde_t pde;
};

struct task {
	struct context ctx;
	struct task_mm *mm;
	struct task *next;
	void *stack;
	pid_t pid;
	enum task_state state;
};

extern struct task *running_proc_head;
extern struct task *waiting_proc_head;
extern struct task *curr_task;
extern pid_t max_pid;

int sched_init();
void schedule();
void sched_timer_callback(struct context *sfrm_ctx);
void switch_to_task(struct task *next);
extern void __switch_to_context(struct context *next_ctx);
pid_t kthread_create(int (* entry)(void), void *arg);
void kthread_exit_holder();


#endif

