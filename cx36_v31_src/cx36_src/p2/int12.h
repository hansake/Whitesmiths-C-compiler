/*	INTERFACE BETWEEN C PASS1 & PASS2
 *	copyright (c) 1978, 1985 by Whitesmiths, Ltd.
 */

/*	the registers
 */
#define	ARGIDX	0240
#define	AUTIDX	0040
#define	INTRET	0001
#define	LONGRET	0003
#define	FLTRET	0100
#define	REGSET	0034
#define	RETIDX	1

/*	the created operators
 */
#define	DADDR	0144
#define	DALIST	0245
#define	DCAST	0146
#define	DGMIN	0247
#define	DELIST	0250
#define	DFNCALL	0251
#define	DFNONLY	0151
#define	DGPLU	0252
#define	DINDIR	0153
#define	DMINUS	0154
#define	DPLUS	0155
#define	DCMP	0256
#define	DGETS	0257
#define	DLESS	0260

/*	pass two types
 */
#define	XTEST	000
#define	XCHAR	001
#define	XUCHAR	002
#define	XSHORT	003
#define	XSFIELD	004
#define	XUSHORT	005
#define	XLONG	006
#define	XLFIELD	007
#define	XULONG	010
#define	XFLOAT	011
#define	XDOUBLE	012
#define	XPTRTO	013			/* SHDB 014 */
#define	XLPTRTO	015
#define	XSTRUCT	016

/*	interface commands from pass1 to pass2
 */
#define	GFFUNC		0200
#define	GAUTOFF		0201
#define	GCASE		0202
#define	GDATA		0203
#define	GDEF		0204
#define	GFLOAT		0205
#define	GFUNC		0206
#define	GGEQ		0207
#define	GGREAT		0210
#define	GINT		0211
#define	GISEQ		0212
#define	GJUMP		0213
#define	GLABEL		0214
#define	GLESS		0215
#define	GLEQ		0216
#define	GNOTEQ		0217
#define	GREF		0220
#define	GREGS		0221
#define	GRET		0222
#define	GSPACE		0223
#define	GSTRING		0224
#define	GSWITCH		0225
#define	GVOID		0226
#define	GSWEND		0227
							/* 230 - 267 are used internal to p2 */
#define	GLITS		0270
#define	GSWTAB		0271
#define	GSPNAME		0272
#define	GDBFUNC		0273
#define	GDBSTAT		0274
#define	GLSWITCH	0275
#define	GADDR		0300	/* uses next four values too */
#define	GADDR1		0301
#define	GADDR2		0302
#define	GADDR3		0303
#define	GADDR4		0304
#define	GERR		0306
#define	GFDATA		0307
#define	GFADDR		0310	/* ditto */
#define	GFADDR1		0311
#define	GFADDR2		0312
#define	GFADDR3		0313
#define	GFADDR4		0314
#define	GJMPTAB		0315
#define	GSCNTAB		0316
#define	GSDBDATA	0320
#define	GEDBDATA	0321
#define	GDEND		0322
#define	GFEND		0323

/*	name byte parameter
 */
#define	NMVAL	0200
#define	NMFAR	010
#define	NMDAT	001
#define	NMLIT	002
#define	NMBSS	003
#define	NMFUN	004
#define	NMFRD	005
#define	NMFRS	006

/*	space field parameters
 */
#define	TFFUNC		0x8000
#define	TFVOL		0x4000
#define	TFCONST		0x2000
#define	TFRVAL		0x1000
#define	TFFAR		0x0800
#define	TFSPACE		(TFFAR-1)

/*	created label type codes
 */
#define CRFUN	0
#define CRDAT	2
#define CRLIT	4
#define CRBSS	6
