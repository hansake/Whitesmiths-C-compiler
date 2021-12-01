/*	GENERATE CODE FOR EXPRESSIONS 
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
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
#define RTL		3

#define NOTYET	0
#define FAILED	1
#define COOKED	2

/*	automatic offsets and register stuff
 */
GLOBAL LONG autmin = {0};
GLOBAL LONG autoff = {0};
GLOBAL RESOURCE regset = {0};

/*	the character pool offset
 */
GLOBAL BYTES choff = {0};
GLOBAL BYTES chsize = {0};

/*	constant zero
 */
GLOBAL EXPR exzero = {0};

/*	are the addresses exactly the same
 */
BOOL adequ(pl, pr)
	FAST ADDR *pl, *pr;
	{
	IMPORT TINY equtype[];

	if (pl->idx != pr->idx || pl->refs != pr->refs || pl->space != pr->space)
		;
	else if (equtype[pl->ty & TYPEMASK] != equtype[pr->ty & TYPEMASK])
		;
	else if (cmpstr(pl->nm , pr->nm) && pl->bias == pr->bias)
		return (YES);
	return (NO);
	}

/*	buy automatic space
 */
LONG autbuy(size)
	BYTES size;
	{
	IMPORT LONG autmin, autoff;

	autoff -= size;
	if (autoff < autmin)
		autmin = autoff;
	return (autoff + AUTBIAS);
	}

/*	gen code for a binop
 */
RESOURCE binop(p, want, set, tab)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	TAB *tab;
	{
	IMPORT BYTES choff, chsize;
	IMPORT LONG autmin, autoff;
	FAST EXPR *l, *r;
	BYTES savoff, savsize;
	COUNT done, i;
	LONG savmin, savaut;
	RESOURCE savset;
	TYPE lty, rty;
	WANTS lwant, rwant;

dbprint("{/binop %O %W %S %T %E", p->op, want, set, tab, p);
	l = p->e.o.left;
	r = p->e.o.right;
	savoff = choff;
	savsize = chsize;
	savset = set;
	savmin = autmin;
	savaut = autoff;
	if (!tab)
		{
		tab = pick(p, NO, want);
		if (!tab)
			panic("MISSING ENTRY");
		lwant = twant(tab->lattr);
		if (!(tab->res & (CCONLY|STOS)))
			lwant &= VOLSET;
		lwant = pref(want, lwant);
		if (p->op == LGETS || p->op == GGETSTR)
			lwant |= GNOCODE;
		}
	else
		lwant = pref(want, twant(tab->lattr));
	rwant = twant(tab->rattr) | GJUNK | (want & GRVOL);
	if (tab->res & RVOL)
		rwant &= VOLSET|GJUNK|GRVOL;
	if (want & WNULL)
		lwant |= GJUNK;
	lty = tab->lattr & TYPEMASK;
	i = tab->rattr & BYTMASK;
	rty = ((i & ATTRMASK) <= CONOP) ? r->e.o.ty : i & TYPEMASK;
	for (done = NOTYET, i = LR; done == NOTYET && i <= RTL; ++i)
		{
dbprint("/binop-   r%W l%W %S %T\n", rwant, lwant, set, tab);
		switch (i)
			{
		case LR:
dbprint("/binop-LR:\n");
			if (!(set = fix(l, lwant, savset, lty)))
				done = FAILED;
			else if (set = fix(r, rwant, set, rty))
				done = COOKED;
			break;
		case RL:
dbprint("/binop-RL:\n");
			if (!(set = fix(r, rwant, savset, rty)))
				done = FAILED;
			else if (set = fix(l, lwant, set, lty))
				done = COOKED;
			break;
		case LTR:
dbprint("/binop-LTR:\n");
			if (!(set = fix(l, lwant, savset, lty)))
				panic("LTR bop");
			else if ((set = tempify(l, twant(tab->lattr), set))
				&& (set = fix(r, rwant, set, rty))
				&& (set = force(l, lwant, set, lty)))
				done = COOKED;
			break;
		case RTL:
dbprint("/binop-RTL:\n");
			if (!(set = fix(r, rwant, savset, rty)))
				panic("RTL bop");
			else if ((set = tempify(r, rwant, set))
				&& (set = fix(l, lwant, set, lty))
				&& (set = force(r, rwant, set, rty)))
				done = COOKED;
			}
		if (done != COOKED)
			{
			choff = savoff;
			chsize = savsize;
			autmin = savmin;
			autoff = savaut;
			}
		}
	if (want & GRVOL)
		r->got |= GVOL;
	if (done != COOKED)
		return (FAIL);
	else
		{
		return (SUCC(gen(p, tab, set)));
		}
	}

