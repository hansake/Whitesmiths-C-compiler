/*	JUMP CODE FOR EXPRESSIONS
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	the branch and jump ops
 */
GLOBAL BYTES jmpidx = {11};
GLOBAL TINY brops[] = {BJUMP, BLESS, BLEQ, BGREAT, BGEQ, BISEQ, BNOTEQ, 
	BLOW, BLOS, BHIGH, BHIS,
	GJUMP, GLESS, GLEQ, GGREAT, GGEQ, GISEQ, GNOTEQ,
	GLOW, GLOS, GHIGH, GHIS, 0};
GLOBAL TEXT *jtext[] = {
	"^j", "^jl", "^jnh", "^jh", "^jnl", "^je", "^jne",
	"^jl", "^jnh", "^jh", "^jnl",
	"^b", "^bl", "^bnh", "^bh", "^bnl", "^be", "^bne",
	"^bl", "^bnh", "^bh", "^bnl", "BAD JMP"};
GLOBAL TINY bncops[]{0, BGEQ, BGREAT, BLEQ, BLESS, BNOTEQ, BISEQ,
	BHIS, BHIGH, BLOS, BLOW,
	0, BGEQ, BGREAT, BLEQ, BLESS, BNOTEQ, BISEQ,
	BHIS, BHIGH, BLOS, BLOW};
GLOBAL TINY jncops[] = {0, GGEQ, GGREAT, GLEQ, GLESS, GNOTEQ, GISEQ,
	GHIS, GHIGH, GLOS, GLOW,
	0, GGEQ, GGREAT, GLEQ, GLESS, GNOTEQ, GISEQ,
	GHIS, GHIGH, GLOS, GLOW};

/*	fix arguments
 */
RESOURCE argfix(p, set, offset, mid)
	FAST EXPR *p;
	RESOURCE set;
	BYTES offset;
	BOOL mid;
	{
	EXPR ex, par;
	IMPORT TINY partype[];

dbprint("{/argfix %S %E", set, p);
dbfmt("  offset: %i\n", offset);
		exzap(&par);
		par.e.o.ty = partype[p->e.o.ty & TYPEMASK];
		par.e.v.idx = XXR1;
		par.e.v.refs = 1;		
		par.e.v.bias = offset;
		par.e.v.nm = "";
		if (!p->e.o.stsize || mid)
			setop(exzap(&ex), LGETS, partype[p->f.ty & TYPEMASK],
			p->f.pspace, &par, p);
		else	/* structure */
			{
			par.e.v.refs = 0;
			setop(exzap(&ex), GGETSTR, p->f.ty, p->f.pspace, &par, p);
			ex.e.o.stsize = p->e.o.stsize;
			}
		return(fix(&ex, WNULL, set, p->e.v.ty));
	}

/*	compute a boolean value
 */
RESOURCE boolify(p, q, l, r, want, set)
	EXPR *p, *q;
	FAST EXPR *l, *r;
	WANTS want;
	RESOURCE set;
	{
	IMPORT BYTES chsize;
	IMPORT EXPR exzero;
	EXPR ex, left, right;
	LABEL lbl1, lbl2;

	if ((set & FNSET) != FNSET)
		return (0);
	exzap(&ex);
	if (!l)
		{
		l = exzap(&left);
		cpyad(&left.e.v, &exzero.e.v);
		left.e.v.bias = 1;
		}
	if (!r)
		{
		r = exzap(&right);
		cpyad(&right.e.v, &exzero.e.v);
		}
	lbl1 = jf(q, 0, 0, set);
	setbool(p);
	ex.got = 0;
	cpyad(&ex.f, &p->f);
	if (!ex.f.ty)
		ex.f.ty = YSHORT;
	if (!fix(l, WIMM|WREREF|VOLSET|PTRSET, set, l->e.o.ty) || !move(&ex, l, set))
		panic("? X");
	lbl2 = crs();
	chput("^b ", lbln(lbl2), ";", NULL);
	chsize += JMPSIZE;
	chput(lbln(lbl1), ":;", NULL);
	ex.got = 0;
	cpyad(&ex.f, &p->f);
	if (!ex.f.ty)
		ex.f.ty = YSHORT;
	if (!fix(r, WIMM|WREREF|VOLSET|PTRSET, set, r->e.o.ty) || !move(&ex, r, set))
		panic(": X");
	chput(lbln(lbl2), ":;", NULL);
	return (set);
	}

