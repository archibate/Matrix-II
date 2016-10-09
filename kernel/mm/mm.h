#ifndef	_KERNEL_MM_MM_H
#define	_KERNEL_MM_MM_H

typedef	r_t addr_t;
typedef	addr_t lin_t;
typedef	addr_t phy_t;

#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>
#include <kernel/mm/heap.h>

int mm_init();

#endif

