#ifndef	_KERNEL_MM_MM_H
#define	_KERNEL_MM_MM_H

#define	KERNEL_STACK_TOP	0xC03F8000
#define	KERNEL_STACK_SIZE	0x10000
#define	KERNEL_STACK_BOTTOM	(KERNEL_STACK_TOP - KERNEL_STACK_SIZE)
#define	STACK_SIZE		KERNEL_STACK_SIZE

typedef	r_t addr_t;
typedef	addr_t lin_t;
typedef	addr_t phy_t;

#ifndef	_KERNEL_MM_PMM_H
#include <kernel/mm/pmm.h>
#endif
#ifndef	_KERNEL_MM_VMM_H
#include <kernel/mm/vmm.h>
#endif
#ifndef	_KERNEL_MM_HEAP_H
#include <kernel/mm/heap.h>
#endif

int mm_init();

#endif