/*	setup compare operator
 */
LEX cmpset(p, l, r, tok)
	FAST EXPR *p, *l, *r;
	LEX tok;
	{
	IMPORT TINY cmpops[], swpops[], unsops[], unzops[];
	TYPE maxty;
	BOOL ok;
	LONG lo;

	setop(p, DCMP, 0, 0, l, r);
	if (iscons(l) && iscons(r))
		{
		lo = l->e.v.bias - r->e.v.bias;
		switch (tok)
			{
		case GLESS:
			ok = (lo < 0);
			break;
		case GLEQ:
			ok = (lo <= 0);
			break;
		case GGREAT:
			ok = (lo > 0);
			break;
		case GGEQ:
			ok = (lo >= 0);
			break;
		case GISEQ:
			ok = (lo == 0);
			break;
		case GNOTEQ:
			ok = (lo != 0);
			}
		tok = ok ? GJUMP : GNOP;
		return (tok);
		}
	if (iscons(l))
		{
		p->e.o.left = r;
		p->e.o.right = l;
		tok = swpops[scnstr(cmpops, tok)];
		}
	r = p->e.o.right;
	maxty = p->e.o.left->e.o.ty & TYPEMASK;
	if (maxty < (r->e.o.ty & TYPEMASK))
		maxty = r->e.o.ty & TYPEMASK;
	if (maxty == YUCHAR || maxty == YUSHORT || maxty == YULONG)
			tok = ((iscons(r) && r->e.v.bias == 0 &&
				maxty != YDOUBLE && maxty != YFLOAT)
				? unzops : unsops)[scnstr(cmpops, tok)];
	if (tok == GISEQ || tok == GNOTEQ)
		p->op = DCMP;
	else
		p->op = DLESS;
	return (tok & BYTMASK);
	}

/*	generate calls for builtin
 */
BYTES dobuiltin(p, size)
	FAST EXPR *p;
	BYTES size;
	{
	IMPORT BYTES chsize;
	INTERN TEXT *bname[] = {"", ""};
	INTERN BYTES bsize[] = {1, 1};
	FAST EXPR *l = p->e.o.left;
	FAST BYTES n = l->f.bias;

	if (!(gotten(l) & WIMM) || sizeof (bname) / sizeof (bname[0]) <= n
		|| size)
		perror("bad builtin", NULL);
	else
		{
		chput(bname[n], ";", NULL);
		chsize += bsize[n];
		}
	return (size);
	}

/*	setup args to function
 */
BYTES fnargs(p, pset)
	FAST EXPR *p;
	ARGINT *pset;
	{
	IMPORT LONG autoff;
	IMPORT TINY bytype[];
	FAST COUNT size, nsize;
	LONG savaut;

dbprint("{/fnargs %S %E", *pset, p);
	if (p->op == DALIST)
		{
		size = fnargs(p->e.o.right, pset);
		p = p->e.o.left;
		}
	else
		size = 0;
	if (!p->e.o.stsize)
		nsize = bytype[p->e.o.ty & TYPEMASK];
	else
		nsize = p->e.o.stsize;
	size += nsize;
	msp(-nsize);	
	savaut = autoff;
	*pset = argfix(p, *pset, FPOFFSET + (BYTES)autoff, NO);
	autoff = savaut;
	if (!*pset)
		panic("ARG");
	return (SUCC(size));
	}

/*	call a function
 */
