/*	GEN CODE FOR PASS2
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	register table for generation, indexed by xtab
 */
LOCAL TEXT *xname[] = {"@ac0", "#xr1", "#xr2", "#f0", "??"};

/*	register masks to index numbers
 */
GLOBAL RESOURCE rtab[] = {AC0, XR1, XR2, F0, 0};
GLOBAL TINY xtab[] = {XAC0, XXR1, XXR2, XF0, 0};

/*	add at compile time if possible
 */
BOOL canadd(p, la, ra)
	FAST EXPR *p;
	FAST ADDR *la, *ra;
	{
	TINY tidx;

dbprint("{/canadd %A", p);
	if (!la->refs && !ra->refs && !la->nm[0] && !ra->nm[0] &&
		(p->f.ty & TYPEMASK) <= YUSHORT &&
		0 <= (tidx = canidx(la->idx, ra->idx)) &&
		0 <= la->bias + ra->bias &&
		la->bias + ra->bias < 248)
		{
		p->f.nm = (la->nm[0]) ? la->nm : ra->nm;
		p->f.bias = la->bias + ra->bias;
		p->f.refs = 0;
		p->f.idx = tidx;
		p->f.space = (la->space) ? la->space : ra->space;
		p->f.pspace = (la->pspace) ? la->pspace : ra->pspace;
		return (SUCC(YES));
		}
	return (FAIL);
	}

/*	test for valid index combination
 */
TINY canidx(lidx, ridx)
	FAST TINY lidx, ridx;
	{
	if (!lidx)
		lidx = ridx;
	else if (ridx)
		return(-1);
	if (lidx == XXR1 || lidx == XXR2)
		return(lidx);
	else
		return(-1);
	}

/*	generate code sequences
 */
RESOURCE gen(p, tab, set)
	FAST EXPR *p;
	TAB *tab;
	RESOURCE set;
	{
	IMPORT BYTES chsize;
	IMPORT TAB esctab;
	IMPORT TAB *(*gfun[])();
	IMPORT TINY equtype[], regtype[];
	FAST EXPR *l, *r;
	RES code;
	TYPE ty;
	RESOURCE jset, restore;

	l = p->e.o.left;
	r = p->e.o.right;
	code = tab->res;
dbprint("{/gen      %S %T %A", set, tab, p);
	cpyad(&p->f, &l->f);			/* set up p */
	p->got = l->got & GVOL;
	ty = regtype[p->e.o.ty & TYPEMASK];
	if (equtype[p->f.ty & TYPEMASK] == equtype[ty])
		p->f.ty = ty;
	if (code & CCONLY || l->got & GJUNK)
		{
		l->got |= GJUNK;
		p->got = 0;
		}
	jset = 0;
	if (l->got & GJUNK)
		jset |= voltor(l);
	if (r->got & GJUNK)
		jset |= voltor(r);
	set |= jset;
	if (p->op == LGETS && adequ(&l->f, &r->f))
		return (SUCC(set));
	if (p->op == LPLUS && canadd(p, &l->f, &r->f, NO))
		{
		p->got |= r->got & GVOL;
		return (SUCC(set & ~voltor(p)));
		}
	if (code & CCSET)
		p->got |= GCCSET;
	if (code & CCZERO)
		p->got |= GCCZERO;
	if (code & ESCR)
		{
		esctab.lattr = tab->lattr;
		esctab.rattr = tab->rattr;
		if (!(tab = (*gfun[*tab->str])(p, set & ~jset, tab, &set)))
			return (FAIL);
		}
	restore = genfix(p, set & ~jset, tab->lattr & ATTRMASK, 
		tab->rattr & ATTRMASK);
	if (!genac(p, tab))
		return (FAIL);
	if (restore & XR1)
		{
		chput("^l @fp+1,#xr1;", NULL);
		chsize += 4;
		}
	if (restore & XR2)
		{
		chput("^l @xr2+1,#xr2;", NULL);
		chsize += 4;
		}
	return (SUCC(set));
	}

/*	generate the code part
 */
