#include <kernel/init/init.h>

void centry(void)
{
	//(* (short *) 0xC00B8000) = 0x4444;
	init();
	__asm__ ("cli;hlt");
}

