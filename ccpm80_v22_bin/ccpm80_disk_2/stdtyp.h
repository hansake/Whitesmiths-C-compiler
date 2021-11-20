/*	DEFINE STANDARD TYPES IF NOT ALREADY DEFINED IN std.h
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */

#ifndef FOREVER
#define USHORT	1	/* if compiler supports unsigned short */
#define TINY 	1	/* if char is signed */
#define UTINY	1	/* if compiler supports unsigned char */
/*	#define VOID	1	/* if compiler supports VOID type */

#ifdef USHORT
	typedef unsigned short ushort;
	#define USHORT(n) (unsigned short)(n)
#else
	typedef short ushort;
	#define USHORT(n) (unsigned)((unsigned)((n) & 0xFFFF))
#endif
#ifdef TINY
	typedef char tiny;
	#define TINY(n) (char)(n)
#else
	typedef char tiny;
	#define TINY(n) (char)(((n) & 0x80) ? (~0x7F | (n)) : (n))
#endif
#ifdef UTINY
	typedef unsigned char utiny;
	#define UTINY(n) (unsigned char)(n)
#else
	typedef char utiny;
	#define UTINY(n) (unsigned)((n) & 0xFF)
#endif
#ifndef VOID
	typedef int void;
#endif

typedef char tbits, tbool;
typedef int bool;
typedef long lbits;
typedef short bits, metachar;

#define ABS(a)	((a) > 0 ? (a) : -(a))
#define FAIL	1
#define FOREVER	for(;;)
#define LURSHIFT(n, b)	(((long)(n) >> (b)) & (0x7FFFFFFF >> (b-1)))
#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define NO		0
#define READ	0
#define STDERR 	2
#define STDIN 	0
#define STDOUT	1
#define SUCCEED	0
#define UPDATE	2
#define WRITE	1
#define YES		1
#endif
