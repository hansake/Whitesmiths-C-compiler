/*	OUTPUT TABLES
 *	copyright (c) 1978,	1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int012.h"
#include "int12.h"

/*	CODES FOR TEXT BYTES:
	L	put	left address mode
	R	put	right address mode
	Cx	complement value before	putting
	Nx	don't generate size	information	when putting
	Ux	up address to next less significant byte when putting
	Hx	get	next more significant constant byte when putting
	B	operands are byte sized
	D	operands are double	sized
	F	operands are float sized
	Q	operands are 8-byte	int	sized
	S	operands are long sized
	W	operands are short sized
	T	put	created	label
	K	put	call
	(=n	put	literal of n bytes (n =	2 or 4))
	;	put	newline
 */

LOCAL TAB andtab[] = {
	MEMOP|YUCHAR,	IMMOP,			NOCC,	"^sbfB NL,CR",
	MEMOP|YUSHORT,	IMMOP,			NOCC,	"^sbfB NL,CHR;^sbfB UNL,CR",
	MEMOP|YULONG,	IMMOP,			NOCC,	"^sbfB NL,CHHHR;^sbfB UNL,CHHR;\
^sbfB UUNL,CHR;^sbfB UUUNL,CR",
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,	"^Kiand",
	AC0OP|YULONG,	PXR2OP|YULONG,	NOCC,	"^Kland",
	0};

LOCAL TAB cmptab[] = {
	MEMOP|YCHAR,	IMMOP,			CCZERO|CCONLY,	"^cliB NL,R",
	MEMOP|YCHAR,	MEMOP|YCHAR,	CCZERO|CCONLY,	"^clcB L,R",
	MEMOP|YUCHAR,	MEMOP|YUCHAR,	CCSET|CCONLY,	"^clcB L,R",
	MEMOP|YSHORT,	MEMOP|YSHORT,	CCZERO|CCONLY,	"^clcW L,R",
/*	MEMOP|YSFIELD,	IMMOP,			ESCR|CCZERO,	"\000",	/* tstbits() */
	MEMOP|YUSHORT,	MEMOP|YUSHORT,	CCSET|CCONLY,	"^clcW L,R",
	MEMOP|YLONG,	MEMOP|YLONG,	CCZERO|CCONLY,	"^clcS L,R",
	MEMOP|YULONG,	MEMOP|YULONG,	CCSET|CCONLY,	"^clcS L,R",
	MEMOP|YFLOAT,	MEMOP|YFLOAT,	CCZERO|CCONLY,	"^clcF L,R",
	MEMOP|YDOUBLE,	MEMOP|YDOUBLE,	CCZERO|CCONLY,	"^clcD L,R",
	0};

LOCAL TAB lestab[] = {
	MEMOP|YCHAR,	IMMOP,			ESCR|CCSET,		"\004",	/* signicmp() */
	MEMOP|YCHAR,	MEMOP|YCHAR,	ESCR|CCSET|RVOL,"\010",	/* signcmp() */
	MEMOP|YUCHAR,	IMMOP,			CCSET|CCONLY,	"^cliB NL,R",
	MEMOP|YUCHAR,	MEMOP|YUCHAR,	CCSET|CCONLY,	"^clcB L,R",
	MEMOP|YSHORT,	IMMOP,			ESCR|CCSET,		"\004",	/* signicmp() */
	MEMOP|YSHORT,	MEMOP|YSHORT,	ESCR|CCSET|RVOL,"\010",	/* signcmp() */
	MEMOP|YUSHORT,	MEMOP|YUSHORT,	CCSET|CCONLY,	"^clcW L,R",
	MEMOP|YLONG,	IMMOP,			ESCR|CCSET,		"\004",	/* signicmp() */
	MEMOP|YLONG,	MEMOP|YLONG,	ESCR|CCSET|RVOL,"\010",	/* signcmp() */
	MEMOP|YULONG,	MEMOP|YULONG,	CCSET|CCONLY,	"^clcS L,R",
	AC0OP|YFLOAT,	MEMOP|YFLOAT,	CCSET|CCONLY,	"^laB NR,#wr7;^Kfcmp",
	AC0OP|YDOUBLE,	MEMOP|YDOUBLE,	CCSET|CCONLY,	"^laB NR,#wr7;^Kdcmp",
	0};

LOCAL TAB comtab[] = {
	MEMOP|YUSHORT,	0,				CCZERO,		"^la -1,#wr4;^s NL,#wr4;\
^st NL,#wr4",
	MEMOP|YULONG,	0,				CCZERO,		"^mvc @ac0-1(4),@const-1;\
^slcS @ac0-1(4),NL;^mvcS L,@ac0-1",
	0};

