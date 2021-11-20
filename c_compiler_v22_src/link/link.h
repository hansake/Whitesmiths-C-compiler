/*	HEADER FOR IDRIS LINKER
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */

/* the constructed output header
 */
typedef struct {
	ULONG tsiz, dsiz, bsiz, ssiz;
	ULONG tbias, dbias;
	ULONG atbias, adbias, atsiz, adsiz;
	UCOUNT sbytes;
	} BHDR;

/*	symbol table stuff
 */
#define DEFD	004
#define EXTERN	010
#define MAXSYMS	2048
#define NTABS	32
#define SMASK	077
#define SOFF	6
#define SSIZE	12

typedef struct {
	LONG value;
	TEXT flags, name[15];
	} SYMBOL;

/*	the list of undefined arguments
 */
#define NUND	10

typedef struct {
	BYTES nm;
	TEXT *sym[NUND];
	} ULIST;

/*	the list of libraries
 */
#define EXLIB	10
#define MAXLIBS	128

typedef struct {
	BYTES num;
	TEXT *nam[EXLIB];
	} LIBLIST;

/*	the buffer controllers
 */
#define WRMI	8
#define DRAIN	4
#define DREL	3
#define TREL	2
#define	DDAT	1
#define	TDAT	0

/*	input buffer controllers
 */
#define HIBUF	1
#define LOBUF	0

/*	buffer size parameters
 *	NOTE, must be power of 2
 *	all three must change in phase!
 */
#define L_BUFSIZE	64
#define L_BUF_MASK	077
#define	NOT_L_BUF_MASK	~077L

typedef struct {
	LONG off;
	COUNT start, n;
	UTINY *buf;
	} IOBUF;
