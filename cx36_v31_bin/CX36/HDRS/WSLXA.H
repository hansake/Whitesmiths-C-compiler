/*	THE STANDARD WSL DEFINITIONS HEADER
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef __WSLXA__
#define __WSLXA__	1

/*	set up default compiler and library version if none given
 */
#ifndef _CVERSION
#define _CVERSION	300
#endif
#ifndef _LVERSION
#define _LVERSION	300
#endif

/*	set up prototyping
 */
#ifndef __
#ifdef _PROTO
#define __(a)	a
#else
#define __(a)	()
#endif
#endif

/*	the pseudo storage classes
 */
#define FAST	register
#define GLOBAL	extern
#define IMPORT	extern
#define INTERN	static
#define LOCAL	static

/*	the pseudo types
 */
typedef char TBOOL, TEXT;
typedef double DOUBLE;
typedef float FLOAT;
typedef int ARGINT, BOOL, INT, METACH;
typedef long LONG;
typedef short COUNT, FD;
typedef unsigned char TBITS, UTINY;
typedef unsigned int BYTES;
typedef unsigned long LBITS, ULONG;
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

/*	system parameters
 */
#define STDIN	0
#define STDOUT	1
#define STDERR	2
#define YES		1
#define NO		0
#define FAIL	1
#define SUCCESS	0
#define NULL	(VOID *)0
#define FOREVER	for (;;)
#define BYTMASK	0377
#define CPERM	0666
#define R_RAW		1
#define R_COOKED	0
#define R_QUERY		-1

/*	being phased out when WSL library is phased out
 */
#define READ	0
#define WRITE	1
#define UPDATE	2
#define BWRITE	-1	

/*	declarations for libc functions returning pointers, longs or doubles
 */
COUNT (*mkord __((TEXT **keyarray, TEXT *lnordrule)))();
DOUBLE dtento __((DOUBLE d, COUNT exp));
DOUBLE sqr __((DOUBLE x));
DOUBLE sqrt __((DOUBLE x));
LONG lseek __((FD fd, LONG offset, COUNT sense));
LONG lstol __((TEXT *s));
TEXT *atime __((struct _tvec *pv, TEXT *s));
TEXT *cpystr __((TEXT *dest, ...));
TEXT *decrypt __((TEXT data[8], TINY ks[16][8]));
TEXT *encrypt __((TEXT data[8], TINY ks[16][8]));
TEXT *getflags __((BYTES *pac, TEXT ***pav, TEXT *fmt, ...));
TEXT *itols __((TEXT *s, COUNT n));
TEXT *ltols __((TEXT *pl, LONG lo));
TEXT *pathnm __((TEXT *buf, TEXT *n1, TEXT *n2));
TEXT *pattern __((TEXT *pat, TEXT delim, TEXT *p));
TEXT *uniqnm __((VOID));
TINY *bldks __((TINY ks[16][8], TEXT key[8]));
ULONG xstol __((TEXT *s, BOOL lsfmt));
VOID *alloc __((BYTES need, VOID *link));
VOID *buybuf __((VOID *s, BYTES n));
VOID *frelst __((VOID *p, VOID *plast));
VOID *lfree __((VOID *addr, VOID *link));
VOID *nalloc __((BYTES need, VOID *link));
VOID *sbreak __((BYTES size));
struct _file *fdopen __((FD fd, TEXT *type));
struct _tvec *ltime __((struct _tvec *pv, ULONG lt));
struct _tvec *vtime __((struct _tvec *pv, ULONG lt));

/*	optionally include all other function prototype declarations
 */
#ifdef _PROTO
#include <pcdecl.h>
#endif

/*	macros
 */
#define max(x, y)	(((x) < (y)) ? (y) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))

#endif
