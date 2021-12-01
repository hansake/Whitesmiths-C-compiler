/*	STRUCTURES FOR PASS 2
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */

/*	the registers:
 *
 *		#XR1 -		Frame Pointer
 *		#XR2 -		Work Register
 *		#WR4
 *		#WR5
 *		#WR6
 *		#WR7
 *
 *		@FP -		Frame Pointer, copy of XR1
 *					(2 bytes in static memory)
 *		@XR2 -		Temporary copy of XR2
 *					(2 bytes in static memory)
 *		@AC0 -		Accumulator 0, holds returned value
 *					(8 bytes in static memory)
 *		(#F0 -		Floating point accumulator
 *					(8 bytes in control processor) )
 */

#define bit(n) (1 << n)

/*	the quasi types
 */
typedef char INDEX;
typedef unsigned short LABEL;
typedef short LEX;
typedef unsigned short OP;
typedef unsigned short RESOURCE;
typedef unsigned short SPACE;
typedef unsigned short TYPE;
typedef unsigned long WANTS;

/*	the registers
 */
#define AC0	(RESOURCE) bit(0)
#define XR2	(RESOURCE) bit(1)
#define F0	(RESOURCE) bit(2)
#define XR1	(RESOURCE) bit(3)

/*	aliases for the above registers
 */
#define IRET	AC0
#define FRET	AC0		/* maybe F0 */

/*	register sets
 */
#define BLFSET	(AC0|XR2|F0)	/* boolify set */
#define FNSET	(AC0|XR2|F0)	/* function set */

#define FPOFFSET 246
#define ARGBIAS	(FPOFFSET+4)
#define AUTBIAS	FPOFFSET
#define MAXBIAS 256		/* ?? */
#define MINBIAS 0		/* ?? */

/*	parameters
 */
#define LENNAME	63

#define ISDATA	1
#define ISLITS	2
#define ISTEXT	4
#define	ISBSS	8

/*	size in bytes
 */
#define BRSIZE	3
#define JCSIZE	4
#define JMPSIZE	4
#define RETSIZE	4
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
#define XXR1	1	/* special [not a resource] */
#define XXR2	2
#define XAC0	3
#define XF0		4
#define XVXR1	5

/*	the space (segment) codes
 */
#define SPORT	(SPACE) bit(1)
#define SBUI	(SPACE) bit(2)
#define SFAR	(SPACE) bit(3)
#define SVOL	(SPACE) bit(4)
#define SCONST	(SPACE) bit(5)
#define SFUNC	(SPACE) bit(6)
#define	SRVAL	(SPACE) bit(7)

/*	the want attributes
 */
#define WNULL	(WANTS) bit(0)
#define WTERM	(WANTS) bit(1)
#define WMEM	(WANTS) bit(2)
#define WVMEM	(WANTS) bit(3)
#define WIMM	(WANTS) bit(4)
#define WXR2	(WANTS) bit(5)
#define WVXR2	(WANTS) bit(6)
#define WAC0	(WANTS) bit(7)
#define WVAC0   (WANTS) bit(8)
#define WPMEM	(WANTS) bit(10)
#define WREREF	(WANTS) bit(11)

#define PTRSET	(WMEM|WPMEM|WAC0|WXR2)
#define VOLSET	(WVMEM|WVAC0|WVXR2)
#define WANY	(PTRSET|VOLSET|WIMM|WREREF)

/*	the got attributes
 */
#define GRVOL	(WANTS) bit(26)
#define GCCZERO	(WANTS) bit(27)
#define GCCSET	(WANTS) bit(28)
#define GJUNK	(WANTS) bit(29)
#define GVOL	(WANTS) bit(30)
#define GNOCODE	(WANTS) bit(31)

/*	the type cast codes
 */
#define TBYTE	0
#define TWORD	1
#define TLONG	2
#define TQUAD	3
#define TFLOAT	4
#define TDOUBLE	5

