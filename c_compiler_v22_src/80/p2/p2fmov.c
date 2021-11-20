/*	FORCE TABLES
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	codes for force tables:

	0--	dest is stack
	1--	dest is hl
	2--	dest is bc
	3--	dest is mem (X)

	-0-	src is stack
	-1-	src is hl
	-2-	src is bc
	-3-	src is mem (Y or Z[W])
	-4-	src is de
	-5-	src is ptr on stack
	-6-	src is *hl
	-7-	src is *bc

	--1	clobbers hl
	--2	clobbers bc
	--4	clobbers de [not used]
 */

/*	double to mem
 */
LOCAL TEXT *dbmem[] {
	"\373bc->hl=&X;\4",
	"\363bc=&X;\4",
/* 	"\353hl<=sp;bc=&X;\4", */
	"\333hl=&Y;bc=&X;\4",
	0};

/*	double to stack
 */
LOCAL TEXT *dbstk[] {
	"\073hl=8+bc;\5",
	"\063hl+(bc=8);\5",
/* 	"\053hl<=sp+(bc=8);\5", */
	"\033hl=&Y[8];\5",
	0};

/*	force table deref
 */
GLOBAL TEXT *deref[] {
	"\241bc=^(hl=de)",
	"\231bc=(hl=Y)",
	"\221bc->hl=^hl",
	"\211bc=^hl",
	"\201bc=^(hl<=sp)",
	"\140hl=de=a^hl",
	"\130hl=Y",
	"\120hl=bc=a^hl",
	"\110hl=a^hl",
	"\100hl<=sp=a^hl",
	"\013bc=^hl=>sp",
	"\003bc=^(hl<=sp)=>sp",
	0};

/*	force table for int to register
 */
GLOBAL TEXT *intreg[] {
	"\271bc->hl=^hl",
	"\261bc=^hl",
/* 	"\251bc=^(hl<=sp)", */
	"\240bc=de",
	"\231bc=(hl=Y)",
	"\210bc=hl",
	"\200bc<=sp",
	"\170hl=bc=a^hl",
	"\160hl=a^hl",
/* 	"\150hl<=sp=a^hl", */
	"\140hl=de",
	"\130hl=Y",
	"\120hl=bc",
	"\100hl<=sp",
	0};

/*	force int to memory
 */
LOCAL TEXT *intmem[] {
	"\371hl=bc=a^->Y",
	"\361hl=a^->Y",
/* 	"\351hl<=sp=a^->Y", */
	"\341hl=de->Y",
	"\331hl=X->Y",
	"\321hl=bc->Y",
	"\310hl->Y",
	"\301hl<=sp->Y",
	0};

/*	force table for int to stack
 */
LOCAL TEXT *intstk[] {
	"\073bc->hl=^hl=>sp",
	"\063bc=^hl=>sp",
/* 	"\053bc=^(hl<=sp)=>sp", */
	"\040sp<=de",
	"\031hl=Y=>sp",
	"\020sp<=bc",
	"\010sp<=hl",
	0};

/*	long to mem
 */
LOCAL TEXT *lomem[] {
	"\373bc->hl=&X;\2",
	"\363bc=&X;\2",
/* 	"\353hl<=sp;bc=&X;\2", */
	"\333hl=&Y;bc=&X;\2",
	0};

/*	long to stack
 */
LOCAL TEXT *lostk[] {
	"\073hl=bc;\3",
	"\063\3",
/* 	"\053hl<=sp;\3", */
	"\033hl=&Y;\3",
	0};

/*	pointer moves for force
 */
GLOBAL TEXT *ptmov[] {
	"\240bc=de",
	"\230bc=&Y",
	"\210bc=hl",
	"\200bc<=sp",
	"\140hl=de",
	"\130hl=&Y",
	"\120hl=bc",
	"\100hl<=sp",
	"\040sp<=de",
	"\031hl=&Y=>sp",
	"\020sp<=bc",
	"\010sp<=hl",
	0};

/*	reref moves for force
 */
GLOBAL TEXT *reref[] {
	"\241hl=&Z+de->bc",
	"\231hl=W+(bc=&Z)->bc",
	"\221hl=&Z+bc->bc",
	"\211hl+(bc=&Z)->bc",
	"\201hl<=sp+(bc=&Z)->bc",
	"\140hl=&Z+de",
	"\132hl=W+(bc=&Z)",
	"\120hl=&Z+bc",
	"\112hl+(bc=&Z)",
	"\102hl<=sp+(bc=&Z)",
	0};

