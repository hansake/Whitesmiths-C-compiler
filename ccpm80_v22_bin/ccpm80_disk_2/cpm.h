/*	CP/M INTERFACE HEADER FILE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */

/*	the CP/M file control block, as augmented
 */
#define FCB	struct fcb
struct fcb {
	TEXT et;			/* device number */
	TEXT fn[8];			/* blank padded */
	TEXT ft[3];			/* ditto */
	UCOUNT curblk, recsiz;
	LONG fsize;
	UCOUNT date;
	TINY dmap[10];		/* reserved for system use */
	TINY nr;			/* next record to read/write */
	UTINY rno[4];		/* random record number */
	TINY bleft;
	TEXT *bnext;
	LONG lact, lend, lseek;
	TEXT buf[128];
	};

/*	the Whitesmiths control block
 */
#define WCB	struct wcb

struct wcb {
	TINY flags, dev;
	FCB *pf;
	};

/*	flag values:
 */
#define WOPEN	001
#define WDIRT	002
#define WSTD	004
#define WCR		010
#define WUSED	020

/*	device values:
 */
#define LST		-4
#define PUN		-3
#define RDR		-2
#define CON		-1

/*	intermodule communication
 */
#define CTRLZ	032
#define EOF		1
#define FAIL	-1
#define MCREATE	0
#define MOPEN	1
#define MREMOVE	2
#define MWRITE	4
#define SYSBUF	0x80

/*	CP/M innards
 */
#define CRESET	0
#define CRDCON	1
#define CWRCON	2
#define CRDRDR	3
#define CWRPUN	4
#define CWRLST	5
#define CDCIO	6
#define CGIOST	7
#define CSIOST	8
#define CPRBUF	9
#define CRDBUF	10
#define CICRDY	11
#define CLFTHD	12
#define CINIT	13
#define CLOGIN	14
#define COPEN	15
#define CCLOSE	16
#define CSRCH	17
#define CSRCHN	18
#define CDEL	19
#define CREAD	20
#define CWRITE	21
#define CMAKE	22
#define CRENAME	23
#define CILOGIN	24
#define CIDRNO	25
#define CSETAD	26
#define CIALLOC	27
#define CWPROT	28
#define CGETVEC	29
#define CSETFA	30
#define CGETPAR	31
#define CGSUSER	32
#define CRREAD	33
#define CRWRITE	34
#define CFSIZE	35
#define CSETRR	36
#define CDRESET	37
#define CRZWRIT	40
