/*
 * kernel Standard C library : memory
 */

typedef unsigned int size_t;
typedef char *va_list;
typedef unsigned char u8;


void	*memcpy(
		void		*dst,
		const void	*src,
		size_t		cnt
	       )
{
	__asm__ (	"testb	$0x01, %%cl\n"
			"jz	1f\n"
			"movsb\n"
			"1:"
			"testb	$0x02, %%cl\n"
			"jz	2f\n"
			"movsw\n"
			"2:"
			"shrl	$2, %%ecx\n"
			"cld\n"
			"rep\nmovsl" ::
			"D" (dst),
			"S" (src),
			"c" (cnt)
		);
	return dst;
}


void	*memset(
		void		*dst,
#ifdef	__GNUC__
		int		val,
#else
		u8		val,
#endif
		size_t		cnt
	       )
{
	__asm__ (	"movb	%%dl, %%al\n"
			"testb	$0x01, %%cl\n"
			"jz	1f\n"
			"stosb\n"
			"1:"
			"movb	%%al, %%ah\n"
			"movb	%%al, %%dh\n"
			"shll	$16, %%eax\n"
			"movw	%%dx, %%ax\n"
			"testb	$0x02, %%cl\n"
			"jz	2f\n"
			"stosw\n"
			"2:"
			"shrl	$2, %%ecx\n"
			"cld\n"
			"rep\nstosl\n" ::
			"D" (dst),
#ifdef	_TCC
			"d" (val),
#else
			"dl" (val),
#endif
			"c" (cnt)
		);
	return dst;
}


void	*memmove(
		void		*dst,
		const void	*src,
		size_t		cnt
	       )
{
	void	*ret;
	__asm__ (	"testb	$0x01, %%cl\n"
			"jz	1f\n"
			"movsb\n"
			"1:"
			"testb	$0x02, %%cl\n"
			"jz	2f\n"
			"movsw\n"
			"2:"
			"shrl	$2, %%ecx\n"
			"cld\n"
			"rep\nmovsl\n" :
			"=D" (ret) :
			"D" (dst),
			"S" (src),
			"c" (cnt)
		);
	return ret;
}


int	memcmp(
		const void	*dst,
		const void	*src,
		size_t		cnt
	       )
{
	src = 0;
	cnt = (int) src; // wait a moment...
	dst = src;
	cnt = (int) dst;
	dst = (void *) cnt;
	return 0;
}