/*	force strategies
 */
GLOBAL FTAB ftab[] {
	WPHL, HL, XDOUBLE, ptmov,
	WPBC, BC, XDOUBLE, ptmov,
	WHL, HL, XUSHORT, intreg,
	WBC, BC, XUSHORT, intreg,
	WVMEM, R0|R1, XUSHORT, intmem,
	WVMEM, R0|R1, XFLOAT, lomem,
	WVMEM, R0|R1, XDOUBLE, dbmem,
	WPSTK, RPS|TS, XDOUBLE, ptmov,
	WSTACK, TS, XUSHORT, intstk,
	WSTACK, TS, XFLOAT, lostk,
	WSTACK, TS, XDOUBLE, dbstk,
	0};

LOCAL TINY binops[] {LTIMES, LDIVIDE, LMODULO, LPLUS, LMINUS,
	LLSHIFT, LRSHIFT, LAND, LXOR, LOR, LCOMP, DMINUS, DCMP, DLESS,
	LGETS, LGTIM, LGDIV, LGMOD, LGPLU, LGMIN,
	LGLSH, LGRSH, LGAND, LGXOR, LGOR, DGPLU, DGMIN, 0};
LOCAL TINY optytab[] {3, 0, 0, 3, 3,
	3, 0, 3, 3, 3, 3, 3, 3, 3,
	2, 3, 0, 0, 3, 3,
	3, 0, 3, 3, 3, 3, 3};

/*	the want expansions
 */
GLOBAL BITS wantab[] {WBC, WSTACK, WPSTK, WPHL,
	WBC|WHL, 0, 0, WHL,
	WBC|WHL|WPHL, 0, 0, WPBC|WPHL,
	WBC|WHL|WVMEM|WPBC|WPHL|WMEM, 0, 0, WVMEM|WMEM};

/*	emit code
 */
VOID emit(tab, leave)
	HEADER *tab;
	TEXT *leave;
	{
	IMPORT BYTES choff;
	IMPORT TEXT *jtext[];
	IMPORT TINY brops[];
	FAST CODE *q;
	FAST HEADER *p;

	for (p = tab; p; p = p->next)
		if (p->flags & TOEMIT)
			{
			if (0 < p->nrefs)
				putasm("%n: /%i\n", p->label, p->lineno);
			for (q = p->first; q; q = q->next)
				switch (q->inst)
					{
				case GNOP:
					break;
				case GHDR:
					if (0 < q->c.hdr->nrefs)
						putasm("%n: / %i\n", q->c.hdr->label, q->c.hdr->lineno);
					break;
				case GCODE:
					for (choff = q->c.choff; putasm(chread()); )
						;
					break;
				case GRET:
					if (q->c.hdr)
						putasm("%n:\n", q->c.hdr->label);
					putasm(leave);
					break;
				case GSWITCH:
					putasm("hl=&%n jmp c.switch\n", q->c.lbl);
					break;
				default:
					putasm("%p %n\n", jtext[scnstr(brops, q->inst)],
						q->c.hdr->label);
					break;
					}
			}
	}

/*	generate force code
 */