BOOL genac(p, tab)
	FAST EXPR *p;
	TAB *tab;
	{
	IMPORT BYTES chsize;
	FAST TEXT *q;
	BOOL any, compl, label;
	COUNT crlab, putlen, type, typlen, up;
	EXPR *l	= p->e.o.left;
	EXPR *r	= p->e.o.right;

dbprint("/genac ");
	crlab = 0;
	type = TWORD;
	putlen = TPUTLEN;
	typlen = 2;
	up = 0;
	compl = NO;
	for (any = NO, q = tab->str; *q; any = YES, ++q)
		{
		label = NO;
		switch (*q)
			{
		case 'L':
			genad(l, up, compl, putlen | type, typlen);
			putlen = TPUTLEN;
			up = 0;
			compl = NO;
			break;
		case 'R':
		case 'A':
			genad(r, up, compl,
				((*q == 'R') ? TNOPUTL : putlen) | type, typlen);
			putlen = TPUTLEN;
			up = 0;
			compl = NO;
			chsize += 4;
			break;
		case 'C':
			compl = YES;
			break;
		case 'H':
			putlen = TNOPUTL;
			typlen = 1;
		case 'U':
			up++;
			break;
		case 'N':
			putlen = TNOPUTL;
			break;
		case 'K':
			q = genkall(q);
			--q;
			break;
		case ';':
			chwrite(q, 1);
			chsize += 2;
			break;
		case ':':
			chwrite(q, 1);
			label = YES;
			break;
		case 'T':
			if (!crlab)
				crlab = crs();
			chput(lbln(crlab), NULL);
			break;
		case 'B':
			type = TBYTE;
			typlen = 1;
			break;
		case 'D':
			type = TDOUBLE;
			typlen = 8;
			break;
		case 'F':
			type = TFLOAT;
			typlen = 4;
			break;
		case 'Q':
			type = TQUAD;
			typlen = 8;
			break;
		case 'S':
			type = TLONG;
			typlen = 4;
			break;
		case 'W':
			type = TWORD;
			typlen = 2;
			break;
		default:
			chwrite(q, 1);
			}
		}
	if (any && !label)
		{
		chput(";", NULL);
		}
dbprint("/genac-end ");
	return (YES);
	}

/*	generate an address mode
 */
VOID genad(p, up, compl, type, typlen)
	FAST EXPR *p;
	COUNT up;
	BOOL compl;
	COUNT type, typlen;
	{
	IMPORT BYTES chsize;
	IMPORT TINY witype[];
	FAST TINY idx, refs;
	LONG off, realoff;
	TEXT fc, *on, *xn, *nm;

	idx = p->f.idx;
	refs = p->f.refs;
	xn = xnm(idx);
	nm = p->f.nm;
	if (idx && idx != XXR1 && idx != XXR2 && idx != XF0 && idx != XVXR1)
		{ /* pseudo register */
		refs += 1;
		nm = xn;
		idx = 0;
		xn = NULL;
		/* align to rightmost byte */
		up += ((type == TFLOAT) ? 12 : 8) - witype[p->f.ty & TYPEMASK];
		}
	fc = nm[0];
	if (refs)
		off = p->f.bias + up;
	else
		{
		off = p->f.bias;
		for (; up; up--)
			off >>= 8;
		}
	if (compl)
		off = ~off;
	if ((type & TYPEMASK) == TBYTE)
		on = nn((COUNT)off & BYTMASK, 10);
	else
		on = nn((COUNT)off, -10);
	if (!fc && !off && idx)
		{
		if (refs == 0)
			chput(xn, NULL);
		else
			{
			chput(nn(typlen - 1, -10), "(", NULL);
			chput((type & TPUTLEN) ? nn(typlen, -10) : "", NULL);
			chput(",", xn, ")", NULL);
			chsize += 1;
			}
		}
	else if (!fc && !idx)
		{
		chput(on, NULL);
		chsize += 2;
		}
	else
		{
		realoff = off + typlen - 1;
		if (fc)
			{
			chput(nm, NULL);
			if (0 < realoff)
				chput("+", NULL);
			chsize += 2;
			}
		if (realoff)
			chput(nn((COUNT)realoff, -10), NULL);
		if (idx || (typlen && (type & TPUTLEN)))
			{
			chput("(",
				(typlen && (type & TPUTLEN)) ? nn(typlen, -10) : "", NULL);
			if (idx)
				chput(",", xn, NULL);
			chput(")", NULL);
			chsize += 1;
			}
		}
	}

/*	put a K call or jump
 */
TEXT *genkall(q)
	FAST TEXT *q;
	{
	IMPORT TEXT runname[];
	FAST TEXT *sq;

	chwrite("b ", 2);
	xref(q + 1);
	chwrite(runname, lenstr(runname));
	for (sq = q; *++q != ';' && *q;)
		chwrite(q, 1);
	return (q);
	}

