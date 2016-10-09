#ifndef	_KERNEL_SCHED_SCHED_H
#define	_KERNEL_SCHED_SCHED_H

enum task_state {
	TASK_UNINIT = 0,
	TASK_SLEEPING = 1,
	TASK_RUNNABLE = 2,
	TASK_ZOMBIE = 3
};

struct context {
};

int sched_init();

#endif

