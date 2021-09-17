/*	GENERATE CODE FOR EXPRESSIONS
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	strategies
 */
#define LR		0
#define RL		1
#define LTR		2

#define FAIL	1
#define SUC		2

/*	automatic offsets and register stuff
 */
GLOBAL BYTES autmin {0};
GLOBAL BYTES autoff {0};
GLOBAL BITS regset {0};
GLOBAL BITS regmin {0};

/*	the character pool offset
 */
GLOBAL BYTES choff {0};

/*	constant zero
 */
GLOBAL EXPR exzero {0};

/*	are the addresses exactly the same
 */
BOOL adequ(pl, pr)
	FAST ADDR *pl, *pr;
	{
	IMPORT TINY equtype[];

	if (pl->idx != pr->idx || pl->refs != pr->refs || pl->idx == XSP)
		;
	else if (equtype[pl->ty & 017] != equtype[pr->ty & 017])
		;
	else if (cmpbuf(pl->nm, pr->nm, LENNAME)
		&& pl->bias == pr->bias)
		return (YES);
	return (NO);
	}

/*	gen code for a binop
 */
BITS binop(p, want, set, tab)
	FAST EXPR *p;
	BITS want, set;
	UTINY *tab;
	{
	IMPORT BYTES choff;
	IMPORT TINY regtype[];
	FAST EXPR *l, *r;
	BITS lvol, lwant, rvol, rwant, savset, lty, rty;
	BOOL done, wvol;
	BYTES savoff;
	COUNT i;

#ifdef DEBUG
putfmt("/binop %o %o %o %o\n", p->op, want, set, tab);
putterm(p, 1);
#endif
	wvol = NO;
	l = p->e.o.left;
	r = p->e.o.right;
	savoff = choff;
	savset = set;
	if (!tab)
		{
		tab = pick(p, NO);
		lwant = twant(tab[0]);
		if (p->op != DCMP && p->op != DLESS)
			{
			wvol = YES;
			lwant = pref(VOLSET, lwant);
			}
		lwant = pref(want, lwant);
		if (p->op == LGETS)
			lwant =| GNOCODE;
		}
	else
		lwant = pref(want, twant(tab[0]));
	if (want & WNULL)
		lwant =| GJUNK;
	if (lwant & PTRSET)
		lwant =| WPSTK;
	rwant = twant(tab[1]) | GJUNK;
	if (want & GRVOL)
		r->got =| GVOL;
	lty = tab[0] & 017;
	rty = (0374 <= tab[1] || tab[1] <= 017) ? r->e.o.ty : tab[1] & 017;
	lvol = l->got & GVOL;
	rvol = r->got & GVOL;
	for (done = NO, i = LR; !done && i <= LTR; ++i)
		{
		l->got = lvol;
		r->got = rvol;
		switch (i)
			{
		case LR:
#ifdef DEBUG
putfmt("LR:\n");
#endif
			if (!(set = vfix(l, lwant, savset, lty, wvol)))
				done = FAIL;
			else if (set = fix(r, rwant, set, rty))
				done = SUC;
			break;
		case RL:
#ifdef DEBUG
putfmt("RL:\n");
#endif
			if ((tab[0] & 0360) == 040)
				;
			else if (!(set = fix(r, rwant, savset, rty)))
				done = FAIL;
			else if (set = vfix(l, lwant, set, lty, wvol))
				done = SUC;
			break;
		case LTR:
#ifdef DEBUG
putfmt("LTR:\n");
#endif
			if (!(set = vfix(l, lwant, savset, lty, wvol)))
				panic("LTR bop");
			else if ((set = tempify(l, lwant, set))
				&& (set = fix(r, rwant, set, rty)))
				done = SUC;
			break;
			}
		if (done != SUC)
			{
			choff = savoff;
			}
		}
	if (done != SUC)
		return (0);
	else
		return (gen(p, l, r, tab, set));
	}

/*	fix on an address descriptor
 */
