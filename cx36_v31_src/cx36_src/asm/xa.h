/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - STANDARD HEADER
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

/* define various types needed
 */
typedef	COUNT	BS;			/* basic symbol (< 255)	*/
typedef	COUNT	LVAL;			/* lh(+rh) value (16 bits)	*/
/* default values */
#define	XRPTYES	1				/* report success	*/
#define	XRPTNO	0				/* report failure	*/
#define	XLST	"ls"			/* listing extension	*/
#define	XSYM	"sm"			/* symbol table	*/
#define	XR21	"o"				/* new rel o/p	*/
#define	XR20	"r"				/* old rel o/p	*/
#define	XOUT	"ou"			/* hex o/p	*/
#define	XERR	"er"			/* errors	*/
#define	XSRC	".s"			/* source	*/
#define	XLSRC	2				/* length of source xtn (+1) */
#define	XTMP	"xa"			/* base for tmp files	*/
#define	XMTMP	1				/* modifable char in tmp ext	*/
#define	MAX_FILENAME	32		/* longest file name	*/
/* alter these defines to change sizes of tables etc	*/
#define NLS_SIZE	673	/* no of distinct names (better if prime)	*/
#define	MAX_PSECT	5	/* no of psects (Whitesmiths rel rstrict 2) */
#define	LINE_SIZE	128	/* largest line to be handled	*/
#define	MAX_READ	512	/* 512 == optimum size	*/
#define	MAX_ICOUNT	10	/* depth of 'include' files	*/
#define	MP_SIZE		128	/* no. of tokens in macro pars	*/
#define	MAX_MLEVEL	10	/* depth of macro calls	*/
#define	MAX_IF_LEVEL	10	/* depth of conditional assembly	*/
#define	MAX_STAB	64	/* no. of strings/stab size	*/
#define	MAX_MTAB	64	/* no of macro tokens/mtab size	*/


#define	YES	1
#define	NO	0
#define	MAYBE	-1

#define	NBS	(*yylex)()		/* depends on pass	*/

#define	SF_NO(f,l)	((f & 017) << 12) | (l & 07777)
#define	F_NO(k)		(k >> 12) & 017		/* file no.	*/
#define	L_NO(k)		k & 07777		/* line no	*/

#define	F_TYPE(i)	nlist[i].n_type	/* identifier type	*/
#define	F_VAL(i)	nlist[i].n_val	/* identifier value	*/
#define	F_PS(i)		nlist[i].n_ps	/* program section	*/
#define	S_TYPE(i,t)	nlist[i].n_type = t
#define	S_VAL(i,t)	nlist[i].n_val = t
#define	S_PS(i,t)	nlist[i].n_ps = t

/* strings + fpnums stored in sep table space allocated as needed */
#define	STR_MASK	077
#define	STR_OFF		6
/* macros stored in sep table spaceallocated as needed	*/
#define	MAC_MASK	077
#define	MAC_OFF		6

/* control of listing line	- use macro for speed	*/
#define	CL_SIZE		27
#define	CL_LN		3	/*last digit in line no	*/
#define	CL_PC		8	/* pc position	*/
#define	CL_CSTART	14	/* start of code in listing	*/
#define	CL_END		26	/* end of code (tab or newline)	*/
#define	CLEAR_CL	cl_b_ptr = CL_CSTART

/* reserved words + special symbols */
#define	AUTOD	0200	/* --	*/
#define	AUTOI	0201	/* ++	*/
#define	ADDR	0202	/* addr(ess)	*/
#define	ASCII	0203	/* ascii 7/8 odd/even	*/
#define BYTE	0204	/* byte	*/
#define	DBLE	0205	/* double 	*/
#define DEF	0206	/* def(ine)	*/
#define	ELSESYM	0207	/* else	*/
#define	ENDIF	0210	/* endif*/
#define	ENDM	0211	/* endm(acro)	*/
#define	ENDR	0212	/* endr(epeat)	*/
#define	ENDSYM	0213	/* end	*/
#define	EQSYM	'='
#define	EQU	0214	/* equ			*/
#define	EQUIV	0215	/* equivalent (==)	*/
#define	EVEN	0216	/* even	*/
#define	EXITM	0217	/* exitm(acro)	*/
#define	EXT	0220	/* ext(ernal)	*/
#define	FLOATS	0253	/* 4 bytes	*/
#define	GE	0221	/* greater than or eq	*/
#define	GTSYM	'>'
#define	IFSYM	0222	/* if	*/
#define	INCLD	0223	/* include	*/
#define	LBYTE	0224	/* l(ower)byte	*/
#define	LE	0225	/* less than or equal	*/
#define	LFLOAT	0252	/* l(ong) float	*/
#define	LINUM	0226	/* lineno	*/
#define	LIST	0227	/* list	*/
#define	LOGAND	0230	/* logical and && */
#define	LOGOR	0231	/* logical or ||	*/
#define	LTSYM	'<'
#define	MACRO	0232	/* macro	*/
#define	MODSYM	'%'
#define	NE	0233	/* not equal	*/
#define	ODD	0234	/* odd		*/
#define	OPTN	0235	/* opt(io)n	*/
#define	ORIGIN	0236	/* or(i)g(en)	*/
#define	PAGE	0237	/* page	*/
#define	PROCESSOR	0240	/* processor	*/
#define	PSECT	0241	/* p(rogram) sect(ion)	*/
#define	PUBLIC	0242	/* public (definition)	*/
#define	REPEAT	0243	/* repeat	*/
#define	SET	0244	/* set	*/
#define	SHL	0245	/*SHift Left	*/
#define	SHR	0246	/*SHift Right	*/
#define	TEXTSYM	0247	/* text	*/
#define	TITLE	0254	/* title	*/
#define	UBYTE	0250	/* u(pper) byte	*/
#define	WORD	0251	/* word */

