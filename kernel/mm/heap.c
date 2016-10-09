#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/mm/mm.h>
#include <kernel/mm/heap.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>

r_t heap_end = HEAP_START;
struct mhdr *heap_first;

static void play_heap();

int heap_init()
{
	heap_first = NULL;
	play_heap();
	return 0;
}

static void play_heap()
{
	void *p = kmalloc(50);
	kfree(p);
	p = NULL;
}

void *kmalloc(r_t len)
{
	//assert(len == 50);
	//(* (short *) 0xC00B8000) = 0x1234;
	//assert(0);
	addr_t ret_chk_addr = 0;
	len += MHDR_SIZE;
	/* 加上管理头的长度，因为管理头和内存块是在一起的 */

	struct mhdr *curr = heap_first;
	struct mhdr *prev = NULL;

	while (curr) {
		if (curr->state == MHDR_FREE
				&& curr->len >= len) {
			chunk_split(curr, len);
			curr->state = MHDR_ALLOCATED;
			ret_chk_addr = (r_t) curr + MHDR_SIZE;
			goto out;
		}
		prev = curr;
		curr = curr->next;
	}
	r_t chunk_start;
	if (prev) {
		chunk_start = (r_t) prev + prev->len;
	} else {
		chunk_start = HEAP_START;
		heap_first = (struct mhdr *) chunk_start;
	}

	chunk_alloc(chunk_start + len);
	curr = (struct mhdr *) chunk_start;
	curr->prev = prev;
	curr->next = NULL;
	curr->state = MHDR_ALLOCATED;
	curr->len = len;
	if (prev)
		prev->next = curr;
	ret_chk_addr = chunk_start;
out:
	return (void *) (ret_chk_addr + MHDR_SIZE);
}

void kfree(void *p)
{
	struct mhdr *hdr = (struct mhdr *) ((r_t) p - MHDR_SIZE);
	hdr->state = MHDR_FREE;
	chunk_glue(hdr);
}

void chunk_alloc(r_t end)
{
	while (end > heap_end) {
		r_t page_pa = pmm_alloc_page();
		mmap(pde_kern, heap_end,
				page_pa | PAGE_PRESENT | PAGE_WRITEABLE);
		heap_end += PAGE_SIZE;
	}
}

void chunk_free(struct mhdr *chunk)
{
	if (!chunk->prev)
		heap_first = NULL;
	else
		chunk->prev->next = NULL;
	while (heap_end - PAGE_SIZE >= (r_t) chunk) {
		heap_end -= PAGE_SIZE;
		r_t page_pa;
		page_pa = get_mapping(pde_kern, heap_end);
		munmap(pde_kern, heap_end);
		pmm_free_page(page_pa);
	}
}

void chunk_split(struct mhdr *chunk, r_t len)
{
	if (unlikely(chunk->len - len > sizeof(struct mhdr)))
		return;
	struct mhdr *new_chunk = (struct mhdr *)
		((r_t) chunk + chunk->len);
	new_chunk->prev = chunk;
	new_chunk->next = chunk->next;
	new_chunk->state = MHDR_FREE;
	new_chunk->len = chunk->len - len;
	chunk->next = new_chunk;
	chunk->len = len;
}

void chunk_glue(struct mhdr *chunk)
{
	struct mhdr *next = chunk->next;
	struct mhdr *prev = chunk->prev;
	if (next && next->state == MHDR_FREE) {
		chunk->len += next->len;
		if (next->next)
			next->next->prev = chunk;
		next = chunk->next = next->next;
	}
	if (prev && prev->state == MHDR_FREE) {
		prev->len += chunk->len;
		prev->next = next;
		if (next)
			next->prev = prev;
		chunk = prev;
	}
	if (!next)
		chunk_free(chunk);
}