BITS fix(p, want, set, ty)
	FAST EXPR *p;
	BITS want, set, ty;
	{
	IMPORT BITS regset;
	FAST EXPR *l, *r;

#ifdef DEBUG
putfmt("/fix %o %o %o %o\n", p->op, want, set, ty);
putterm(p, 1);
#endif
	l = p->e.o.left;
	r = p->e.o.right;
	switch (p->op)
		{
	case 0:
		if (!(xtor(p->e.v.idx) & regset) && (xtor(p->e.v.idx) & REGSET))
			p->got = 0;
		else
			p->got =& GVOL;
		cpyad(&p->f, &p->e.v);
		break;
	case DINDIR:
	case DADDR:
		if (set = fix(l, VOLSET|PTRSET|WTERM, set, l->e.o.ty))
			{
			p->got = l->got & (GVOL|GJUNK);
			l->f.ty = p->e.o.ty;
			cpyad(&p->f, &l->f);
			p->f.refs =+ (p->op == DINDIR) ? 1 : -1;
			}
		break;
	case LNOT:
	case LANDAND:
	case LOROR:
	case LLESS:
	case LLEQ:
	case LGREAT:
	case LGEQ:
	case LISEQ:
	case LNOTEQ:
		set = boolify(p, p, 0, 0, want, set);
		break;
	case LQUERY:
		set = boolify(p, l, p->e.o.mid, r, want, set);
		break;
	case DFNCALL:
	case DFNONLY:
		set = fncall(p, want, set);
		if (!(want & WNULL))
			set =& ~xtor(p->f.idx);
		break;
	case DELIST:
		if (fix(l, WNULL, set, l->e.o.ty)
			 && (set = fix(r, want, set, r->e.o.ty)))
			{
			p->got = r->got;
			cpyad(&p->f, &r->f);
			}
		else
			set = 0;
		break;
	case DCAST:
		if (set = fix(l, VOLSET, set, p->e.o.ty))
			{
			p->got = l->got;
			cpyad(&p->f, &l->f);
			p->f.ty = p->e.o.ty;
			}
		break;
	case DCMP:
	case DLESS:
	case LCOMP:
	case DMINUS:
	case LTIMES:
	case LDIVIDE:
	case LMODULO:
	case LPLUS:
	case LMINUS:
	case LLSHIFT:
	case LRSHIFT:
	case LAND:
	case LXOR:
	case LOR:
		set = binop(p, want, set, NULL);
		break;
	case LGETS:
	case LGTIM:
	case LGDIV:
	case LGMOD:
	case LGPLU:
	case LGMIN:
	case LGLSH:
	case LGRSH:
	case LGAND:
	case LGXOR:
	case LGOR:
		set = getsop(p, want, set, WPBC|WPHL|WMEM);
		break;
	case DGMIN:
	case DGPLU:
		set = postop(p, want, set);
		}
	return (!set ? 0 : force(p, want, set, ty));
	}

/*	coerce to the right type and storage class
 */
BITS force(p, want, set, ty)
	FAST EXPR *p;
	BITS want, set;
	FAST LEX ty;
	{
	IMPORT FTAB ftab[];
	IMPORT TEXT *deref[], *reref[], *intreg[], *ptmov[];
	IMPORT TINY bytype[], regtype[];
	FAST FTAB *q;

#ifdef DEBUG
putfmt("/force %o %o %o %o %o\n", p->op, want, set, ty, p->got);
putterm(p, 1);
#endif
	ty =& 017;
	if (!ty)
		{
		if (p->f.idx == XP0 || p->f.idx == XP1)
			chput("hl <= sp;", NULL);
		else if (p->f.idx == XSP)
			p->got = 0;
		return (set);
		}
	p->got =| want & GJUNK;
	if (want & WTERM)
		return (set);
	else if (want & WNULL)
		{
		if (p->f.idx == XSP || p->f.idx == XP0 || p->f.idx == XP1)
			chput(msp(p->f.refs ? 2 : bytype[ty]), "\n", NULL);
		if (p->got & GVOL)
			set =| xtor(p->f.idx);
		setad(p, ty, 0, 0);
		return (set);
		}
	if (!(set = ftyp(p, want, set, ty)))
		return (0);
	if (p->f.idx && (p->f.nm[0] || p->f.bias))
		if (!(set = fgen(p, BC|HL, set, reref, NO)))
			return (0);
	if (1 < p->f.refs && xtor(p->f.idx) & REGSET)
		++p->f.refs;
	for (; 1 < p->f.refs; --p->f.refs)
		if (!(set = fgen(p, BC|HL, set, deref, NO)))
			return (0);
	if (p->f.refs && (xtor(p->f.idx) & (R2|R3|R4|AUTIDX)))
		{
		if (!(set = fgen(p, BC|HL, set, intreg, NO)))
			return (0);
		}
	else if (!p->f.refs && p->f.nm[0])
		{
		if (!(set = fgen(p, BC|HL, set, ptmov, NO)))
			return (0);
		}
	if (gotten(p) & want)
		return (set);
	ty = regtype[ty];
	if (p->got & GVOL)
		set =| xtor(p->f.idx);
	q = ftab;
	for (; q->fwant; ++q)
		if (q->fwant & want && q->fset & set && ty <= q->fty
			&& (p->f.refs || (xtor(p->f.idx) & RS) || !(q->fwant & PTRSET)))
			if (!(set = fgen(p, q->fset, set, q->ftab, !(q->fwant & PTRSET))))
				return (0);
			else
				break;
	if (!q->fwant)
		return (0);
	p->f.refs = (q->fwant & PTRSET) ? 1 : 0;
	p->got =| GVOL;
	return (set);
	}

