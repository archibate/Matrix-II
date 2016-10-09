#ifndef _LIB_MEMORY_H
#define _LIB_MEMORY_H

void *memcpy(void *src, void *dst, size_t size);
void *memset(void *buf, int data, size_t size);
int memcmp(void *src, void *dst, size_t size);
void *memmove(void *src, void *dst, size_t size);

#endif

