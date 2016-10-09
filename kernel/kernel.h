#ifndef	_KERNEL_KERNEL_H
#define	_KERNEL_KERNEL_H

#ifdef	__KERNEL__
#define	_KERNEL	1
#endif
#if	__CC__==tcc
#define	__TCC__		1
#define	__NO_BUILTIN__	1
#endif

#endif