/*	fixup before gen
 */
RESOURCE genfix(p, set, lat, rat)
	EXPR *p;
	RESOURCE set;
	ATTR rat, lat;
	{
	IMPORT BYTES chsize;
	FAST EXPR *l = p->e.o.left;
	FAST EXPR *r = p->e.o.right;
	BOOL dol, dor, xr2free;
	EXPR *toxr1, *toxr2;
	RESOURCE restore;
	TINY lidx = l->f.idx;
	TINY ridx = r->f.idx;
	WANTS lgot = gotten(l);
	WANTS rgot = gotten(r);

dbprint("/genoff %S \nl: %A r: %A", set, l, r);
	toxr1 = NULL;
	toxr2 = NULL;
	restore = 0;
	xr2free = (lidx != XXR2 && ridx != XXR2);
	dol = (lgot & WPMEM || (lgot & (WMEM|WVMEM) && !gotadr(l)));
	dor = (rgot & WPMEM || (rgot & (WMEM|WVMEM) && !gotadr(r)));
	if (rat == XROP)
		{
		if (lat != XROP)
			panic("GENFIX XROP");
		l->f.refs++;
		r->f.refs++;
		if (lgot & WVXR2)
			toxr1 = r;
		else
			{
			toxr1 = l;
			toxr2 = r;
			}
		}
	else if (rat == PXR2OP && !gpxr2(r))
		toxr2 = r; /* fail if not free ? */
	else if (dol && dor)
		{
		if (lidx == XXR1 && ridx == XXR1 && l->f.refs == 1 && r->f.refs == 1 &&
			(-256 < l->f.bias - r->f.bias && l->f.bias - r->f.bias < 256))
			if (l->f.bias > r->f.bias)
				{
				toxr1 = r;
				l->f.bias -= r->f.bias;
				}
			else
				{
				toxr1 = l;
				r->f.bias -= l->f.bias;
				}
		else
			{			
			if (!xr2free)
				panic("genoff");
			toxr1 = l;
			toxr2 = r;
			}
		}
	else if (dol)
		if ((xr2free && (set & XR2) || ridx == XXR1))
			toxr2 = l;
		else
		    toxr1 = l;
	else if (dor)
		if (xr2free && (set & XR2) || lidx == XXR1)
			toxr2 = r;
		else
		    toxr1 = r;
	if (toxr2 && (!xr2free || !(set & XR2)))
		{
		chput("^st @xr2+1,#xr2;", NULL);
		chsize += 4;
		restore |= XR2;
		}
	if (toxr2) /* xr2 f|rst garanterar xr1 or|rd */
		genpx2(toxr2);
	if (toxr1)
		{
		genpx1(toxr1);
		restore |= XR1;
		}
	if (rat == XROP)
		{
		l->f.refs--;
		r->f.refs--;
		}
	return (restore);
	}

VOID genpx1(p)
	FAST EXPR *p;
	{
	IMPORT BYTES chsize;
	FAST COUNT refs;
	EXPR ex;

	refs = p->f.refs;
	p->f.refs = 1;
	if (p->f.idx == XAC0)
		{
		chput("^l @ac0+7,#xr1;", NULL);
		chsize += 4;
		}
	else if (!gotadr(p))
		{
#ifdef DEBUG
if (p->f.idx != XXR1 || p->f.nm[0])
	panic("GENPX1");
#endif
		exzap(&ex);
		setad(&ex, YUSHORT, 0, 0);
		ex.f.bias = p->f.bias;
		mklit(&ex, YUSHORT);
		chput("^a ", NULL);
		genad(&ex, 0, NO, TWORD, 2);
		chput(",#xr1;", NULL);
		chsize += 2;
		}
	else if (refs > 1)
		{
		chput("^l ", NULL);
		genad(p, 0, NO, TWORD, 2);
		chput(",#xr1;", NULL);
		chsize += 2;
		refs--;
		}
	else
		{
		chput("^la ", NULL);
		genad(p, 0, NO, TWORD, 1);
		chput(",#xr1;", NULL);
		chsize += 2;
		}
	p->f.bias = 0;
	p->f.nm = "";
	p->f.idx = XXR1;
	while (refs-- > 1)
		{
		chput("^l 1(,#xr1),#xr1;", NULL);
		chsize += 3;
		}
	}

