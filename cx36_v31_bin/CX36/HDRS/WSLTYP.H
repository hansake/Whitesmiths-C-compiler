/*	STANDARD DEFINITIONS FOR ANSI USERS USING WHITESMITHS TYPES
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef __WSLTYP__
#define __WSLTYP__	1

/*	the pseudo storage classes
 */
#define FAST	register
#define GLOBAL	extern
#define IMPORT	extern
#define INTERN	static
#define LOCAL	static

/*	the pseudo types
 */
typedef char TBOOL, TEXT
typedef double DOUBLE;
typedef float FLOAT;
typedef int ARGINT, BOOL, INT, METACH;
typedef long LONG;
typedef short COUNT, FD;
typedef unsigned char TBITS, UTINY;
typedef unsigned int BYTES;
typedef unsigned long LBITS, TIME, ULONG;
typedef unsigned short BITS, UCOUNT;

#if _CVERSION < 300
typedef char TINY;
#define void char
#define VOID char
#else
typedef signed char TINY;
typedef void VOID;
#define RDONLY const
#define TOUCHY volatile
typedef VOID (*(*FNPTR)())();	/* pseudo type for onexit */
#endif

#define TIMEVEC struct _tm

#endif
