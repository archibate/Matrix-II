#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/init/init.h>
#include <kernel/mm/mm.h>
#include <kernel/mm/vmm.h>
#include <lib/memory.h>

int vmm_init()
{
	ndx_t i;
	/* PTE 从指向 0xC0000000 开始 */
	for (i = 0; i < PTES_KERN_COUNT; ++i)
		pde_kern[i + lin2pdeidx(KVMBASE)] =
			klin2phy(ptes_kern[i])
			| PAGE_PRESENT | PAGE_WRITEABLE;
	
	pte_t pte0_kern = (pte_t) ptes_kern;
	//for (i = 0, pa = 0; pa < (PTES_KERN_COUNT * MBR_PER_PGT) << 12;
	//		pa += PAGESZ)
	//	pte0_kern[i] = pa | PAGE_PRESENT | PAGE_WRITEABLE;
	/* 不映射第 0 页便于跟踪 NULL 指针 */
	for (i = 0; i < PTES_KERN_COUNT * MBR_PER_PGT; ++i)
		pte0_kern[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITEABLE;
	/* (i << 12) 是物理地址 */
	vmm_switch_pde(klin2phy(pde_kern));
	/*mmap(pde_kern, 0xC0000000, 0x100000 | PAGE_PRESENT);
	(* (short *) 0xC00B8000) = 0x6173;
	if ((* (int *) 0xC0000000) == (* (int *) 0xC0100000))
		if (get_mapping(pde_kern, 0xC0000000) == 0x100000)
			(* (short *) 0xC00B8000) = 0x6174;*/
	/*assert(get_mapping(pde_kern, 0xC0100000) == 0x100000);
	(* (short *) 0xC00B8000) = 0x6174;
	assert(0);*/
	return 0;
}

void mmap(pde_t pde, lin_t la, pgd_t pgd)
{
	ndx_t pdeidx = lin2pdeidx(la);
	ndx_t pteidx = lin2pteidx(la);
	pte_t pte;
	if (likely(page_present(pde[pdeidx]))) {
		/* 线性地址 la 所对应的 PTE 存在 */
		pte = (pte_t) kphy2lin(page_mask(pde[pdeidx]));
	} else {
		/* 对应的 PTE 还不存在，我们来添加一个 */
		pte = (pte_t) pmm_alloc_page();	/* PMM 返回的是物理地址 */
		pde[pdeidx] = (r_t) pte | PAGE_PRESENT | PAGE_WRITEABLE;
		/* 转为内核线性地址以清零 */
		pte = (pte_t) kphy2lin(pte);
		memset(pte, 0, PAGESZ);
	}
	pte[pteidx] = pgd;
	vmm_cpu_invlpg(la);
}

phy_t get_mapping(pde_t pde, lin_t la)
{
	ndx_t pdeidx = lin2pdeidx(la);
	ndx_t pteidx = lin2pteidx(la);
	pte_t pte;
	if (unlikely(!page_present(pde[pdeidx])))	/* PTE 不存在 */
		return 0;
	/* 转为内核线性地址以访问 */
	pte = (pte_t) kphy2lin(pde[pdeidx] & PAGE_MASK);
	if (unlikely(!page_present(pte[pteidx])))	/* 页不存在 */
		return 0;
	return page_mask(pte[pteidx]);
}

void munmap(pde_t pde, lin_t la)
{
	ndx_t pdeidx = lin2pdeidx(la);
	ndx_t pteidx = lin2pteidx(la);
	if (unlikely(!page_present(pde[pdeidx])))
			return;
	pte_t pte = (pte_t) kphy2lin(page_mask(pde[pdeidx]));
	pte[pteidx] = 0;
	vmm_cpu_invlpg(la);
}