/*  generate pointer in XR2
 */
VOID genpx2(p)
	FAST EXPR *p;
	{
	IMPORT BYTES chsize;
	FAST COUNT refs;

	refs = p->f.refs;
	p->f.refs = 1;
	if (p->f.idx == XAC0)
		{
		chput("^l @ac0+7,#xr2;", NULL);
		chsize += 4;
		}
	else if (!gotadr(p))
		{
#ifdef DEBUG
if (p->f.idx != XXR1 || p->f.nm[0])
	panic("GENPX2");
#endif
		chput("^la ", nn((COUNT)p->f.bias, -10), ",#xr2;", NULL);
		chput("^a @fp+1,#xr2;", NULL);
		chsize += 8;
		}
	else if (refs > 1)
		{
		chput("^l ", NULL);
		genad(p, 0, NO, TWORD, 2);
		chput(",#xr2;", NULL);
		chsize += 3;
		refs--;
		}
	else
		{
		chput("^la ", NULL);
		genad(p, 0, NO, TWORD, 1);
		chput(",#xr2;", NULL);
		chsize += 3;
		}
	p->f.bias = 0;
	p->f.nm = "";
	p->f.idx = XXR2;
	while (refs-- > 1)
		chput("^l 1(,#xr2),#xr2;", NULL);
	}

/*	make a litteral
 */
VOID mklit(p, ty)
	FAST EXPR *p;
	TYPE ty;
	{
	FAST TEXT *s;
	FAST EXPR *pp;
	COUNT littype;
	LONG offset;

dbprint("/mklit %A", p);
	if (p->f.nm[0])
		ty = YUSHORT;
	if (ty <= YULONG)
		{
		if (p->f.nm[0])
			littype = TAGA;
		else if (ty <= YUSHORT)
			littype = TAGW;
		else
			littype = TAGL;
		}
	else
		littype = (ty == YDOUBLE) ? TAGD : TAGF;
	offset = 0L;
	s = putlit(p, littype, &offset);
	pp = mexpr(lenstr(s));			/* ! */
	setad(p, ty, 0, 1);
	cpystr(pp->name, s, NULL);
	p->f.bias = offset;
	p->f.nm = pp->name;
	}


/*	compute effective address into reg, also get XR1 and XR2 into memory
 */
RESOURCE reref(l, r, set)
	FAST EXPR *l, *r;
	RESOURCE set;
	{
	IMPORT BYTES chsize;
	FAST TEXT *s;
	BOOL isxr2, xr1res;

	xr1res = NO;
dbprint("{/reref %S %E %E", set, l, r);
/*	if (r->got & GJUNK)  */  /* Why!! */
		set |= voltor(r);
	/* get idx+bias */
	if (l->f.idx == XXR2)
		{
		s = "#xr2";
		isxr2 = YES;
		}
	else
		{
		s = "#wr4";
		isxr2 = NO;
		}
	if ((r->f.idx == XXR1 || r->f.idx == XXR2) && !r->f.nm[0] &&
		0 <= r->f.bias && r->f.bias < 256)
		{
		if (!r->f.bias && !isxr2)
			s = xname[r->f.idx];
		else
			{
			chput("^la ", nn((BYTES)r->f.bias, -10), 
			"(,", xname[r->f.idx], "),", s, ";", NULL);
			chsize += 3;
			}
		}
	else
		{
		chput("^la ", nn((COUNT)r->f.bias, -10), ",", s, ";", NULL);
		chsize += 2;
		if (r->f.idx == XXR1)
			{
			chput("^a @fp+1,", s, ";", NULL);
			chsize += 4;
			}
		}
	if (!isxr2)
		{
		if (!gotadr(l))
			{
			genpx1(l);
			xr1res = YES;
			}
		chput("^st ", NULL);
		genad(l, 0, NO, TWORD, 2);
		chput(",", s, ";", NULL);
		chsize += 2;
		if (xr1res)
			{
			chput("^l @fp+1,#xr1;", NULL);
			chsize += 4;
			}
		}
	cpyad(&r->f, &l->f);
	r->got = l->got;
	return (SUCC(set));
	}

/*	pick an x register name
 */
TEXT *xnm(idx)
	TINY idx;
	{
	IMPORT TEXT *xname[];
	IMPORT TINY xtab[];

	return (xname[scnstr(xtab, idx)]);
	}
