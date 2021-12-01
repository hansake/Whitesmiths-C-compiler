/*  SYSIN PARAMETER LIST FOR IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 */

typedef struct	/* sysin parameter list */
	{
	UTINY	siopt1;			/* option and reply byte 1 */
		/* input options */
#define SINM120B	0x20		/* on: 120 byte sysin buffer */
								/* off: 512 byte sysin buffer */
#define SINMNSUB	0x10		/* do not scan record for substitution */
#define SINMLOG		0x04		/* perform logging to history file */
#define SINMASTR	0x02		/* return records with an asterisk */
#define SININTR		0x01		/* sysin called by initiator */
		/* output options */
#define SINMNORM	0x40		/* normal completion */
#define SINMEOF		0x50		/* EOF found in current buffer */
	UTINY	siopt2;			/* option byte 2 */
#define SINMRSSA	0x01		/* don't process proc. control expr. */
	UTINY	sistat;			/* status byte */
#define SINMKB		0x80		/* record came from keyboard */
#define SINMKANJ	0x40		/* record contains BALBOA data */
	UTINY	res1;			/* reserved (must be 0x80) */
	TEXT	*siaddr;		/* message address */
	} SYSIN;
