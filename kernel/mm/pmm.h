#ifndef	_KERNEL_MM_PMM_H
#define	_KERNEL_MM_PMM_H

int pmm_init();
r_t pmm_alloc_page();
void pmm_free_page(r_t addr);

#endif

