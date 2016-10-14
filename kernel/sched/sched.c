#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/sched/sched.h>
#include <kernel/init/init.h>
#include <lib/memory.h>
#include <lib/ascommon.h>

struct task *running_proc_head = NULL;
struct task *waiting_proc_head = NULL;
struct task *curr_task = NULL;
pid_t max_pid = 1;

static void play_sched();
static int hello_thread_main(void);
static int idle_thread_main(void);

int sched_init()
{
	/* 进程的信息结构体位于当前执行流栈的最底端 */
	curr_task = (struct task *) KERNEL_STACK_BOTTOM;
	curr_task->state = TASK_RUNNABLE;
	curr_task->pid = max_pid++;
	curr_task->mm = NULL;
	curr_task->ctx.esp = (r_t) KERNEL_STACK_TOP;
	curr_task->ctx.efl = 0x200;
	curr_task->ctx.eip = (r_t) idle_thread_main;
	curr_task->ctx.gs = curr_task->ctx.fs =
		curr_task->ctx.es = curr_task->ctx.ds =
		curr_task->ctx.ss = 0x20;
	curr_task->ctx.cs = 0x18;
	curr_task->next = curr_task;
	running_proc_head = curr_task;
	play_sched();
	return 0;
}

static void play_sched()
{
	kthread_create(hello_thread_main, NULL);
	schedule();
	(* (short *) 0xC00B8014) = 0x1238;
	assert(0);
}

static int hello_thread_main(void)
{
	//int val = 1234;
	for (;;) {
		//assert(val == 1234);
		++(* (short *) 0xC00B8010);
		(* (short *) 0xC00B8012) = 0x1234;
		__asm__ ("sti;hlt");
		(* (short *) 0xC00B8014) = 0x1234;
	}
	return 0;
}

static int idle_thread_main(void)
{
	//int val = 1235;
	for (;;) {
		//assert(val == 1235);
		++(* (short *) 0xC00B800E);
		(* (short *) 0xC00B8012) = 0x1235;
		__asm__ ("sti;hlt");
	}
	return 0;
}

void schedule()
{
	assert(curr_task);
	assert(curr_task->next);
	switch_to_task(curr_task->next);
}

void sched_timer_callback(struct context *sfrm_ctx)
{
	memcpy(&curr_task->ctx, sfrm_ctx, sizeof(struct context));
	as_outb(0x20, 0x20);
	schedule();
}

void switch_to_task(struct task *next)
{
	//assert(curr_task != next);
	curr_task = next;
	__switch_to_context(&curr_task->ctx);
}

pid_t kthread_create(int (* entry)(void), void *arg)
{
	struct task *new_task = (struct task *) kmalloc(STACK_SIZE);
	assert(new_task);
	memset(new_task, 0, sizeof(struct task));
	new_task->state = TASK_RUNNABLE;
	new_task->pid = max_pid++;
	new_task->mm = NULL;
	r_t *stack_top = (r_t *) ((r_t) new_task + STACK_SIZE);
	*--stack_top = (r_t) arg;
	*--stack_top = (r_t) kthread_exit_holder;
	//*--stack_top = (r_t) entry;
	new_task->ctx.esp = (r_t) stack_top;
	new_task->ctx.efl = 0x200;
	new_task->ctx.eip = (r_t) entry;
	new_task->ctx.gs = new_task->ctx.fs =
		new_task->ctx.es = new_task->ctx.ds =
		new_task->ctx.ss = 0x20;
	new_task->ctx.cs = 0x18;
	new_task->next = running_proc_head;
	struct task *tail = running_proc_head;
	assert(tail);
	while (tail->next != running_proc_head)
		tail = tail->next;
	tail->next = new_task;
	return new_task->pid;
}

void kthread_exit_holder()
{
	//register r_t eax_val asm ("eax");
	assert(0);
}

