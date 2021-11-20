/*	THE STANDARD HEADER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */

/* the pseudo storage classes
 */
#define FAST	register
#define GLOBAL	extern
#define IMPORT	extern
#define INTERN	static
#define LOCAL	static

/* the pseudo types
 */
#ifdef UTEXT
typedef unsigned char TEXT;
#else
typedef char TEXT;
#endif
typedef TEXT TBOOL;
typedef char TINY;
typedef double DOUBLE;
typedef int ARGINT, BOOL, VOID;
typedef long LONG;
typedef short COUNT, FILE, METACH;
typedef unsigned BYTES;
typedef unsigned char UTINY;
typedef unsigned long ULONG;
typedef unsigned short BITS, UCOUNT;

/* system parameters
 */
#define STDIN	0
#define STDOUT	1
#define STDERR	2
#define YES		1
#define NO		0
#define NULL	0
#define FOREVER	for (;;)
#define BUFSIZE	512
#define BWRITE	-1
#define READ	0
#define WRITE	1
#define UPDATE	2
#define EOF		-1
#define BYTMASK	0377

/*	macros
 */
#define abs(x)		((x) < 0 ? -(x) : (x))
#define gtc(pf)	(0 < (pf)->_nleft ? (--(pf)->_nleft, \
		*(pf)->_pnext++ & BYTMASK) : getc(pf))
#define isalpha(c)	(islower(c) || isupper(c))
#define isdigit(c)	('0' <= (c) && (c) <= '9')
#define islower(c)	('a' <= (c) && (c) <= 'z')
#define isupper(c)	('A' <= (c) && (c) <= 'Z')
#define iswhite(c)	((c) <= ' ' || 0177 <= (c))
#define max(x, y)	(((x) < (y)) ? (y) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))
#define ptc(pf, c)	(((pf)->_nleft < 512) ? (pf)->_buf[(pf)->_nleft++] = (c) :\
	putc(pf, c))
#define tolower(c)	(isupper(c) ? ((c) + ('a' - 'A')) : (c))
#define toupper(c)	(islower(c) ? ((c) - ('a' - 'A')) : (c))

/* the file IO structure
 */
typedef struct fio
	{
	FILE _fd;
	COUNT _nleft;
	COUNT _fmode;
	TEXT *_pnext;
	TEXT _buf[BUFSIZE];
	} FIO;
