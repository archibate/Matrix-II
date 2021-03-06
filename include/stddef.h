#ifndef	_STDDEF_H
#define	_STDDEF_H

#ifndef	NULL
#ifdef	_DEFINE_NULL_AS_ZERO
#define	NULL	0
#else
#define	NULL	((void *) 0)
#endif	/* _DEFINE_NULL_AS_ZERO */
#endif	/* NULL */
#ifndef	EOF
#define	EOF	(-1)
#endif	/* EOF */
#ifndef	_ENUM_BOOLEAN
#ifndef	TRUE
#define	TRUE	1
#endif	/* TRUE */
#ifndef	FALSE
#define	FALSE	0
#endif	/* FALSE */
#endif	/* _ENUM_BOOLEAN */
#if	__GNUC__ == 2 && __GNUC__MINOR__ < 96
#define	__builtin_expert(x, expected)	(x)
#error	GCC version lesser than 2.96, no __builtin_expect supported!
#endif	/* __GNUC__ == 2 && __GNUC__MINOR__ < 96 */
#ifndef	likely
#define	likely(x)	(__builtin_expect((x), 1))
#endif	/* likely */
#ifndef	unlikely
#define	unlikely(x)	(__builtin_expect((x), 0))
#endif 	/* unlikely */
#ifndef	assert
#define	assert(x, info) ({ \
		if (unlikely(!(x))) { \
			assert_panic(info); \
		} \
	})
#endif	/* assert */
#ifndef	assert_info
#define	assert_info(x, info) ({ \
		if (unlikely(!(x))) { \
			assert_panic(info); \
		} \
	})
#endif	/* assert_info */
#ifndef	assert_quick
#define	assert_quick(x) ({ \
		if (unlikely(!(x))) { \
			assert_panic(info); \
		} \
	})
#endif	/* assert_quick */
#ifndef	assert_static
#define	assert_static(x) ({ \
		switch (x) { case 0: default: ; }; \
	})
#endif	/* assert_static */

#endif	/* _STDDEF_H */