/*	force p to proper type
 */
BITS ftyp(p, want, set, ty)
	FAST EXPR *p;
	BITS want, set;
	FAST LEX ty;
	{
	IMPORT TINY equtype[], regtype[];
	FAST BITS reg;
	BITS junk;
	EXPR ex;

	if (equtype[ty] == equtype[p->f.ty & 017])
		;
	else if (iscons(p) && ty <= XUSHORT)
		p->f.ty = ty;
	else
		{
		junk = p->got & GJUNK;
		ty = regtype[ty];
		if (p->got & GVOL)
			set =| xtor(p->f.idx);
		if (ty <= XUSHORT)
			reg = set & BC;
		else
			{
			reg = set & (R0|R1);
			if (gotten(p) & (WSTACK|WPSTK) || p->f.idx == XHL)
				if (!(set = fgen(p, BC, set,
					(p->f.nm[0] || p->f.bias) ? reref : intreg, NO)))
					return (0);
			}
		if (!reg)
			return (0);
		reg =& (reg - 1) ^ reg;
		setad(&ex, ty, rtox(reg), 0);
		ex.got = GVOL;
		if (want & GNOCODE)
			cpyad(&p->f, &ex.f);
		else
			set = move(&ex, p, set);
		set =& ~reg;
		p->got =| GVOL | junk;
		}
	return (set);
	}

/*	gen code for a gets operator
 */
BITS getsop(p, want, set, pwant)
	FAST EXPR *p;
	BITS want, set, pwant;
	{
	IMPORT TEXT *ptmov[];
	FAST EXPR *l = p->e.o.left;
	FAST EXPR *r = p->e.o.right;
	BITS bwant, lgot, savset;
	EXPR ex;
	TEXT *tab;

	if (!p->e.o.ty)
		p->e.o.ty = l->e.o.ty;
#ifdef DEBUG
putfmt("/getsop %o %o %o %o\n", p->op, want, set, pwant);
putterm(p, 1);
#endif
	bwant = want & (GRVOL|WNULL);
	if (!l->op)
		{
		cpyad(&l->f, &l->e.v);
		lgot = gotten(l);
		if (p->op != LGETS || !(want & WNULL)
			|| !(lgot & WVMEM) || l->f.idx != X0)
			pwant =| lgot & VOLSET;
		else
			{
			if (!(set = fix(r, WVMEM, set, l->f.ty)))
				return (0);
			cpyad(&p->f, &r->f);
			p->got = r->got & GVOL;
			return (move(l, p, set));
			}
		}
	if (!(set = fix(l, pwant, set, l->e.o.ty)))
		return (0);
	savset = set;
	tab = pick(p, YES);
	ex.op = 0;
	ex.got = l->got & GVOL;
	cpyad(&ex.e.v, &l->f);
	p->e.o.left = &ex;
	if ((!tab && (gotten(l) & (WPHL|WPBC))) || !(set = binop(p, bwant, set, tab)))
		{
		if (!(set = tempify(l, pwant, set ? set : savset)))
			{
			p->e.o.left = l;
			return (0);
			}
		ex.got = tab ? (l->got & GVOL) : 0;
		cpyad(&ex.e.v, &l->f);
		if (!(set = binop(p, bwant, set, tab)))
			{
			p->e.o.left = l;
			return (0);
			}
		}
	p->e.o.left = l;
	if (set && !tab)
		{
		if ((p->f.idx == XSP || p->f.idx == XP0 || p->f.idx == XP1)
			&& !(set = fgen(p, (xtor(l->f.idx) & BC) ? HL : BC,
			set, ptmov, NO)))
			return (0);
		if ((l->f.idx == XSP || l->f.idx == XP0 || l->f.idx == XP1)
			&& !(set = fgen(l, (xtor(p->f.idx) & HL) ? BC : HL,
			set, ptmov, NO)))
			return (0);
		l->f.ty = l->e.o.ty;
		set = move(l, p, set);
		}
	else
		p->got =| l->got & (GVOL|GJUNK);
	return (set);
	}

