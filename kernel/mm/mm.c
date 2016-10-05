#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/mm/mm.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>
#include <kernel/mm/heap.h>

int mm_init()
{
	pmm_init();
	vmm_init();
	heap_init();
	return 0;
}

