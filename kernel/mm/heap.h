#ifndef	_KERNEL_MM_HEAP_H
#define	_KERNEL_MM_HEAP_H

#ifndef	_KERNEL_MM_MM_H
#include <mm/mm.h>
#endif

#define	HEAP_START	0xE0000000
#define	MHDR_SIZE	(sizeof(struct mhdr))

enum mhdr_state {
	MHDR_FREE = 0,
	MHDR_ALLOCATED = 1	/* 这段内存已被申请 */
};

struct mhdr {		/* Heap Memory Part Header */
	struct mhdr *prev;
	struct mhdr *next;
	r_t len : 31;
	enum mhdr_state state : MHDR_ALLOCATED;
};

int heap_init();
void *kmalloc(r_t len);
void kfree(void *p);
void chunk_alloc(r_t end);
void chunk_free(struct mhdr *chunk);
void chunk_split(struct mhdr *chunk, r_t len);
void chunk_glue(struct mhdr *chunk);

extern r_t heap_end;
extern struct mhdr *heap_first;

#endif

