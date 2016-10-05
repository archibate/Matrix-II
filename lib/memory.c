#include <kernel/kernel.h>
#include <unistd.h>
#include <lib/memory.h>

void *memcpy(void *src, void *dst, size_t size)
{
	u8 *dst8 = dst;
	u8 *src8 = src;
	while (size--)
		*dst8++ = *src8++;
	return dst;
}

void *memset(void *buf, int data, size_t size)
{
	u8 *buf8 = buf;
	while (size--)
		*buf8++ = data;
	return buf;
}

int memcmp(void *src, void *dst, size_t size)
{
	u8 *dst8 = dst;
	u8 *src8 = src;
	while (size--)
		if (unlikely(*dst8++ != *src8++))
			return TRUE;
	return FALSE;
}

void *memmove(void *src, void *dst, size_t size)
{
	//if (unlikely(0));
	return dst;
}