/*	move r to l, set r
 */
BITS move(l, r, set)
	FAST EXPR *l, *r;
	BITS set;
	{
	FAST BITS want;
	EXPR ex, left, right;

#ifdef DEBUG
putfmt("/move %o %o %o\n", l->op, r->op, set);
putterm(l, 1);
putterm(r, 1);
#endif
	if (adequ(&l->f, &r->f))
		return (set);
	ex.op = LGETS;
	ex.e.o.ty = l->f.ty;
	ex.e.o.left = &left;
	ex.e.o.right = &right;
	left.got = l->got;
	left.op = 0;
	cpyad(&left.e.v, &l->f);
	right.got = 0;
	right.op = 0;
	cpyad(&right.e.v, &r->f);
	want = gotten(l);
	if (r->got & GVOL)
		want =| GRVOL;
	set = getsop(&ex, want, set, want);
	cpyad(&r->f, &ex.f);
	r->got = ex.got | l->got & (GVOL|GJUNK);
	return (set);
	}

/*	do a post inc/dec
 */
BITS postop(p, want, set)
	FAST EXPR *p;
	BITS want, set;
	{
	FAST EXPR *l, *r;
	EXPR ex;
	TEXT *tab;
	TINY ty;

	if (!(ty = p->e.o.ty))
		{
		p->got = 0;
		return (set);
		}
	l = p->e.o.left;
	r = p->e.o.right;
	if (!(set = fix(l, WPBC|WPHL|WMEM, set, l->e.o.ty)))
		return (0);
	cpyad(&p->f, &l->f);
	p->got = l->got;
	if (!(set = force(p, (p->f.ty & 017) <= XUSHORT ? WSTACK : WVMEM,
		set, ty)))
		return (0);
	p->got =& ~(GZSET|GNSET);
	ex.op = (p->op == DGPLU) ? LGPLU : LGMIN;
	ex.e.o.ty = p->e.o.ty;
	ex.e.o.left = l;
	ex.e.o.right = r;
	return (fix(&ex, WNULL, set, p->e.o.ty));
	}

/*	fix to a volatile
 */
BITS vfix(p, want, set, ty, wvol)
	FAST EXPR *p;
	BITS want, set, ty;
	BOOL wvol;
	{
	FAST BITS vwant, got;

	if (!(set = fix(p, want, set, ty)))
		return (0);
	else if (!wvol)
		return (set);
	vwant = VOLSET;
	got = gotten(p);
	switch (got)
		{
	case WBC:
		if (!iscons(p))
			return (set);
		vwant =& ~WBC;
		break;
	case WHL:
	case WVMEM:
	case WSTACK:
		return (set);
	case WPSTK:
		if (p->f.idx == XP0 || p->f.idx == XP1)
			return (set);
		}
	set = force(p, vwant, set, ty);
	return (force(p, want, set, ty));
	}

/*	void an expression
 */
CODE **void(qb, r)
	FAST CODE **qb;
	FAST EXPR *r;
	{
	IMPORT BITS regset;
	IMPORT BYTES choff;
	IMPORT EXPR *exlist;
	FAST BYTES chsave;

#ifdef DEBUG
putfmt("/void %o\n", r->op);
putterm(r, 1);
putexpr(r, 0);
#endif
	chsave = choff;
	if ((fix(r, WNULL, regset | (FNSET|TS), r->e.o.ty))
		!= (regset | (FNSET|TS)))
		panic("VOID");
	chput("`", NULL);
	exlist = frelst(exlist, NULL);
	*qb = buycode(GCODE, chsave, CODSIZE);
	return (&(*qb)->next);
	}
