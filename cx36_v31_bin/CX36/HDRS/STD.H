/*	THE STANDARD HEADER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */

#ifndef	__STD__
#define	__STD__	1

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
typedef int ARGINT, BOOL;
typedef long LONG;
typedef short COUNT, FD, FILE, METACH;
typedef unsigned char UTINY;
typedef unsigned int BYTES;
typedef unsigned long ULONG;
typedef unsigned short BITS, UCOUNT;

#if _CVERSION < 300
typedef char TINY;
typedef char VOID;
#else
typedef signed char TINY;
typedef void VOID;
typedef VOID (*(*FNPTR)())();	/* pseudo type for onexit */
#endif

/*	system parameters
 */
#define STDIN	0
#define STDOUT	1
#define STDERR	2
#define YES		1
#define NO		0
#define NULL	(VOID *)0
#define FOREVER	for (;;)
#define BUFSIZE	512
#define BWRITE	-1
#define READ	0
#define WRITE	1
#define UPDATE	2
#define EOF		-1
#define BYTMASK	0377

/*	the file IO structure
 */
typedef struct _file
	{
	struct _file *flist;	/* chain used to flush buffers */
	short fd;				/* file descriptor */
	unsigned short flag;	/* flag bits */
	int nleft;				/* reading, # of chars undelivered in buffer */
							/* writing, # of chars put in buffer to output */
	int bufsize;			/* size of buffer (default BUFSIZ) */
	unsigned char *pnext;	/* ptr to next char to deliver */
	unsigned char *buf;		/* ptr to allocated buffer */
	} FIO;

/*	map old names to new names
 */
#define fioerr	_fioerr
#define fread	furead
#define fwrite	fuwrite
#define free	lfree
#define readerr	_reaerr
#define stdin	_stdin
#define stdout	_stdout
#define uname	uniqnm
#define writerr	_wrierr

/*	declarations for libw functions returning pointers or doubles
 */
DOUBLE abs __((DOUBLE d));
DOUBLE arctan __((DOUBLE x));
DOUBLE cos __((DOUBLE x));
DOUBLE exp __((DOUBLE x));
DOUBLE ln __((DOUBLE x));
DOUBLE sin __((DOUBLE x));
DOUBLE sqr __((DOUBLE x));
DOUBLE sqrt __((DOUBLE x));
FIO *fclose __((FIO *pf));
FIO *fcreate __((FIO *pf, TEXT *fname, COUNT mode));
FIO *finit __((FIO *pf, FILE fd, COUNT mode));
FIO *fopen __((FIO *pf, TEXT *fname, COUNT mode));
FNPTR onexit __((FNPTR (*pfn)()));

/*	declarations for libc functions returning pointers
 */
COUNT (*mkord __((TEXT **keyarray, TEXT *lnordrule)))();
DOUBLE dtento __((DOUBLE d, COUNT exp));
LONG lseek __((FD fd, LONG offset, COUNT sense));
LONG lstol __((TEXT *s));
TEXT *cpystr __((TEXT *dest, ...));
TEXT *decrypt __((TEXT data[8], TINY ks[16][8]));
TEXT *encrypt __((TEXT data[8], TINY ks[16][8]));
TEXT *getflags __((BYTES *pac, TEXT ***pav, TEXT *fmt, ...));
TEXT *itols __((TEXT *s, COUNT n));
TEXT *ltols __((TEXT *pl, LONG lo));
TEXT *pathnm __((TEXT *buf, TEXT *n1, TEXT *n2));
TEXT *pattern __((TEXT *pat, TEXT delim, TEXT *p));
TEXT *uniqnm __((VOID));
TEXT *atime __((struct _tvec *pv, TEXT *s));
TINY *bldks __((TINY ks[16][8], TEXT key[8]));
ULONG xstol __((TEXT *s, BOOL lsfmt));
VOID *alloc __((BYTES need, VOID *link));
VOID *buybuf __((VOID *s, BYTES n));
VOID *free __((VOID *addr, VOID *link));
VOID *frelst __((VOID *p, VOID *plast));
VOID *nalloc __((BYTES need, VOID *link));
VOID *sbreak __((BYTES size));
struct _tvec *ltime __((struct _tvec *pv, ULONG lt));
struct _tvec *vtime __((struct _tvec *pv, ULONG lt));

/*	optionally include all other function prototype declarations
 */
#ifdef _PROTO
#include <pwdecl.h>
#include <pcdecl.h>
#endif

/*	setup for gtc and ptc macros
 */
#ifndef READING
#define READING		020		/* should track values in stioli.h */
#endif
#ifndef WRITING
#define WRITING		040
#endif

/*	macros
 */
#define abs(x)		((x) < 0 ? -(x) : (x))
#define gtc(pf)		(((pf)->flag & READING && 0 < (pf)->nleft) \
						? (--(pf)->nleft, *(pf)->pnext++) : getc(pf))
#define isalpha(c)	(islower(c) || isupper(c))
#define isdigit(c)	('0' <= (c) && (c) <= '9')
#define islower(c)	('a' <= (c) && (c) <= 'z')
#define isupper(c)	('A' <= (c) && (c) <= 'Z')
#define iswhite(c)	((c) <= ' ' || 0177 <= (c))
#define max(x, y)	(((x) < (y)) ? (y) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))
#define ptc(pf, c)	(((pf)->flag & WRITING && (pf)->nleft < (pf)->bufsize) ? \
						(pf)->buf[(pf)->nleft++] = (c) : putc(pf, c))
#define tolower(c)	(isupper(c) ? ((c) + ('a' - 'A')) : (c))
#define toupper(c)	(islower(c) ? ((c) - ('a' - 'A')) : (c))

#endif
