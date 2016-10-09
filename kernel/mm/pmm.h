#ifndef	_KERNEL_MM_PMM_H
#define	_KERNEL_MM_PMM_H

#ifndef	_KERNEL_MM_VMM_H
#include <kernel/mm/vmm.h>
#endif

extern r_t pmm_pa_stack[100];
extern r_t *pmm_pa_stack_top;
extern u8 pmm_free_space_start[];
extern u8 pmm_free_space_end[];

int pmm_init();
phy_t pmm_alloc_page();
void pmm_free_page(phy_t pa);

#endif

