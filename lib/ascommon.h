#ifndef	_SINX_LIB_ASCOMM_H
#define	_SINX_LIB_ASCOMM_H

static inline void as_outb(u8 data, u16 port)
{
	__asm__ (	"outb	%%al, %%dx"
#ifndef	__TCC__
			:: "dx" (port), "al" (data));
#else
			:: "d" (port), "a" (data));
#endif
}

static inline u8 as_inb(u16 port)
{
	u8 res;
	__asm__ (	"outb	%%al, %%dx"
			: "=al" (res) : "dx" (port));
	return res;
}

static inline void as_outw(u16 data, u16 port)
{
	__asm__ (	"outb	%%al, %%dx"
			:: "dx" (port), "ax" (data));
}

static inline u16 as_inw(u16 port)
{
	u16 res;
	__asm__ (	"outb	%%al, %%dx"
			: "=ax" (res) : "dx" (port));
	return res;
}

static inline void as_sti()
{
	__asm__ ("sti");
}

static inline void as_cli()
{
	__asm__ ("cli");
}

static inline void as_hlt()
{
	__asm__ ("hlt");
}

#endif