/* define various types */
#define	UNK	0	/* unknown	*/
#define	LABEL	1	/* label	*/
#define	MAC	2	/* macro	*/
#define	CONST	3	/* usr defined constant	*/
#define	VAR	4	/* usr defined var	*/
#define PSCT	5	/* program section no	*/
#define	SPECIAL	6	/* special (depends on u_type	*/
#define	ISPECIAL	7	/* internal use only	*/

#define	EXT_T	010
#define	PUB_T	020

#define	NRQRD	0100	/* == UNK but when not copying */
/* define class names */

#define	ID	1	/* identifier	*/
#define	NUMBER	2	/* any const	*/
#define	STRNG	3
#define	OP	4	/* opcode for selected u-proc	*/
#define	NLABEL	5	/* numeric (local) label	*/
#define	MPAR	6	/* macro paramter	*/
#define	MACRO_ID	7	/* macro name	*/
#define	FPNUM	8	/* real number */
#define	LGNUM	016	/* long number	*/

/* BOB = special marker for start of lexical block
 * EOB = special marker for end of lexical block
 * BIF =	"	    start of incld file
 * EIF =        "           end of included file
 */
#define	BOB	034
#define	BIF	035
#define	EOB	036
#define	EIF	037

#define	FORM_FEED	014

/* define special types of char (use with ctable)	*/
#define	UC	1	/* upper case */
#define	LC	2	/* lower case */
#define	NC	4	/* digit */
#define	HC	8	/* hex digit */
#define	XC	16	/* allowed id char */
#define	SC	32	/* allowed string char */
#define	ALFA	UC|LC|NC|XC
/* expression() returns results in a structure of type
 * result.  The fields include details about any label
 * used as reference (for relocation purposes) + any offset
 * from this label + for convenience the 'value' to be
 * placed in any opcode etc.
 */
struct	result	{
	COUNT	r_label;	/* -1 == none	*/
	TINY	r_ps;		/* psect (0 == abs)	*/
	LVAL	r_offset;	/* x in L + x	*/
	LVAL	r_value;	/* really unnecessary but convenient */
	BOOL	r_unk;		/* flag set if unknown label	*/
};
/* define name-table	*/
struct nlist_item
{
	TINY	n_type;
	TINY	n_ps;		/* psect of definition	*/
	LVAL	n_val;
	TEXT	*n_ptr;
	COUNT	n_sdinum;	/* number of sdi's seen	*/
};
/* define structure holding presets for each proc type
 * will be entered by put_in_nametable
 */
struct	presets{
	TEXT	*pset_name;
	COUNT	pset_type;
	LVAL	pset_value;
};
/* define form of table to hold op-codes for each machine */
struct opitem
{
	TEXT	*opcode;
	TINY	optype;
	TINY	opbase;
};

/* various items (optable,assemble etc)
 * are set up by init routine found from a table of supported devices
 * held in 'type-pars' form is as below:
 */
struct	tp_item	{
	TEXT		*tp_uid;		/* u_proc name	*/
	VOID		(*tp_init)();	/* ref to 'init' function	*/
};
struct	rsw_item	{	/* reserved word table	*/
	TEXT	*rw_ptr;
	LVAL	rw_val;
	};
/* define psect variables */
struct psect_item
{
	LVAL	ps_high;
	TINY	ps_nptr;	/* it's name	*/
	FILE	ps_tfile;	/* chl used in pass 2 to o/p code	*/
	TEXT	*ps_tfname;	/* name of temp file			*/
	LONG	ps_tlen;	/* length of temp file	*/
	COUNT	ps_osdi;		/* prev sdi count	*/
	FILE	ps_cfile;	/* chl used in pass2 to o/p control bytes */
	TEXT	*ps_cfname;	/* name of c temp file			*/
	LONG	ps_clen;	/* length of c temp file	*/
};
#define	ABS_PS	0		/* absolute psect	*/
#define	TEXT_PS	1		/* _text	*/
#define	DATA_PS	2		/* _data	*/
#define	ZP_PS	3		/* _zpage	*/
/* stack for include files + line nos */
struct ic_item{
	COUNT	icln;
	COUNT	icch;
	TEXT	*icrb;
	TEXT	*icrp;
	TEXT	*icrl;
	};
/* objects connected with macro definitions + expansions
	macro definitions will be read (as tokens) + stored in
	mvars.
	During the expansion the paramter line will be read (as tokens) and
	stored from 0 .. MAX_MP of mvars - the starts being held in pstarts.
 */
struct mtoken {
	BS	mc_val;	/* = value returned by yylex */
	LVAL 	mc_lv;	/* = lv		*/
	};
struct	m_mpar_info	{
	COUNT	mp_plb;		/* previous lex block	*/
	COUNT	mp_slb;		/* stack for enclosing lex blk	*/
	struct m_npar_info *mp_pst;		/* enclosing macro pstarts	*/
	COUNT	mp_mt;		/* start of macro body in mvars	*/
	COUNT	mp_smt;		/* save current mt during par xpsn	*/
	COUNT	mp_rpt;		/* repeat/macro	*/
	COUNT	mp_pif;		/* previous if level	*/
	COUNT	mp[10];		/* [0] == no. of pars
				 * [1..9] == start index of pars	*/
	};

/* conditional assembly objects
 *  uses iflevel to keep track of nesting depth
 *  + c_if to decide which branch to assemble
 */

/* listing flags used to control just which parts are listed
 */
#define	LIST_on		001
#define	LIST_text	002
#define	LIST_macro	004
#define	LIST_if		010
#define	LIST_include	020

/* global variables flags etc
 * are allocated + initialised near main
 */

