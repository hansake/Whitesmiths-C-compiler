/*  SYSLIST PARAMETER LIST FOR IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 */

typedef struct	/* syslist parameter list */
	{
	UTINY	slfnc1;			/* function/return code byte 1 */
	UTINY	slapge;			/* page control */
	UTINY	slfnc2;			/* function/return code byte 1 */
	COUNT	slmic;			/* MIC number */
	UTINY	sllen;			/* length of text */
	UTINY	res1;			/* reserved (must be 0x80) */
	TEXT	*sladdr;		/* message address */
	} SYSLIST;