LOCAL TAB divtab[] = {
	AC0OP|YCHAR,	CPWR2OP,		ESCR|NOCC,	"\011",	/* divimm()	*/
	MEMOP|YUCHAR,	CPWR2OP,		ESCR|NOCC,	"\011",	/* divimm()	*/
	AC0OP|YSHORT,	CPWR2OP,		ESCR|NOCC,	"\011",	/* divimm()	*/
	AC0OP|YSHORT,	PXR2OP|YSHORT,	NOCC,		"^Kidiv",
	MEMOP|YUSHORT,	CPWR2OP,		ESCR|NOCC,	"\011",	/* divimm()	*/
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kuidiv",
	AC0OP|YLONG,	CPWR2OP,		ESCR|NOCC,	"\011",	/* divimm()	*/
	AC0OP|YLONG,	PXR2OP|YLONG,	NOCC,		"^Kldiv",
	MEMOP|YULONG,	CPWR2OP,		ESCR|NOCC,	"\011",	/* divimm()	*/
	AC0OP|YULONG,	PXR2OP|YULONG,	NOCC,		"^Kuldiv",
	AC0OP|YFLOAT,	MEMOP|YFLOAT,	NOCC,		"^laB NR,#wr7;^Kfdiv",
	AC0OP|YDOUBLE,	MEMOP|YDOUBLE,	NOCC,		"^laB NR,#wr7;^Kddiv",
	0};

LOCAL TAB getstrtab[] =	{
	XROP|YUSHORT,	XROP|YUSHORT,	ESCR|NOCC|STOS,	"\012",	/* strtostr() */
	0};

