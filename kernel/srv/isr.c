#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/srv/isr.h>
#include <lib/ascommon.h>
#include <kernel/sched/sched.h>

void isr_handler_0x30()
{
	(* (short *) 0xC00B8000) = 0x1234;
}

void isr_handler_0x20()
{
	++(* (short *) 0xC00B8000);
	as_outb(0x20, 0x20);
}

