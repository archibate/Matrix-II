#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/init/init.h>
#include <kernel/sched/sched.h>
#include <kernel/mm/mm.h>

void *mboot_ptr = 0;
void *isr_entry_table[256] = {0};

int init()
{
	mm_init();
	sched_init();
	return 0;
}

