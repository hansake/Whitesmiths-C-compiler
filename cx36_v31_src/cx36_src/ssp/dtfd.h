/*  DISK DTF DEFINITIONS UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */

typedef struct
	{
	UTINY	device;			/* allways '0xa0' for fixed disk */

	TINY	fill_a[10];

	TEXT	*oreca;			/* output record address */

	UTINY	completion;		/* completion code */

#define DC_NORMAL		0x40
#define DC_PERMIOERR	0x41
#define DC_EOF			0x42
#define DC_INVOP		0x43
#define DC_RECNOTF		0x44
#define DC_NOPENDREC	0x45
#define DC_INVRRN		0x48
#define DC_INVDATAREC	0x49

	UTINY	operation;		/* operation code */

#define DO_GET_FIRST	0x81
#define DO_GET_LAST		0x82
#define DO_GET_NEXT		0x83
#define DO_GET_PREV		0x84
#define DO_GET_CURR		0x85
#define DO_GET_RRN		0x89
#define DO_ADD_EOD		0x40
#define DO_ADD_RRN		0x41
#define DO_UPDATE		0x48
#define DO_RNDUPD		0x49
#define DO_SET_EOF		0x04
#define DO_SET_BOF		0x05
#define DO_FIX_EOD		0x0c

	UTINY	attrib1;		/* file attribute byte 1 */

#define DA1_GRAN		0x80
#define DA1_GSEQ		0x40
#define DA1_AEOD		0x20
#define DA1_ARRN		0x10
#define DA1_UPD			0x08
#define DA1_DEL			0x04

	UTINY	attrib2;		/* file attribute byte 1 */

#define DA2_PSEUDO		0x08
#define DA2_ZPAM		0x04
#define DA2_KEYED		0x02
#define DA2_OPENED		0x01

	UTINY	attrib3;		/* file attribute byte 1 */

#define DA3_CREIND		0x80
#define DA3_CREDIR		0x40
#define DA3_CRESEQ		0x20
#define DA3_ORDADD		0x10
#define DA3_BINRRN		0x08
#define DA3_ALOCTD		0x04
#define DA3_SPCOPN		0x02
#define DA3_OPNREQ		0x01

	UTINY	attrib4;		/* file attribute byte 1 */
	UCOUNT	reclen;			/* record length */
	TEXT	filename[8];	/* file name */
	UTINY	fill_b[1];
	TEXT	*ireca;			/* input record address */
	UTINY	fill_c[5];
	UTINY	modifb1;		/* access modifier byte 1 */

	UTINY	modifb2;		/* access modifier byte 2 */

#define DM2_RETRRN 0x08

	UTINY	modifb3;		/* access modifier byte 3 */
	UTINY	fill_d[6];
	TEXT	rrn[8];			/* relative record number (decimal) */
	UTINY	fill_e[24];
	UTINY	fill_f[20];		/* needed in pseudo mode */
	} DTFD;

GLOBAL DTFD _dtfd;