#define TPUTLEN	16
#define TNOPUTL 0

/*	the address descriptor
 */
typedef struct {
	TYPE ty;
	SPACE pspace;
	BYTES stsize;
	LONG bias;
	TEXT *nm;
	TINY idx, refs;
	SPACE space;
	} ADDR;

/*	the expression element
 */
typedef struct expr {
	struct expr *next;
	LEX op;
	WANTS got;
	ADDR f;
	union {
		struct {
			TYPE ty;
			SPACE pspace;
			BYTES stsize;
			struct expr *left, *right, *mid;
			} o;
		ADDR v;
		} e;
	TEXT name[4];
	} EXPR;

/*	the code list element
 */
typedef struct code {
	struct code *next;
	BYTES size;
	LEX inst;
	BYTES linenum;
	union {
		BYTES lbl;
		struct header *hdr;
		BYTES choff;
		} c;
	} CODE;

/*	the code header table element
 */
typedef struct header {
	struct header *next;
	CODE *first;
	BYTES offset;
	LABEL label;
	COUNT nrefs;
	BITS flags;
	COUNT lineno;
	} HEADER;

/*	types and attribute names used in code generation tables
 */
typedef UTINY ATTR;
typedef UTINY RES;

typedef struct {
	ATTR lattr;
	ATTR rattr;
	RES res;
	TEXT *str;
	} TAB;

/*	attributes and types for operands
 */
#define NOOP		0			/* No operand; matches constant 0 in opick */
#define CPWR2OP		(1 << 4)	/* Power of 2, less then 16 */
#define CLT256OP	(2 << 4)	/* Constant less than 256 */
#define IMMOP		(3 << 4)	/* Immediate operand, any value*/
#define	CONOP		(4 << 4)	/* Constant operands, 0 - 15 in type field*/
#define XROP		(5 << 4)	/* Operand in XR1 or XR2, strtostr only */
#define PXR2OP		(6 << 4)	/* Pointer XR2 (right operand only) */
#define XR2OP		(7 << 4)	/* In XR2 */
#define AC0OP		(8 << 4)	/* In AC0 */
#define MEMOP		(9 << 4)	/* In memory */

/*	pass two types
 */
#define	YTEST	000
#define YCHAR	001
#define	YUCHAR	002
#define	YSHORT	003
#define	YSFIELD	004
#define	YUSHORT	005
#define	YLONG	006
#define	YLFIELD	007
#define	YULONG	010
#define	YFLOAT	011
#define	YDOUBLE	012
#define	YPTRTO	013
#define	YLDOUBL	014
#define	YLPTRTO	015
#define	YSTRUCT	016

/*	result types
 */
#define NOCC		bit(0)	/* Condition-code undefined */
#define CCZERO 		bit(1)	/* Condition code ok if result is zero */
#define CCSET		bit(2)	/* Condition code ok */
#define CCONLY		bit(3)	/* Only condition code set */
#define RVOL		bit(4)	/* Right side volatile */
#define STOS		bit(5)	/* Left need not be volatile, struct assign */
#define ESCR		bit(7)	/* Escape to routine n (byte following)*/

/*	masks
 */
#define ATTRMASK	(~0 << 4)	/* attr & ATTRMASK yields attribute */
#define	TYPEMASK	~ATTRMASK	/* attr & TYPEMASK yields type */

/*	widening in optytab
 */
#define	LMW			bit(1)	/* Left and middle widening before picking */
#define	RW			bit(0)	/* Right widening */

/*	tags for literals
 */
#define	TAGB	1	/* a byte (8 bits) */
#define	TAGW	2	/* a word (16 bits) */
#define	TAGL	3	/* a long (32 bits) */
#define	TAGF	4	/* a float (32 bits) */
#define	TAGD	5	/* a double (64 bits) */
#define	TAGA	6	/* an address (name+[bias]) */
#define	TAGS	7	/* a string */
#define	TAGE	8	/* external */

#include "int2fdef.h"
