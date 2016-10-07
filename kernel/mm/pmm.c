#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/mm/mm.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>

r_t pmm_stack[100];	/* 保存的是空闲的内存地址 */
r_t *pmm_stack_top = pmm_stack;
size_t phy_page_count = 0;

void nope_func(void*x){}
#define	UNUSED(x)	/*(*(void(*)(void))*/nope_func/*)*/((void*)(r_t)(x));

int pmm_init()
{
	u32 page_addr = 0x800000 - PAGESZ;
	while (page_addr >= 0x400000) {
		pmm_free_page(page_addr);
		page_addr -= PAGESZ;
		++phy_page_count;
	}
	r_t addr = pmm_alloc_page();
	//UNUSED(addr);
	assert(addr);
	return 0;
}

r_t pmm_alloc_page()
{
	return *pmm_stack_top--;
}

void pmm_free_page(r_t addr)
{
	*++pmm_stack_top = addr;
}

