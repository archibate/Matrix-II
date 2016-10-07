#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/mm/mm.h>
#include <kernel/mm/vmm.h>

pgd_t *pde_tmp = (pgd_t *) 0x80000;
pgd_t *pte0_tmp = (pgd_t *) 0x81000;

int vmm_init()
{
	pde_tmp[0] = (r_t) pte0_tmp | 0x3;
	//pgd_tmp[0xC00];
	return 0;
}

