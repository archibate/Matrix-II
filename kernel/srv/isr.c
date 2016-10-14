#include <kernel/kernel.h>
#include <unistd.h>
#include <kernel/srv/isr.h>
#include <lib/ascommon.h>
#include <kernel/sched/sched.h>

void isr_handler_0x30()
{
	(* (short *) 0xC00B8000) = 0x1234;
}

void isr_handler_0x20(/*struct context sfrm_ctx0*/r_t stkemparg)
{
	++(* (short *) 0xC00B8000);
	struct context *sfrm_ctx = (struct context *) &stkemparg;
	//__asm__ ("cli;hlt" :: "a" (sfrm_ctx));
	sched_timer_callback(sfrm_ctx);
}

void isr_handler_0x0e()
{
	(* (short *) 0xC00B8002) = 0x1262;
	__asm__ ("mov %%cr2, %%eax;cli;hlt" :: );
}

