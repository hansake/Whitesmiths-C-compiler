/*  WORKSTATION DTF DEFINITIONS FOR IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Unisoft AB of Sweden
 */

typedef struct
	{
	UTINY	device;			/* allways '0xC0' for workstation */
	VOID	*data_mgr;		/* data management address */
	UTINY	upsi;			/* external indicators (upsi) */
	UTINY	res0;			/* reserved (must be 0x80) */
	VOID	*bchaina;		/* dtf backward chain address */
	UTINY	res1;			/* reserved (must be 0x80) */
	VOID	*fchaina;		/* dtf forward chain address */
	/**/
	UTINY	res2;			/* reserved (must be 0x80) */
	VOID	*lreca;			/* logical record address */
	UTINY	completion;		/* completion code */
	UTINY	operation;		/* operation code */
	UTINY	attrib1;		/* file attribute byte 1 */
	UTINY	attrib2;		/* file attribute byte 1 */
	UTINY	attrib3;		/* file attribute byte 1 */
	UTINY	attrib4;		/* file attribute byte 1 */
	UCOUNT	reclen;			/* record length */
	/**/
	TEXT	filename[8];	/* workstation file name */
	UTINY   wsopm;			/* operation code modifier */
#define	WOM_ERROR	0x06	/* put to error line */
#define	WOM_OVR		0x40	/* put for displaying override fields and attr. */
#define WOM_ROLL	0x20	/* roll request */
#define WOM_UNF		0x10	/* unformatted request */
#define WOM_PRINT	0x08	/* print request */
#define WOM_PRUF	0x01	/* put for read under format */
#define WOM_ZERO	0x00	/* null operation modifier */

	UTINY	wsopc;			/* work station operation code */
#define WOP_ZERO	0x00	/* no operation code */
#define WOP_GET		0x01	/* recieve data */
#define WOP_PUT		0x02	/* send data */
#define WOP_PTG		0x03	/* put no wait, then get */
#define WOP_INV		0x05	/* invite */
#define WOP_PNW		0x06	/* put no wait */
#define WOP_PTI		0x07	/* put no wait, then invite */
#define WOP_ACI		0x08	/* request data from any station with invite */
#define WOP_ACQ		0x09	/* allocate display station */
#define WOP_REL		0x0a	/* release display station */
#define WOP_GTA		0x0c	/* get terminal attributes */
#define WOP_GST		0x9c	/* get APPC session status */
#define WOP_EGTA	0x0d	/* get extended attributes */
#define WOP_GST		0x9c	/* used by APPC */
#define WOP_STI		0x10	/* cancel invite */
#define WOP_RES		0x20	/* reset keyboard */
#define WOP_RTG		0x21	/* reset keyboard, then get */
#define WOP_RTI		0x25	/* reset keyboard, then invite */
#define WOP_ERS		0x40	/* erase modified input fields */
#define WOP_ETG		0x41	/* erase modified input fields, then get */
#define WOP_ETI		0x45	/* erase modified input fields, then invite */
#define WOP_STM		0x0b	/* set interval time to wait before timeout */

	UCOUNT	outl;			/* output length */
	UTINY	res3;			/* reserved (must be 0x80) */
	TEXT	*reca;			/* record area address */
	UTINY	res4;			/* reserved (must be 0x00) */
	TEXT	*tub;			/* terminal unit block (TUB) address */
	UTINY   res5;			/* reserved (must be 0x80) */
	TEXT	*pla;			/* parameter list address */
	/**/
	UTINY	aid;			/* AID byte on input operations */
	UTINY	iopc;			/* internal operation code */
	UTINY	rupd;			/* roll flags */
	UTINY	nlne;			/* number of lines to roll */
	UTINY	slne;			/* start line number for roll */
	UTINY	elne;			/* end line number for roll */
	UTINY	wsln;			/* variable start line number */
	UTINY   res6;			/* reserved (must be 0x80) */
	TEXT	*indadr;		/* address of override indicators */
	UTINY	rtco;			/* return code extend byte */
	UTINY	rtc;			/* external return code */
#define WRC_ROK		0x00	/* operation successful */
#define WRC_RACC	0x01	/* new requester */
#define WRC_RSTP	0x02	/* stop system requested by system operator */
#define WRC_RCTL	0x03	/* no data returned, control info. only */
#define WRC_RQO		0x08	/* ACQ successful, display already allocated */
#define WRC_RAFW	0x18	/* ACQ failed, station allocated to non-NEP */
#define WRC_RAFN	0x38	/* ACQ failed */
#define WRC_RACR	0x11	/* ACI rejected, no invites outstanding */
#define WRC_RNAV	0x24	/* display station released */
#define WRC_RREL	0x28	/* op. ignored display released by program */
#define WRC_RIRJ	0x34	/* input rejected, input buffer too small */
#define WRC_ROFL	0x40	/* requested display off line */
#define WRC_RSPF	0x44	/* STI failed, operator entered data */
#define WRC_POGE	0x45	/* invalid ideograpic data in print operation */
#define WRC_RPAL	0x48	/* print operation issued to printer */
#define WRC_RGRF	0x50	/* on output, ideographic character table full */
#define WRC_RGI		0x51	/* on output, invalid ideographic character */
#define WRC_RGU		0x52	/* on output, ideographic character error */
#define WRC_POST	0x60	/* posted with user defined address */
#define WRC_PPRE	0x80	/* permanent i/o error, option 2 selected */

	UTINY	cprow;			/* cursor position, row */
	UTINY	cpcol;			/* cursor position, column */
	UTINY	fnc1;			/* function byte 1 */
	/**/
	UTINY	ckmk[3];		/* command key mask */
	UTINY	fnc2;			/* function byte 2 */
	UTINY	res7[7];		/* reserved */
	UTINY	kmsk;			/* key mask */
	/**/
	TEXT	fmtnm[8];		/* format name */
	COUNT	inl;			/* maximum input data length */
	TEXT	name[2];		/* symbolic terminal id */
	UTINY	res8;			/* reserved must be 0x80 */
	TEXT	*exta;			/* address of RPG DTF extension */
	UTINY	res9;			/* reserved must be 0x80 */
	TEXT	*tabl;			/* address of terminal id table */
	UTINY	tenl;			/* length of table entry */
	UTINY	tnum;			/* max number of terminals in file */
	UTINY	fnca;			/* function byte A */
	UTINY	res10[6];		/* reserved area */
	} DTFW;

#define DTFW_INIT \
	{0xc0, NULL, 0, 0x80, NULL, 0x80, (VOID *)-1, \
	0x80, NULL, 0x40, 0, 0, 0, 0, 0, 0, \
	{0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}, \
	0, 0, 0, 0x80, NULL, 0, NULL, 0x80, NULL, \
	0, 0, 0, 1, 1, 24, 1, 0x80, NULL, 0, 0, 0, 0, 0, \
	{0xff, 0xff, 0xff}, 0, {0, 0, 0, 0, 0, 0, 0}, 0, \
	{0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}, \
	0, {0x40, 0x40}, 0x80, NULL, 0x80, NULL, 0, 1, 0, }

