/*
 * kernel Standrad C library : vsprintf
 */

typedef unsigned int size_t;
typedef char *va_list;
typedef unsigned char u8;


/*static inline char	*itoa_base_sub(	// 一会儿再来改进
		int	val,
		int	base,
		char	**ps)
{
	register int	q, m;
	if (q = val / base)
		itoa_base_sub(q, base, ps);
	*(*ps)++ =
		((m = (val % base)) < 10) ?
		(m + '0') : (m - 10 + 'A');

	return *ps;
}*/


static inline void	itoa10_sub(
		int	val,
		char	**ps)
{
	register int	q;
	if (q = val / 10)
		itoa10_sub(q, ps);
	*(*ps)++ = (val % 10) + '0';
}


static inline void	itoa16_sub(
		int	val,
		char	**ps)
{
	register int	q, m;
	if (q = val / 16)
		itoa16_sub(q, ps);
	*(*ps)++ =
		((m = val % 16) < 10) ?
		(m + '0') : (m - 10 + 'A');
}


static inline void	itoa16_low_sub(
		int	val,
		char	**ps)
{
	register int	q, m;
	if (q = val / 16)
		itoa16_sub(q, ps);
	*(*ps)++ =
		((m = val % 16) < 10) ?
		(m + '0') : (m - 10 + 'a');
}


size_t	vsprintf(
		char		*buf,
		const char	*fmt,
		va_list		args)
{
	/* 
	 * 实际上，va_list 是一个指向栈上元素的指针 
	 */
	va_list		next_argp = args;
	
	int		m;

	char		*p;
	char		inner_buf[256];
	char		blank_fill;
	register char	ch;
	int		align_nr;

	for (p = buf; ch = *fmt; fmt++) {
		
		if (ch != '%') {
			*p++ = ch;
			continue;
		}
		else {		/* a format string begins */
			align_nr = 0;
		}

		switch (ch = *++fmt) {
		case '0':
			blank_fill = '0';
			fmt++;
			break;
		case '%':
			*p++ = ch;
			continue;
		default:
			blank_fill = ' ';
		}
		while (((u8) (ch = *fmt)) >= '0' 
				&& ((u8) ch <= '9')) {
			align_nr *= 10;
			align_nr += ch - '0';
			fmt++;
		}

		char	*q = inner_buf;
		memset(q, 0, sizeof(inner_buf));
		char	*as;

		switch (ch = *fmt) {
		case 'c':
			*q++ = * (char *) next_argp;
			break;
		case 'd':
			m = * (int *) next_argp;
			if (m < 0) {
				m = -m;
				*q++ = '-';
			}
			itoa10_sub(m, &q);
			break;
		case 's':
			as = * (char **) next_argp;
			/*strcpy(q, (*((char**)next_argp)));
			q += strlen(*((char**)next_argp));*/
			while (ch = *as++)
				*q++ = ch;
			break;
		case 'u':
			m = * (int *) next_argp;
			itoa10_sub(m, &q);
			break;
		case 'X':
			m = * (int *) next_argp;
			itoa16_sub(m, &q);
			break;
		case 'x':
			m = * (int *) next_argp;
			itoa16_low_sub(m, &q);
			break;
		case 'p':
			m = * (int *) next_argp;
			itoa16_sub(m, &q);
			if (!align_nr)
				align_nr = 8;
			blank_fill = '0';
			break;
		}
		next_argp += 4;

		size_t	k;

		size_t	kmax = (size_t) align_nr > (size_t) (q - inner_buf) ?
				(size_t) align_nr - (size_t) (q - inner_buf)
				: 0;

		for (k = 0; k < kmax; k++)
			*p++ = blank_fill;
		q = inner_buf;
		while (*q)
			*p++ = *q++;
	}

	*p = 0;

	return (size_t) (p - buf);
}


size_t	sprintf(
		char		*buf,
		const char	*fmt,
		...)
{
	va_list args = (va_list) ((char *)(&fmt) + 4);
	return vsprintf(buf, fmt, args);
}

