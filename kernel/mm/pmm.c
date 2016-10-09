#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/mm/mm.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>

r_t pmm_pa_stack[100];		/* 保存的是空闲的内存页地址 */
r_t *pmm_pa_stack_top = pmm_pa_stack;
size_t phy_page_count = 0;

//static inline void nope_func(void *x) {}
//#define	UNUSED(x)	/*(*(void(*)(void))*/nope_func/*)*/((void*)(r_t)(x));

int pmm_init()
{
	phy_t page_addr = klin2phy(pmm_free_space_end) - PAGESZ;
	while (page_addr >= klin2phy(pmm_free_space_start)) {
		pmm_free_page(page_addr);
		page_addr -= PAGESZ;
		++phy_page_count;
	}
	/*r_t addr = pmm_alloc_page();
	//UNUSED(addr);
	assert(addr);*/
	return 0;
}

phy_t pmm_alloc_page()
{
	return *pmm_pa_stack_top--;
}

void pmm_free_page(phy_t pa)
{
	*++pmm_pa_stack_top = pa;
}