LOCAL TAB gettab[] = {
	MEMOP|YUCHAR,	IMMOP,			NOCC,		"^mviB NL,R",
	MEMOP|YUCHAR,	MEMOP|YCHAR,	NOCC,		"^mvcB L,R",
	MEMOP|YUCHAR,	MEMOP|YUCHAR,	NOCC,		"^mvcB L,R",
	MEMOP|YUCHAR,	MEMOP|YSHORT,	NOCC,		"^mvcB L,UR",
	MEMOP|YUCHAR,	MEMOP|YUSHORT,	NOCC,		"^mvcB L,UR",
	MEMOP|YUCHAR,	MEMOP|YLONG,	NOCC,		"^mvcB L,UUUR",
	MEMOP|YUCHAR,	MEMOP|YULONG,	NOCC,		"^mvcB L,UUUR",
	MEMOP|YSFIELD,	IMMOP,			ESCR|NOCC,	"\001",	/* ntobits() */
	MEMOP|YSFIELD,	MEMOP|YUSHORT,	ESCR|NOCC|RVOL,	"\002",	/* utobits() */
	MEMOP|YUSHORT,	IMMOP,			NOCC,		"^mviB NL,HR;^mviB UNL,R",
	MEMOP|YUSHORT,	MEMOP|YCHAR,	NOCC,		"^mviB NL,0;^tbn R,128;\
^jf T;^mviB NL,255;T: mvcB UL,R",
	MEMOP|YUSHORT,	MEMOP|YUCHAR,	NOCC,		"^mviB NL,0;^mvcB UL,R",
	MEMOP|YUSHORT,	MEMOP|YSHORT,	NOCC,		"^mvcW L,R",
	MEMOP|YUSHORT,	MEMOP|YSFIELD,	ESCR|NOCC,	"\003",	/* bitstou() */
	MEMOP|YUSHORT,	MEMOP|YUSHORT,	NOCC,		"^mvcW L,R",
	MEMOP|YUSHORT,	MEMOP|YLONG,	NOCC,		"^mvcW L,UUR",
	MEMOP|YUSHORT,	MEMOP|YULONG,	NOCC,		"^mvcW L,UUR",
	XR2OP|YUSHORT,	MEMOP|YCHAR,	NOCC,		"^mvcB @xr2+1(1),R;\
^mvi @xr2,0;^tbn @xr2+1,128;^jf T;^mvi @xr2,255;T: l @xr2+1,#xr2",
	XR2OP|YUSHORT,	MEMOP|YUCHAR,	NOCC,		"^mvcB @xr2+1(1),R;\
^mvi @xr2,0;^l @xr2+1,#xr2",
	XR2OP|YUSHORT,	MEMOP|YSHORT,	NOCC,		"^l R,#xr2",
	XR2OP|YUSHORT,	MEMOP|YUSHORT,	NOCC,		"^l R,#xr2",
	XR2OP|YUSHORT,	MEMOP|YLONG,	NOCC,		"^l UUR,#xr2",
	XR2OP|YUSHORT,	MEMOP|YULONG,	NOCC,		"^l UUR,#xr2",
	AC0OP|YUSHORT,	MEMOP|YFLOAT,	NOCC,		"^laB NR,#wr7;^Kfti",
	AC0OP|YUSHORT,	MEMOP|YDOUBLE,	NOCC,		"^laB NR,#wr7;^Kdti",
	MEMOP|YULONG,	MEMOP|YCHAR,	NOCC,		"^mviB NL,0;^mvi UNL,0;\
^mviB UUNL,0;^tbn R,128;^jf T;^mviB NL,255;^mviB UNL,255;\
^mviB UUNL,255;T: mvc B UUUL,R",
	MEMOP|YULONG,	MEMOP|YUCHAR,	NOCC,		"^mviB NL,0;^mviB UNL,0;\
^mviB UUNL,0;^mvcB UUUL,R",
	MEMOP|YULONG,	MEMOP|YSHORT,	NOCC,		"^mviB NL,0;^mviB UNL,0;\
^tbn R,128;^jf T;^mviB NL,255;^mviB UNL,255;T: mvcW UUL,R",
	MEMOP|YULONG,	MEMOP|YUSHORT,	NOCC,		"^mviB NL,0;^mviB UNL,0;\
^mvcW UUL,R",
	MEMOP|YULONG,	MEMOP|YLONG,	NOCC,		"^mvcS L,R",
	MEMOP|YULONG,	MEMOP|YULONG,	NOCC,		"^mvcS L,R",
	AC0OP|YULONG,	MEMOP|YFLOAT,	NOCC,		"^laB NR,#wr7;^Kftl",
	AC0OP|YULONG,	MEMOP|YDOUBLE,	NOCC,		"^laB NR,#wr7;^Kdtl",
	AC0OP|YFLOAT,	MEMOP|YSHORT,	NOCC,		"^laB NR,#wr7;^Kitf",
	AC0OP|YFLOAT,	MEMOP|YUSHORT,	NOCC,		"^laB NR,#wr7;^Kuitf",
	AC0OP|YFLOAT,	MEMOP|YLONG,	NOCC,		"^laB NR,#wr7;^Kltf",
	AC0OP|YFLOAT,	MEMOP|YULONG,	NOCC,		"^laB NR,#wr7;^Kultf",
	MEMOP|YFLOAT,	MEMOP|YFLOAT,	NOCC,		"^mvcF L,R",
	AC0OP|YFLOAT,	MEMOP|YDOUBLE,	NOCC,		"^laB NR,#wr7;^Kdtf",
	AC0OP|YDOUBLE,	MEMOP|YSHORT,	NOCC,		"^laB NR,#wr7;^Kitd",
	AC0OP|YDOUBLE,	MEMOP|YUSHORT,	NOCC,		"^laB NR,#wr7;^Kuitd",
	AC0OP|YDOUBLE,	MEMOP|YLONG,	NOCC,		"^laB NR,#wr7;^Kltd",
	AC0OP|YDOUBLE,	MEMOP|YULONG,	NOCC,		"^laB NR,#wr7;^Kultd",
	AC0OP|YDOUBLE,	MEMOP|YFLOAT,	NOCC,		"^laB NR,#wr7;^Kftd",
	MEMOP|YDOUBLE,	MEMOP|YDOUBLE,	NOCC,		"^mvcD L,R",
	0};

LOCAL TAB lshtab[] = {
	MEMOP|YUSHORT,	IMMOP,			ESCR|NOCC,	"\005",	/* dolsh() */
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kilsh",
	MEMOP|YULONG,	IMMOP,			ESCR|NOCC,	"\005",	/* dolsh() */
	AC0OP|YULONG,	PXR2OP|YUSHORT,	NOCC,		"^Kllsh",
	0};

TAB minxr2[] = {
	XR2OP|YUSHORT,	MEMOP|YUSHORT,	CCZERO,		"^s R,#xr2",
	0
	};