/*	fix on an address descriptor
 */
RESOURCE fix(p, want, set, ty)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	TYPE ty;
	{
	FAST EXPR *l, *r;
	IMPORT TINY witype[];

dbprint("{/fix %O %W %Y %S %E", p->op, want, ty, set, p);
	l = p->e.o.left;
	r = p->e.o.right;
	switch (p->op)
		{
	case 0:
		p->got = (want & GRVOL) ? GVOL : 0;
		want &= ~GRVOL;
		cpyad(&p->f, &p->e.v);
		if (p->f.idx == XVXR1)
			{
			p->f.idx = XXR1;
			p->got |= GVOL;
			}
		break;
	case DINDIR:
		if (set = fix(l, WIMM|PTRSET|VOLSET|WREREF, set, l->e.o.ty))
			{
			p->got = l->got & GVOL;
			cpyad(&p->f, &l->f);
			++p->f.refs;
			p->f.ty = p->e.o.ty;
			p->f.pspace = p->e.o.pspace;
			p->f.space = l->f.pspace;
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
		set = boolify(p, p, NULL, NULL, want, set);
		break;
	case LQUERY:
		set = boolify(p, l, p->e.o.mid, r, want, set);
		break;
	case DFNCALL:
	case DFNONLY:
		set = fncall(p, want, set);
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
		if (set = fix(l, ((want | VOLSET) & ~(WVXR2|WXR2)),
	 		set, p->e.o.ty))
			{
			p->got = l->got;
			cpyad(&p->f, &l->f);
			p->f.ty = p->e.o.ty;
			}
		break;
	case LPLUS:
		p->f.ty = p->e.o.ty;
		if (!l->op && !r->op && canadd(p, &l->e.v, &r->e.v, YES))
			{
			p->got = 0;
			break;
			}
		if (!r->op && !r->e.v.refs && r->e.v.idx && r->e.v.nm[0])
			fixplus(p);  /* not needed? */
		set = binop(p, want, set, NULL);
		break;
	case LMINUS:
		set = binop(p, want, set, NULL);
		break;
	case DCMP:
	case DLESS:
	case LCOMP:
	case DMINUS:
	case LTIMES:
	case LDIVIDE:
	case LMODULO:
	case LLSHIFT:
	case LRSHIFT:
	case LAND:
	case LXOR:
	case LOR:
	case GGETSTR:
		set = binop(p, (want & ~(WXR2|WVXR2)), set, NULL);
		break;
	case LGETS:
		set = getsop(p, want, set, PTRSET);
		break;
	case LGPLU:
	case LGMIN:
	case LGTIM:
	case LGDIV:
	case LGMOD:
	case LGLSH:
	case LGRSH:
	case LGAND:
	case LGXOR:
	case LGOR:
		set = getsop(p, (want & ~(WXR2|WVXR2)), set, (PTRSET & ~WXR2));
		break;
	case DGMIN:
	case DGPLU:
		set = postop(p, want , set);
		break;
	default:
		panic("FIXOP");
		}
	return ((!set) ? FAIL : SUCC(force(p, want, set, ty)));
	}

/*	string out + reg + const
 */
VOID fixplus(p)
	FAST EXPR *p;
	{
	FAST EXPR *l, *lr;

	l = mexpr(0);
	lr = mexpr(0);
	setop(l, LPLUS, p->e.o.ty, p->e.o.pspace, p->e.o.left, lr);
	p->e.o.left = l;
	lr->op = 0;
	cpyad(&lr->e.v, &p->e.o.right->e.v);
	lr->e.v.nm = "";
	lr->e.v.bias = 0;
	p->e.o.right->e.v.idx = 0;
	}

/*	coerce to the right type and storage class
 */
RESOURCE force(p, want, set, ty)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	FAST LEX ty;
	{
	IMPORT TINY equtype[];
	BOOL tyok;
	EXPR ex;
	RESOURCE takeset;
	TYPE oty;
	WANTS got;

dbprint("{/force %O %W %Y %S %E", p->op, want, ty, set, p);
	while (set)
		{
		if (want & WNULL)
			{
			return (SUCC(set | voltor(p)));
			}
		got = gotten(p);

		tyok = !ty ||
			(equtype[ty & TYPEMASK] == equtype[p->f.ty & TYPEMASK]);
dbprint("/force- = %W %S tyok: %B\n", got, set, tyok);
		if (tyok && got & want)
			break;
		else if (!(want & ~GJUNK))
			return (FAIL);
		else if (tyok && (got == WTERM))
			{
			oty = p->f.ty, p->f.ty = YUSHORT;
			if (--p->f.refs < 0)
				panic("FORCE");
			if (!(set = force(p, WVXR2, set, p->f.ty)))
				return (FAIL);
			p->f.ty = oty;
			++p->f.refs;
			p->got &= ~GCCSET;
			}
		else if (got & WIMM)
			if (want & WIMM)
				p->f.ty = ty;
			else
				mklit(p, ty);
		else if ((want & got) &&
			equtype[ty & TYPEMASK] < equtype[p->f.ty & TYPEMASK] &&
			equtype[p->f.ty & TYPEMASK] < YFLOAT &&
			(got & (WMEM|WVMEM|WAC0|WVAC0)))
			{
			if (got & (WMEM|WVMEM))
				p->f.bias += 
					witype[p->f.ty & TYPEMASK] -  witype[ty & TYPEMASK];
			p->f.ty = ty;
			}
		else if ((got & WREREF) && !tyok)
			set = force(p, want & (~WREREF), set, p->f.ty);
		else
			{
			set |= voltor(p);
			exzap(&ex);
			if (foreg(&ex, want, set, ty))
				;
			else
				return (FAIL);
			if (want & (GNOCODE|WNULL))
				cpyad(&p->f, &ex.f);
			set = move(&ex, p, set);
			p->got |= GVOL;
			break;
			}
		}
	p->got |= want & GJUNK;
	takeset = voltor(p);
dbprint("/force- return");
dbprint((takeset) ? "(%S & ~%S)\n" : "(%S)\n", set, takeset);
	return (SUCC(set & ~takeset));
	}

/*	try to force into a good place
 */
BOOL foreg(p, want, set, ty)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	FAST TYPE ty;
	{
	IMPORT LONG autbuy();
	IMPORT TINY witype[];
	FAST TINY ridx;
	TYPE pty;
	TINY refs;

dbprint("{/foreg %O %W %Y %S %E", p->op, want, ty, set, p);
	ridx = 0;
	refs = 0;
	ty &= TYPEMASK;
	pty = p->f.ty & TYPEMASK;
	if ((ty == YSHORT || ty == YUSHORT) &&
		(want & WVXR2) && (set & XR2))
		ridx = XR2; 
	else if ((want & WVAC0) && (set & AC0))
		ridx = AC0;
	if (!ridx)
		if (want & WVMEM &&
			((ty <= YULONG && pty <= YULONG) || ty == pty))
			{
			setad(p, ty, XXR1, 1);
			p->f.ty = ty;
			p->f.bias = autbuy(witype[ty & TYPEMASK]);
			}
		else
			return (SUCC(NO));
	else
		{
		setad(p, ty, ridx = rtox(ridx), refs);
		p->f.ty = ty;
		}
dbprint("/foreg-return %Y %E", ty, p);
	return (SUCC(YES));
	}

/*	gen code for a gets operator
 */
RESOURCE getsop(p, want, set, pwant)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	WANTS pwant;
	{
	FAST EXPR *l = p->e.o.left;
	EXPR ex;
	RESOURCE savset;
	TAB *tab;
	WANTS bwant;

	if (!p->e.o.ty)
		{
		p->e.o.ty = l->e.o.ty;
		p->e.o.pspace = l->e.o.pspace;
		}
dbprint("{/getsop %O %W p%W %S %E", p->op, want, pwant, set, p);
	bwant = want & GRVOL;
	if (!(set = fix(l, pwant, set, l->e.o.ty)))
		return (FAIL);
	exzap(&ex);
	savset = set;
	if (tab = pick(p, YES, want))
		bwant |= want & WNULL;
	ex.op = 0;
	cpyad(&ex.e.v, &l->f);
	p->e.o.left = &ex;
dbprint("/getsop-to binop %O %W p%W %S %T %E", p->op, want, pwant, set, tab, p);
	if (set = binop(p, bwant, set, tab))
		;
	else if (!(set = tempify(l, pwant, savset)))
		;
	else
		{
		cpyad(&ex.e.v, &l->f);
		cpyad(&ex.f, &l->f);
		set = binop(p, bwant, set, tab);
		}
	p->e.o.left = l;
	if (!set)
		;
	else if (!tab)
		set = move(l, p, set);
	else 
		p->got |= l->got & GVOL;
dbprint("/getsop- return(%S) %E", set, p);
	return (SUCC(set));
	}

/*	move r to l, set r
 */
RESOURCE move(l, r, set)
	FAST EXPR *l, *r;
	FAST RESOURCE set;
	{
	WANTS want;
	EXPR ex, left, right;

	if (gotten(r) & (WREREF|WXR2|WVXR2))
		return(reref(l, r, set));
	exzap(&left);
	exzap(&right);
	setop(exzap(&ex), LGETS, l->f.ty, l->f.pspace, &left, &right);
	cpyad(&left.e.v, &l->f);
	cpyad(&right.e.v, &r->f);
dbprint("{/move     %S %E", set, &ex);
	want = gotten(l);
dbprint("/move gotten= %W\n", want);
	set = getsop(&ex, want | ((r->got & GVOL) ? GRVOL : 0),
		set, want | ((l->got & GVOL) ? GRVOL : 0));
	cpyad(&r->f, &ex.f);
	r->got = ex.got | l->got & GVOL;
	return (SUCC(set));
	}

/*	do a post inc/dec
 */
RESOURCE postop(p, want, set)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	{
	FAST EXPR *l, *r;
	WANTS savset;
	EXPR ex, left;

dbprint("{/postop %O %W %S %E", p->op, want, set, p);
	l = p->e.o.left;
	r = p->e.o.right;
	if (!(set = fix(l, PTRSET, set, l->e.o.ty))
		|| !(set = fix(r, WIMM, set, r->e.o.ty)))
		return (FAIL);
	cpyad(&p->f, &l->f);
	p->got = 0;
	savset = set;
	if (set = force(p, VOLSET, set, p->f.ty))
		;
	else if (!(set = tempify(l, WVMEM, savset)))
		return (FAIL);
	else
		{
		cpyad(&p->f, &l->f);
		p->got = 0;
		if (!(set = force(p, VOLSET, set, p->f.ty)))
			return (FAIL);
		}
	exzap(&left);
	exzap(&ex);
	p->got &= ~GCCSET;
	setop(&ex, p->op, l->f.ty, l->f.pspace, &left, r);
	left.op = 0;
	cpyad(&left.e.v, &l->f);
	savset = set;
	if (set = getsop(&ex, WNULL, set, PTRSET))
		;
	else if (!(set = tempify(p, WVMEM, savset)))
		;
	else
		set = getsop(&ex, WNULL, set, PTRSET);
	return (SUCC(force(l, WNULL, set | voltor(&ex), l->f.ty)));
	}

/*	liberate a register, if possible
 */
RESOURCE tempify(p, want, set)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	{
	IMPORT LONG autbuy();
	IMPORT TINY witype[], bytype[];
	EXPR ex;
	TYPE ty;

dbprint("{/tempify %O %W %S %E", p->op, want, set, p);
	if (!voltor(p))
		return (FAIL);
	else if (want & (WVMEM))
		{
		exzap(&ex);
		setad(&ex, p->f.ty, XXR1, 1);
		ex.f.ty = p->f.ty;
		ex.f.bias = autbuy(witype[p->f.ty & TYPEMASK]);
		ex.got = GVOL;
dbprint("{/tempify MAD? %E", &ex);
		return (SUCC(move(&ex, p, set)));
		}
	else if ((want & WPMEM) && 0 < p->f.refs)
		{
		ty = p->f.ty, p->f.ty = YUSHORT;
		--p->f.refs;
		setad(&ex, YUSHORT, XXR1, 1);
		ex.f.bias = autbuy(bytype[YUSHORT]);
		ex.got = GVOL;
		set = move(&ex, p, set);
		p->f.ty = ty;
		++p->f.refs;
		return (SUCC(set));
		}
	else
		return (FAIL);
	}

/*	void an expression
 */
CODE **voidex(qb, r)
	FAST CODE **qb;
	FAST EXPR *r;
	{
	IMPORT BYTES choff, chsize;
	IMPORT EXPR *exlist;
	IMPORT RESOURCE regset;
	FAST BYTES chsave;

dbprint("/voidex %A", r);
	chsave = choff;
	chsize = 0;
	if ((fix(r, WNULL, regset | FNSET, r->e.o.ty))
		!= (regset | FNSET))
		panic("VOID");
	chput("`", NULL);
	exlist = (EXPR *)frelst(exlist, NULL);
	*qb = buycode(GCODE, NULL, chsize);
	(*qb)->c.choff = chsave;
	return (&(*qb)->next);
	}
