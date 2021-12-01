/*	STRUCTURES FOR PASS 2 FOR P1PR
 *	copyright (c) 1978, 1985 by Whitesmiths, Ltd.
 */

/*	the quasi types
 */
typedef unsigned short LABEL;
typedef short LEX;

/*	the registers
 */
#define R0		0001
#define R1		0002
#define R2		0004
#define R3		0010
#define R4		0020

/*	parameters
 */
#define LENNAME	63

#define ISDATA	1
#define ISLITS	2
#define ISTEXT	4
#define	ISBSS	8

/*	size in bytes
 */
#define BRSIZE	2
#define JCSIZE	5
#define JMPSIZE	3
#define RETSIZE	3
#define SWTSIZE	7

#define ISCASE	1
#define VISITED	2
#define NAILED	4
#define TOEMIT	8
#define SWTAB	16
#define NEWLAB	32

/*	created inst codes
 */
#define BLESS	0230
#define BLEQ	0231
#define BGREAT	0232
#define BGEQ	0233
#define BISEQ	0234
#define BNOTEQ	0235
#define BJUMP	0236
#define GCODE	0240
#define GNOP	0241
#define GHDR	0242
#define	GGETSTR	0244

#define GLOW	0250
#define GLOS	0251
#define GHIGH	0252
#define GHIS	0253
#define BLOW	0254
#define BLOS	0255
#define BHIGH	0256
#define BHIS	0257

/*	the index codes
 */
#define XAX		010
#define XCX		011
#define XDX		012		/* special [not a resource] */
#define XBX		013
#define XSP		014		/* special [not a resource] */
#define XBP		015
#define XSI		016
#define XDI		017
#define XSIBP	020
#define XDIBP	021
#define XSIBX	022
#define XDIBX	023
#define XF0		030
#define	XVBP	031
#define	XONSTK	040

/*	the space (segment) codes
 */
#define SCS		01
#define SDS		02
#define SES		04
#define SSS		010
#define SPORT	020
#define SBUI	040
#define SFAR	0100
#define SVOL	0200
#define SCONST	0400
#define SFUNC	01000
#define SVEST	010000	/* volatile es tmp */
#define	SIUD	020000	/* volatile dx tmp */
#define	SRVAL	0x8000

#define HASSEG	(SSS|SDS|SCS|SES|SVEST|SIUD)

/*	the want attributes
 */
#define WPORT	040000 /* GCCSET */
#define WFAR	020000
#define WBUI	004000	/* GVOL */
#define WNULL	001000
#define WCX		000400
#define WAX		000200
#define WREREF	000100
#define WSTACK	000040
#define WVREG	000020
#define WREG	000010
#define WIMM	000004
#define WMEM	000002
#define WTERM	000001

#define PTRSET	(WREG|WMEM|WPORT)
#define VOLSET	(WCX|WAX|WVREG)

/*	the got attributes
 */
#define GRVOL	0100000
#define GCCSET	040000
#define GJUNK	010000
#define GVOL	004000
#define GNOCODE	002000

/*	the type cast codes
 */
#define TBYTE	0
#define TWORD	1
#define TLONG	2
#define TQUAD	3
#define TFLOAT	4
#define TDOUBLE	5
#define TNEAR	6
