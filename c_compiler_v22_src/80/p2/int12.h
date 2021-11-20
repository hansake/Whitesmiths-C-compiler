/*	INTERFACE BETWEEN C PASS1 & PASS2
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */

/*	the registers
 */
#define ARGIDX	0240
#define AUTIDX	0040
#define INTRET	0001
#define LONGRET 0003
#define FLTRET	0100
#define REGSET	0034

/*	the created operators
 */
#define DADDR	0144
#define DALIST	0245
#define DCAST	0146
#define DGMIN	0247
#define DELIST	0250
#define DFNCALL	0251
#define DFNONLY	0151
#define DGPLU	0252
#define DINDIR	0153
#define DMINUS	0154
#define DPLUS	0155
#define DCMP	0256
#define DGETS	0257
#define DLESS	0260

/*	pass two types
 */
#define XTEST	0
#define XCHAR	1
#define XUCHAR	2
#define XSHORT	3
#define XSFIELD	4
#define XUSHORT	5
#define XLONG	6
#define XLFIELD	7
#define XULONG	8
#define XFLOAT	9
#define XDOUBLE	10
#define XPTRTO	11

/*	interface commands from pass1 to pass2
 */
#define GAUTOFF	0201
#define GCASE	0202
#define GDATA	0203
#define GDEF	0204
#define GFLOAT	0205
#define GFUNC	0206
#define GGEQ	0207
#define GGREAT	0210
#define GINT	0211
#define GISEQ	0212
#define GJUMP	0213
#define GLABEL	0214
#define GLESS	0215
#define GLEQ	0216
#define GNOTEQ	0217
#define GREF	0220
#define GREGS	0221
#define GRET	0222
#define GSPACE	0223
#define GSTRING	0224
#define GSWITCH	0225
#define GVOID	0226
#define GSWEND	0227
#define GNDATA	0230	/**/
#define GLITS	0270
#define GSWTAB	0271
#define GADDR	0300
