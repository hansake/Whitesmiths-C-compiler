/*  SYSLOG PARAMETER LIST FOR IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 */

typedef struct	/* syslog parameter list type-2 */
	{
	UTINY	funb_A;			/* function byte A */
#define LGMMSGM		0x80		/* on: output from msg member */
								/* off: output from user program */
#define LGMWSRT		0x40		/* on: workstation routing */
								/* off: system or subconsole routing */
#define LGMDATR		0x20		/* data response requested */
#define LGMHALT		0x10		/* option response requested */
#define LGMFRMT		0x08		/* format line requested */
#define LGM8BYT		0x04		/* add 8 bytes to message */
#define LGMHIST		0x02		/* log to history file */
#define LGMCRT		0x01		/* display message on crt */
	UTINY	funb_B;			/* function byte B */
	UTINY	funb_C;			/* function byte C */
	UTINY	res1[9];		/* not used */
	UTINY	msglen;			/* message length */
	UTINY	res2;			/* reserved (must be 0x80) */
	TEXT	*msgadr;		/* message address */
	UTINY	res3[12];		/* reserved */
	} SYSLPT2;