LOCAL TAB mintab[] = {
	MEMOP|YUCHAR,	IMMOP,			CCZERO,			"^sliB L,R",
	MEMOP|YUCHAR,	MEMOP|YUCHAR,	CCZERO,			"^slcB L,R",
	MEMOP|YUCHAR,	MEMOP|YUSHORT,	CCZERO,			"^slcB L,UR",
	MEMOP|YUSHORT,	MEMOP|YUSHORT,	CCZERO,			"^slcW L,R",
	MEMOP|YULONG,	MEMOP|YULONG,	CCZERO,			"^slcS L,R",
	AC0OP|YFLOAT,	MEMOP|YFLOAT,	CCONLY|CCSET,	"^laB NR,#wr7;^Kfcmp",
	AC0OP|YFLOAT,	MEMOP|YFLOAT,	NOCC,			"^laB NR,#wr7;^Kfsub",
	AC0OP|YDOUBLE,	MEMOP|YDOUBLE,	CCONLY|CCSET,	"^laB NR,#wr7;^Kdcmp",
	AC0OP|YDOUBLE,	MEMOP|YDOUBLE,	NOCC,			"^laB NR,#wr7;^Kdsub",
	0};

LOCAL TAB modtab[] = {
	AC0OP|YSHORT,	PXR2OP|YSHORT,	NOCC,		"^Kimod",
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kuimod",
	AC0OP|YLONG,	PXR2OP|YLONG,	NOCC,		"^Klmod",
	AC0OP|YULONG,	PXR2OP|YULONG,	NOCC,		"^Kulmod",
	0};

LOCAL TAB negtab[] = {
	MEMOP|YUSHORT,	0,				CCZERO,		"^la 0,#wr4;^s NL,#wr4;\
^st NL,#wr4",
	MEMOP|YULONG,	0,				CCZERO,		"^mvc @ac0-1(4),@const+3;\
^slcS @ac0-1(4),NL;^mvcS L,@ac0-1",
	AC0OP|YFLOAT,	0,				NOCC,		"^laB NL,#wr7;^Kfneg",
	AC0OP|YDOUBLE,	0,				NOCC,		"^laB NL,#wr7;^Kdneg",
	0};

LOCAL TAB iortab[] = {
	MEMOP|YUCHAR,	IMMOP,			NOCC,		"^sbnB NL,R",
	MEMOP|YUSHORT,	IMMOP,			NOCC,		"^sbnB NL,HR;^sbnB UNL,R",
	MEMOP|YULONG,	IMMOP,			NOCC,		"^sbnB NL,HHHR;^sbnB UNL,HHR;\
^sbnB UUNL,HR;^sbnB UUUNL,R",
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kior",
	AC0OP|YULONG,	PXR2OP|YULONG,	NOCC,		"^Klor",
	0};

TAB pluxr2[] = {
	XR2OP|YUSHORT,	CLT256OP,		CCZERO,		"^la R(,#xr2),#xr2",
	XR2OP|YUSHORT,	MEMOP|YUSHORT,	CCZERO,		"^a R,#xr2",
	0
	};

LOCAL TAB plutab[] = {
	MEMOP|YUCHAR,	IMMOP,			CCZERO,		"^aliB NL,R",
	MEMOP|YUCHAR,	MEMOP|YUCHAR,	CCZERO,		"^alcB L,R",
	MEMOP|YUCHAR,	MEMOP|YUSHORT,	CCZERO,		"^alcB L,UR",
	MEMOP|YUSHORT,	MEMOP|YUSHORT,	CCZERO,		"^alcW L,R",
	MEMOP|YULONG,	MEMOP|YULONG,	CCZERO,		"^alcS L,R",
	AC0OP|YFLOAT,	MEMOP|YFLOAT,	NOCC,		"^laB NR,#wr7;^Kfadd",
	AC0OP|YDOUBLE,	MEMOP|YDOUBLE,	NOCC,		"^laB NR,#wr7;^Kdadd",
	0};

LOCAL TAB rshtab[] = {
	AC0OP|YSHORT,	IMMOP,			ESCR|NOCC,	"\006",	/* dorsh() */
	AC0OP|YSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kirsh",
	MEMOP|YUSHORT,	IMMOP,			ESCR|NOCC,	"\006",	/* dorsh() */
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kuirsh",
	AC0OP|YLONG,	IMMOP,			ESCR|NOCC,	"\006",	/* dorsh() */
	AC0OP|YLONG,	PXR2OP|YUSHORT,	NOCC,		"^Klrsh",
	MEMOP|YULONG,	IMMOP,			ESCR|NOCC,	"\006",	/* dorsh() */
	AC0OP|YULONG,	PXR2OP|YUSHORT,	NOCC,		"^Kulrsh",
	0};

