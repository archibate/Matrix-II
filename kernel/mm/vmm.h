#ifndef	_KERNEL_MM_VMM_H
#define	_KERNEL_MM_VMM_H

#ifndef	_KERNEL_MM_PMM_H
#include <kernel/mm/pmm.h>
#endif

#define	PAGE_SIZE		0x1000
#define	PAGE_PRESENT		1
#define	PAGE_WRITEABLE		2
#define	PAGE_FORUSER		4
#define	PAGE_MASK		0xFFFFF000
#define	PAGESZ			PAGE_SIZE
#define	KVMBASE			0xC0000000		
#define	PTES_KERN_COUNT		0x40
#define	MBR_PER_PGT		0x400
#define	lin2pteidx(la)		(((la) >> 12) & 0x3FF)
#define	lin2pdeidx(la)		(((la) >> 22) & 0x3FF)
#define	klin2phy(la)		((lin_t) (la) - KVMBASE)
#define	kphy2lin(pa)		((phy_t) (pa) + KVMBASE)
#define	vmm_cpu_invlpg(la) ({ \
			__asm__ ("invlpg (%0)" :: "a" (la)); \
		})
#define	vmm_switch_pde(pde_pa) ({ \
			__asm__ ("mov %0, %%cr3" :: "r" (pde_pa)); \
		})
//#define	vmm_mmap(pde, la, pa, flags) \
	__vmm_mmap((pde), (la), (pa) | (flags))

typedef	r_t addr_t;
typedef	addr_t lin_t;
typedef	addr_t phy_t;
typedef r_t pgd_t;	/* Page Descriptor */
typedef pgd_t ptei_t;	/* PTE Item */
typedef pgd_t pdei_t;	/* PDE Item */
typedef	pgd_t *pgt_t;	/* Page Table */
typedef	ptei_t *pte_t;
typedef	pdei_t *pde_t;

extern pdei_t pde_kern[MBR_PER_PGT];
extern ptei_t ptes_kern[PTES_KERN_COUNT][MBR_PER_PGT];
/* 在 kernel.ld 中通过 PROVIDE 给出，由链接器生成的符号 */

int vmm_init();
void mmap(pde_t pde, lin_t la, phy_t pa_and_flags);
phy_t get_mapping(pde_t pde, lin_t la);

#endif

