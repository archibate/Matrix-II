#ifndef	_TYPES_H
#define	_TYPES_H

#ifndef	_BASIC_TYPEDEFS
#define	_BASIC_TYPEDEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef u32 r_t;
typedef r_t ndx_t;
typedef void *vp_t;
typedef u8 *va_list_t;
typedef u8 byte_t;
typedef u16 word_t;
typedef u32 long_t;
typedef u64 qual_t;
typedef unsigned int size_t;
typedef __builtin_va_list va_list;
#undef	TRUE
#undef	FALSE
#ifndef	_ENUM_BOOLEAN
#define	_ENUM_BOOLEAN
enum boolean {
	TRUE = 1,
	FALSE = 0
};
#endif
#endif



#endif