LOCAL TAB timtab[] = {
	MEMOP|YUCHAR,	CPWR2OP,		ESCR|NOCC,	"\007",	/* mulimm()	*/
	MEMOP|YUSHORT,	CPWR2OP,		ESCR|NOCC,	"\007",	/* mulimm()	*/
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kimul",
	MEMOP|YULONG,	CPWR2OP,		ESCR|NOCC,	"\007",	/* mulimm()	*/
	AC0OP|YULONG,	PXR2OP|YULONG,	NOCC,		"^Klmul",
	AC0OP|YFLOAT,	MEMOP|YFLOAT,	NOCC,		"^laB NR,#wr7;^Kdmul",
	AC0OP|YDOUBLE,	MEMOP|YDOUBLE,	NOCC,		"^laB NR,#wr7;^Kdmul",
	0};

LOCAL TAB xortab[] = {
	AC0OP|YUSHORT,	PXR2OP|YUSHORT,	NOCC,		"^Kixor",
	AC0OP|YULONG,	PXR2OP|YULONG,	NOCC,		"^Klxor",
	0};

/*	table selectors
 */
LOCAL TAB *bintab[]	= {
	timtab,	divtab,	modtab,	plutab,	mintab,
	lshtab,	rshtab,	andtab,	xortab,	iortab,	comtab,	negtab,	cmptab,	lestab,
	gettab,	timtab,	divtab,	modtab,	plutab,	mintab,
	lshtab,	rshtab,	andtab,	xortab,	iortab,	plutab,	mintab,	getstrtab};

LOCAL TINY binops[]	= {
	LTIMES,	LDIVIDE, LMODULO, LPLUS, LMINUS,
	LLSHIFT, LRSHIFT, LAND,	LXOR, LOR, LCOMP, DMINUS, DCMP,	DLESS,
	LGETS, LGTIM, LGDIV, LGMOD,	LGPLU, LGMIN,
	LGLSH, LGRSH, LGAND, LGXOR,	LGOR, DGPLU, DGMIN,	GGETSTR, 0};

/*	widening in	optyptab
	LMW		Left and middle	widening before	picking
	RW		Right widening
 */
LOCAL TINY optytab[] = {
	LMW, 0,	0, LMW,	LMW,
	LMW|RW,	RW,	LMW|RW,	LMW|RW,	LMW|RW,	LMW|RW,	LMW|RW,	0, 0,
	LMW, LMW, 0, 0,	LMW, LMW,
	LMW|RW,	RW,	LMW|RW,	LMW|RW,	LMW, LMW, LMW|RW, 0};

/*	break functions
 */
GLOBAL TAB *(*gfun[])()	= {
	tstbits,			/* #0 "\000" */
	ntobits,			/* #1 "\001" */
	utobits,			/* #2 "\002" */
	bitstou,			/* #3 "\003" */
	signicmp,			/* #4 "\004" */
	dolsh,				/* #5 "\005" */
	dorsh,				/* #6 "\006" */
	mulimm,				/* #7 "\007" */
	signcmp,			/* #8 "\010" */
	divimm,				/* #9 "\011" */
	strtostr,			/* #10 "\012" */
	};

/*	the	want expansions
 */
GLOBAL WANTS wantab[] =	{
	/* NOOP	*/		WIMM,
	/* CPWR2OP */	WIMM,
	/* CLT256OP	*/	WIMM,
	/* IMMOP */		WIMM,
	/* CONOP */		WIMM,
	/* XROP	*/		WREREF|WIMM|WVXR2|WMEM|WVMEM|WPMEM|WVAC0,
	/* PXR2OP */	WMEM|WVMEM|WPMEM,
	/* XR2OP */		WXR2|WVXR2,
	/* AC0OP */		WAC0|WVAC0,
	/* MEMOP */		WMEM|WVMEM|WPMEM|WAC0|WVAC0,
	};

/*	emit code
 */