RESOURCE fncall(p, want, set)
	FAST EXPR *p;
	WANTS want;
	RESOURCE set;
	{
	IMPORT BYTES chsize;
	IMPORT LONG autoff;
	IMPORT TINY witype[];
	FAST BYTES size;
	FAST EXPR *l;
	BYTES msize;

dbprint("{/fncall %W %S %A", want, set, p);
	l = p->e.o.left;
	if ((set & FNSET) != FNSET)
		return (FAIL);
	if (p->op == DFNCALL)
		size = fnargs(p->e.o.right, &set);
	else
		size = 0;
	if (p->e.o.mid)				/* FOR STRUCTURE RETURN */
		{
		msize = (BYTES)witype[YUSHORT];
		size += msize;
		msp(-msize);
		set = argfix(p->e.o.mid, set, FPOFFSET + (BYTES)autoff , YES);
		}
	if (!fix(l, WMEM|WVMEM|WPMEM, set, l->e.o.ty))
		panic("FNCALL");
dbprint("/fncall--function %S %A",  set, p);
	if (l->f.pspace & SBUI)
		size = dobuiltin(p, size);
	else if (l->f.pspace & SPORT)
		{
		/* call ssp routine */
		}
	else
		{
#ifdef DEBUG
		chput("/size of args ", nn(size, -10), ";", NULL);
#endif
		if (!gotadr(l))
			genpx2(l);
		chput("^la ", nn((COUNT)autoff - 4, -10),
			",#xr1;", NULL);
		chput("^b ", NULL);
		genad(l, 0, NO, TWORD, 1);
		chput(";", NULL);
		chsize += 8;
		}
	size += setfn(p, want);
	msp(size);	
dbprint("from bottom of fncall %E", p);
	return (SUCC(set & ~voltor(p)));
	}

/*	jump conditional
 */
CODE **jc(qb, tok, p, lab, l, r, set)
	FAST CODE **qb;
	LEX tok;
	HEADER *p;
	LABEL lab;
	FAST EXPR *l, *r;
	RESOURCE set;
	{
	IMPORT BYTES choff, chsize, jmpidx;
	IMPORT TEXT *jtext[];
	IMPORT TINY brops[];
	TYPE ty;
	BYTES savoff, savsize;
	EXPR ex;

	tok = cmpset(exzap(&ex), l, r, tok);
	l = ex.e.o.left;
	r = ex.e.o.right;
	if (tok == GNOP)
		{
		if (qb)
			{
			*qb = buycode(GNOP, NULL, 0);
			qb = &(*qb)->next;
			}
		}
	else if (tok == GJUMP)
		if (qb)
			{
			*qb = buycode(GNOP, NULL, 0);
			(*qb)->next = buycode(GJUMP, p, JMPSIZE);
			++p->nrefs;
			qb = NULL;
			}
		else
			{
			chput("^b ", lbln(lab), ";", NULL);
			chsize += BRSIZE;
			}
	else
		{
		savoff = choff;
		savsize = chsize;
		ex.got = 0;
		if (iscons(r) && r->e.v.bias == 0 && l->op != DCAST)
			{
			ty = l->e.o.ty, l->e.o.ty = 0;
			if (!fix(l, VOLSET|PTRSET|WIMM, set, 0))
				panic("X ? 0");
			if ((l->got & GCCSET) ||
				((l->got & GCCZERO) && ((ex.op == DCMP) || (ex.op == DLESS))))
				ex.got = l->got;
			else
				{
				choff = savoff;
				chsize = savsize;
				}
			l->e.o.ty = ty;
			}
		if (!ex.got && !fix(&ex, WNULL, set, 0))
			panic("X ? Y");
		if (!(ex.got & GCCSET|GCCZERO))
			panic("NO CC");
		if (qb)
			{
			chput("`", NULL);
			*qb = buycode(GCODE, NULL, chsize - savsize);
			(*qb)->c.choff = savoff;
			(*qb)->next = buycode(tok, p, JCSIZE);
			++p->nrefs;
			qb = &(*qb)->next->next;
			}
		else
			{
			chput("^", jtext[scnstr(brops + jmpidx, tok)],
				" ", lbln(lab), ";", NULL);
			chsize += BRSIZE;
			}
		}
	return (qb);
	}