BITS fgen(p, reg, set, tab, rmatch)
	FAST EXPR *p;
	BITS reg, set;
	TEXT **tab;
	BOOL rmatch;
	{
	IMPORT TEXT noname[], *ptmov[];
	IMPORT TINY ctab[], xtab[];
	FAST TEXT *q;
	BITS xreg, uses;
	COUNT idx, lcode, rcode;

#ifdef DEBUG
putfmt("/fgen %o %o %o %o\n", reg, set, tab, rmatch);
putterm(p, 1);
#endif
	xreg = xtor(p->f.idx);
	if (xreg & reg & (R0|R1))
		{
		chput("sp=>af\n", NULL);
		p->f.idx = rtox(xreg);
		return (set);
		}
	if ((p->got & GVOL) && !(xreg & REGSET))
		set =| xreg;
	if (reg & RPS && xreg & (R0|R1))
		{
		reg = (p->got & GVOL) ? xreg : 0;
		idx = xreg & R0 ? XP0 : XP1;
		}
	else if (reg =& set)
		{
		reg =& (reg - 1) ^ reg;
		idx = rtox(reg);
		}
	else
		return (0);
	lcode = ctab[scnstr(xtab, idx)] << 3;
	rcode = ctab[scnstr(xtab, p->f.idx)];
	if (rmatch && p->f.refs && rcode <= 020)
		rcode =+ 050;
	if (p->got & GVOL || rcode)
		;
	else if (!(set & HL))
		return (0);
	else
		chput("hl<=sp=>sp=>sp\n", NULL);
	if (rcode != 050)
		;
	else if (lcode != 0100 && !(set & HL))
		return (0);
	else
		{
		rcode = 060;
		chput(p->got & GVOL ? "hl<=sp\n" : "hl<=sp=>sp\n", NULL);
		}
	set = set & ~reg | TS;
	uses = ((set & HL) ? 1 : 0) | ((set & BC) ? 2 : 0);
	while (q = *tab++)
		if (lcode == (*q & 0300) && rcode == (*q & 070))
			{
			if ((*q & uses) != (*q & 3))
				return (0);
#ifdef DEBUG
putfmt("/fgen %p\n", q + 1);
#endif
			while (*++q)
				switch (*q)
					{
				case 'X':
					chput(xnm(idx), NULL);
					break;
				case 'Y':
					if (p->f.idx)
						chput(xnm(p->f.idx), NULL);
					else
						{
						if (q[-1] == '&')
							;
						else if (!p->f.refs)
							chput("&", NULL);
						else if (!p->f.nm[0])
							chput("*", NULL);
						putnb(p->f.nm, p->f.bias);
						}
					break;
				case 'Z':
					putnb(p->f.nm, p->f.bias);
					break;
				case 'W':
					chput(xnm(p->f.idx), NULL);
					break;
				default:
					chwrite(q, 1);
					}
			chput("\n", NULL);
			cpybuf(p->f.nm, noname, LENNAME);
			p->f.bias = 0;
			p->f.idx = idx;
			p->got =| GVOL;
			return (set);
			}
	panic("FGEN");
	}

/*	pick table for one operand
 */
BOOL opick(p, code, match, ty)
	FAST EXPR *p;
	BITS code, ty;
	BOOL match;
	{
	TINY by;

	code =& BYTMASK;
	if (code <= 014 || 0374 <= code)
		return (iscons(p) && (by = code) == p->e.v.bias);
	else if (code == 015)
		return (iscons(p) && !(p->e.v.bias & ~BYTMASK));
	else if (code == 016)
		return (iscons(p) && !(p->e.v.bias & BYTMASK));
	else if (code == 017)
		return (iscons(p));
	else if ((code & 017) < ty || match && ty < (code & 017))
		return (NO);
	else
		return (!match || ((gotten(p) | WPSTK) & twant(code)));
	}

/*	pick a code table entry
 */
TEXT *pick(p, match)
	FAST EXPR *p;
	BOOL match;
	{
	IMPORT TEXT *(*bintab[])[];
	IMPORT TINY binops[], equtype[], optytab[], regtype[];
	FAST COUNT i;
	FAST TEXT **q;
	BITS lty, rty, mty;
	COUNT q00;

	i = scnstr(binops, p->op);
	lty = p->e.o.left->e.o.ty & 017;
	if (!match && p->op != DCMP && p->op != DLESS)
		lty = regtype[lty];
	rty = p->e.o.right->e.o.ty & 017;
	mty = p->e.o.ty & 017;
	if (optytab[i] & 2)
		{
		lty = equtype[lty];
		mty = equtype[mty];
		}
	if (optytab[i] & 1)
		rty = equtype[rty];
	for (q = bintab[i]; *q; ++q)
		{
		q00 = q[0][0] & BYTMASK;
		if (q[0][2] & 0100 && mty)
			;
		else if (opick(p->e.o.left, q00, match, lty)
			&& opick(p->e.o.right, q[0][1], NO, rty)
			&& (q00 <= 017 || opick(p, q00, NO, mty)))
			return (q[0]);
		}
	return (NULL);
	}

/*	decode wants from table control byte
 */
BITS twant(code)
	BITS code;
	{
	IMPORT BITS wantab[];

	if (0374 <= (code =& BYTMASK))
		return (wantab[0]);
	else
		return (wantab[code >> 4]);
	}