VOID emit(tab, leave)
	HEADER *tab;
	TEXT *leave;
	{
	IMPORT BOOL	inemit,	asmflag, upcase;
	IMPORT BYTES choff;
	IMPORT TEXT	*jtext[];
	IMPORT TINY	brops[];
	FAST BYTES size;
	FAST CODE *q;
	FAST HEADER	*p;
	INTERN TEXT	lblfmt[] = {"%n:/line %i, bytes %i;"};

	inemit = YES;
	for	(p = tab; p; p = p->next)
		if (p->flags & TOEMIT)
			{
			if (0 <	p->nrefs)
				{
				upcase = NO;
				putasm(lblfmt, p->label, p->lineno,	p->offset);
				upcase = YES;
				}
			for	(size =	0, q = p->first; q;	q =	q->next)
				{
				size +=	q->size;
				asmflag	= YES;
				if (q->linenum)
					dolist(q->linenum);
				asmflag	= NO;
				switch (q->inst)
					{
				case GNOP:
					break;
				case GHDR:
					if (0 <	q->c.hdr->nrefs)
						{
						upcase = NO;
						putasm(lblfmt, q->c.hdr->label,	q->c.hdr->lineno,
							q->c.hdr->offset);
						upcase = YES;
						}
					break;
				case GCODE:
					for	(choff = q->c.choff; putasm(chread()); )
						;
					break;
				case GRET:
					if (q->c.hdr)
						putasm("%n:;", q->c.hdr->label);
					putasm("%p", leave);
					break;
				case GLSWITCH:
				case GSWITCH:
					putasm("^la %n,#xr2;", q->c.lbl);
					putcall("^bd ",	(q->inst ==	GLSWITCH) ?	"jltab"	:
						"jtab");
					break;
				default:
					putasm("%p %n;", jtext[scnstr(brops, q->inst)],
						q->c.hdr->label);
					break;
					}
				}
			}
	inemit = NO;
	}

/*	pick table for one operand
 */
BOOL opick(p, attr,	got, ty)
	FAST EXPR *p;
	ATTR attr;
	WANTS got;
	TYPE ty;
	{
	LONG bias =	p->e.v.bias;
	FAST at	= attr & ATTRMASK;

	if (at <= CONOP)
		{
		if (!iscons(p))
				return (NO);
		switch (at)
			{
		case CPWR2OP:
			return (0 <	bias &&	(((bias - 1) ^ bias) & bias) ==	bias);
		case CLT256OP:
			return (0 <= bias && bias <	256);
		case IMMOP:
			return (YES);
		case CONOP:
			return (bias <=	4);
		default:
			return (YES);
			}
		}
	
	if (at == MEMOP	&& (got	& WIMM))
		return(YES);
	if ((attr &	TYPEMASK) <	ty || got && ty	< (attr	& TYPEMASK))
		return (NO);
	return (got	== 0 ||	(got & twant(attr)));
	}

/*	pick a code	table entry
 */
TAB *pick(p, match, want)
	FAST EXPR *p;
	BOOL match;
	WANTS want;
	{
	IMPORT TAB *bintab[];
	IMPORT TINY	binops[], equtype[], optytab[],	regtype[];
	FAST COUNT i;
	FAST TAB *tab;
	WANTS got;
	TYPE lty, rty, mty;

	i =	scnstr(binops, p->op);
	lty	= p->e.o.left->e.o.ty &	TYPEMASK;
	rty	= p->e.o.right->e.o.ty & TYPEMASK;
	mty	= p->e.o.ty	& TYPEMASK;
dbprint("/pick lty %Y mty %Y rty %Y\n",	lty, mty, rty);
	if (!match && p->op	!= DCMP	&& p->op !=	DLESS && mty)
		lty	= regtype[lty];
	if (optytab[i] & LMW)
		{
		lty	= equtype[lty];
		mty	= equtype[mty];
		}
	if (optytab[i] & RW)
		rty	= equtype[rty];
dbprint("/pick-middle lty %Y mty %Y	rty %Y\n", lty,	mty, rty);
	got	= (match) ?	gotten(p->e.o.left)	: 0;
	tab = bintab[i];
	if ((want & (WXR2|WVXR2)) && lty == YUSHORT && mty == YUSHORT)
		if (tab == plutab)
			tab = pluxr2;
		else if (tab == mintab)
			tab = minxr2;
	for	(; tab->lattr /*	end	of table */; ++tab)
		{
		if ((tab->res &	CCONLY)	&& mty)
			;
		else if	(opick(p->e.o.left,	tab->lattr,	got, lty)
			&& opick(p->e.o.right, tab->rattr,(WANTS) 0, rty)
			&& opick(p,	tab->lattr,	(WANTS)	0, mty))
			{
dbprint("/pick-returns %T\n", tab);
			return (tab);
			}
		}
	return (NULL);
	}

/*	decode wants from table	control	byte
 */
WANTS twant(attr)
	ATTR attr;
	{
	IMPORT WANTS wantab[];

	return (wantab[(attr >>	4) & TYPEMASK]);
	}