/*	external jc call
 */
CODE **jcond(qb, tok, p, l, r)
	CODE **qb;
	LEX tok;
	HEADER *p;
	EXPR *l, *r;
	{
	IMPORT RESOURCE regset;
	IMPORT EXPR *exlist;

	qb = jc(qb, tok, p, 0, l, r, regset | FNSET);
	exlist = (EXPR *)frelst(exlist, NULL);
	return (SUCC(qb));
	}

/*	jump if false
 */
LABEL jf(p, fb, tb, set)
	FAST EXPR *p;
	FAST LABEL fb, tb;
	RESOURCE set;
	{
	IMPORT EXPR exzero;
	IMPORT TINY flpops[], lexops[];

	if (!fb)
		fb = crs();
	switch (p->op)
		{
	case LANDAND:
		fb = jf(p->e.o.left, fb, 0, set);
		return (jf(p->e.o.right, fb, tb, set));
	case LOROR:
		tb = jt(p->e.o.left, 0, tb, set);
		return (jf(p->e.o.right, fb, tb, set));
	case LNOT:
		return (jt(p->e.o.left, tb, fb, set));
	case LLESS:
	case LLEQ:
	case LGREAT:
	case LGEQ:
	case LISEQ:
	case LNOTEQ:
		jc(NULL, flpops[scnstr(lexops, p->op)] & BYTMASK, NULL, fb,
			p->e.o.left, p->e.o.right, set);
		break;
	default:
		jc(NULL, GISEQ, NULL, fb, p, &exzero, set);
		}
	if (tb)
		chput(lbln(tb), ":;", NULL);
	return (fb);
	}

/*	jump if true
 */
LABEL jt(p, fb, tb, set)
	FAST EXPR *p;
	FAST LABEL fb, tb;
	RESOURCE set;
	{
	IMPORT EXPR exzero;
	IMPORT TINY cmpops[], lexops[];

	if (!tb)
		tb = crs();
	switch (p->op)
		{
	case LANDAND:
		fb = jf(p->e.o.left, fb, 0, set);
		return (jt(p->e.o.right, fb, tb, set));
	case LOROR:
		tb = jt(p->e.o.left, 0, tb, set);
		return (jt(p->e.o.right, fb, tb, set));
	case LNOT:
		return (jf(p->e.o.left, tb, fb, set));
	case LLESS:
	case LLEQ:
	case LGREAT:
	case LGEQ:
	case LISEQ:
	case LNOTEQ:
		jc(NULL, cmpops[scnstr(lexops, p->op)], NULL, tb,
			p->e.o.left, p->e.o.right, set);
		break;
	default:
		jc(NULL, GNOTEQ, NULL, tb, p, &exzero, set);
		}
	if (fb)
		chput(lbln(fb), ":;", NULL);
	return (tb);
	}

/*	set address for boolean expression
 */
VOID setbool(p)
	EXPR *p;
	{
	IMPORT LONG autbuy();
	IMPORT TINY regtype[];

	if (p->e.o.stsize)
		{
		setad(p, p->e.v.ty, XXR1, 1);
		p->f.bias = autbuy(p->e.o.stsize);
		}
	else
		{
		setad(p, p->e.o.ty, XAC0, 0);
		}
	p->got = GVOL;
	}

/*	set function return register
 */
BYTES setfn(p, want)
	FAST EXPR *p;
	WANTS want;
	{
	IMPORT TINY regtype[];
	FAST BYTES size;

dbprint("/setfn %W %A", want, p);
	if (!p->e.o.mid)
		{
		setad(p, p->e.v.ty, XAC0, 0);
		p->got = GVOL;
		size = 0;
		}
	else
		{
		setad(p, p->e.v.ty, XAC0, 0);
		p->got = GVOL;
		size = (want & WNULL) ? 2 : 0;
		}
	return (size);
	}

