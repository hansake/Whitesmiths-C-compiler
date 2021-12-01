/*  PRINTER DTF DEFINITIONS UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */

typedef struct
	{
	UTINY	device;			/* always '0xe0' for printer */
	UCOUNT	qualifier;		/* completion code qualifier */
	UTINY	upsi;			/* external indicators (upsi) */
	UTINY	res0;			/* reserved (must be 0x80) */
	TEXT	*bchaina;		/* dtf backward chain address */
	UTINY	res1;			/* reserved (must be 0x80) */
	TEXT	*fchaina;		/* dtf forward chain address */
	UTINY	res2;			/* reserved (must be 0x80) */
	TEXT	*lreca;			/* logical record address */

	UTINY	completion;		/* completion code */

#define PC_NORMAL		0x40	/* normal completion */
#define PC_IOERR		0x41	/* i/o error */
#define PC_INVREQ		0x43	/* invalid request (transparent mode) */
#define PC_RESERVED		0x44	/* reserved */
#define PC_OVERFLOW		0x48	/* overflow */
#define PC_NOMESSFAIL	0x98	/* 'no message' request failed */
#define PC_NOTOPEN		0x99	/* dtf not opened */

	UTINY	operation;		/* operation code */

#define PO_PRINT		0x40	/* print operation */
#define PO_TRANSMODE	0x20	/* transparent mode request */
#define PO_SKIPSPACE	0x00	/* skip/space request */

	UTINY	attrib1;		/* file attribute byte 1 */
	UTINY	attrib2;		/* file attribute byte 1 */
	UTINY	attrib3;		/* file attribute byte 1 */
	UTINY	attrib4;		/* file attribute byte 1 */

	UCOUNT	reclen;			/* logical record length */
	TEXT	filename[8];	/* file name */

	UTINY	bskip;			/* skip before value */
	UTINY	bspace;			/* space before value */
	UTINY	askip;			/* skip after value */
	UTINY	aspace;			/* space after value */

	UTINY	res3;

	UTINY	aprintff;		/* after printing form feeds (trans. mode) */
	UTINY	res4[2];		/* reserved */

	UTINY	pageoflow;		/* page overflow value */
	UTINY	lines;			/* lines per page */
	UTINY	currline;		/* current line number */
	UTINY	psb[3];			/* psb address */
	UTINY	res5[7];
	} DTFP;
