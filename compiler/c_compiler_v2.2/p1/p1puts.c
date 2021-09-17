/*	OUTPUT FROM PASS1
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int12.h"
#include "int012.h"

/*	zero term
 */
LOCAL TERM zeterm {NULL, NULL, 0, TCHAR};

/*	table of type for pass2
 */

/*	table of relational ops and flipped relops
 */
LOCAL TINY relops[] {LISEQ, LNOTEQ, LLESS, LLEQ, LGREAT, LGEQ, 0};
LOCAL TINY reflip[] {LNOTEQ, LISEQ, LGEQ, LGREAT, LLEQ, LLESS, 0};
LOCAL TINY jctab[] {GISEQ, GNOTEQ, GLESS, GLEQ, GGREAT, GGEQ, 0};

/*	create a label and return it
 */
LABEL crs()
	{
	INTERN LABEL nextsym {-1};

	return (nextsym =+ 2);
	}

/*	make a label number printable
 */
TEXT *lblname(label)
	FAST LABEL label;
	{
	FAST COUNT i;
	INTERN TEXT name[LENNAME];

	name[0] = '>';
	for (i = 1; label && (i < 6); ++i)
		{
		name[i] = (label & 07) + '0';
		label = (label >> 3) & 017777;
		}
	while (i < LENNAME)
		name[i++] = '\0';
	return (name);
	}

/*	put an address
 */
BYTES paddr(s, by, bnd)
	FAST TEXT *s;
	LONG by;
	BYTES bnd;
	{
	IMPORT LONG bytes();
	FAST BYTES n;

	n = bytes(TPTRTO, NULL, YES);
	if (*s)
		{
		pcode(GADDR + bnd);
		ptname(s);
		ptint(by);
		}
	else
		pint(by, n);
	return (n);
	}

/*	pass the current auto offset
 */
VOID pauto(by)
	LONG by;
	{
	pcode(GAUTOFF);
	ptint(by);
	}

/*	put case name
 */
LABEL pcase(at)
	FAST LABEL at;
	{
	IMPORT COUNT lineno;

	pcode(GCASE);
	ptlab(at);
	ptlab(lineno);
	return (at);
	}

/*	put a code byte
 */
VOID pcode(c)
	COUNT c;
	{
	putch(c & BYTMASK);
	}

/*	put data name
 */
VOID pdata(s, bnd, ndata)
	TEXT *s;
	BYTES bnd;
	BOOL ndata;
	{
	IMPORT BOOL dflag;

	if (dflag && ndata)
		pcode(GNDATA);
	else
		pcode(GDATA);
	pcode(bnd);
	ptname(s);
	}

/*	put a definition
 */
VOID pdef(s)
	TEXT *s;
	{
	pcode(GDEF);
	ptname(s);
	}

/*	end a data or function initializer
 */
VOID pend()
	{
	IMPORT LITERAL *littab;
	FAST COUNT i;
	FAST LITERAL *p;

	for (p = littab; p; p = free(p, p->next))
		{
		if (p->sc)
			{
			pcode(GLITS);
			pcode(0);
			ptname(p->n.an);
			i = p->e.l.a.m;
			pstr(p->e.l.next, i - 1);
			}
		free(p->e.l.next, NULL);
		}
	littab = NULL;
	}

/*	put a floating constant PDP-11
 */
BYTES pfloat(pd, n)
	FAST TEXT *pd;
	FAST BYTES n;
	{
	FAST COUNT i;

	pcode(GFLOAT);
	pcode(n);
	for (i = sizeof (DOUBLE); 0 <= --i; )
		pcode(*pd++);
	return (n);
	}

/*	put a function start
 */
VOID pfunc(s)
	TEXT *s;
	{
	pcode(GFUNC);
	ptname(s);
	}

/*	put an integer constant
 */
BYTES pint(b, n)
	LONG b;
	FAST BYTES n;
	{
	pcode(GINT);
	pcode(n);
	ptint(b);
	return (n);
	}

/*	put a jcond 
 */
LABEL pjc(rop, left, right, to)
	LEX rop;
	TERM *left, *right;
	FAST LABEL to;
	{
	IMPORT TINY relops[], jctab[];

	if (!to)
		to = crs();
	pcode(jctab[scnstr(relops, rop)]);
	ptlab(to);
	ptexpr(left);
	ptexpr(right);
	return (to);
	}

/*	put a jump false
 */
LABEL pjf(q, fb, tb)
	FAST TERM *q;
	FAST LABEL fb, tb;
	{
	IMPORT TERM zeterm;
	IMPORT TINY reflip[], relops[];
	TINY i;

	if (!q)
		return (fb ? fb : crs());
	else if (q->op == LANDAND)
		{
		fb = pjf(q->e.o.left, fb, 0);
		return (pjf(q->e.o.right, fb, tb));
		}
	else if (q->op == LOROR)
		{
		tb = pjt(q->e.o.left, 0, tb);
		return (pjf(q->e.o.right, fb, tb));
		}
	else if (q->op == LNOT)
		return (pjt(q->e.o.left, tb, fb));
	else if (relops[i = scnstr(relops, q->op)])
		fb = pjc(reflip[i] & BYTMASK, q->e.o.left, q->e.o.right, fb);
	else
		fb = pjc(LISEQ, q, &zeterm, fb);
	plabel(tb);
	return (fb);
	}

/*	put a jump true
 */
LABEL pjt(q, fb, tb)
	FAST TERM *q;
	FAST LABEL fb, tb;
	{
	IMPORT TERM zeterm;
	IMPORT TINY relops[];

	if (!q)
		return (pjump(tb, fb));
	else if (q->op == LANDAND)
		{
		fb = pjf(q->e.o.left, fb, 0);
		return (pjt(q->e.o.right, fb, tb));
		}
	else if (q->op == LOROR)
		{
		tb = pjt(q->e.o.left, 0, tb);
		return (pjt(q->e.o.right, fb, tb));
		}
	else if (q->op == LNOT)
		return (pjf(q->e.o.left, tb, fb));
	else if (relops[scnstr(relops, q->op)])
		tb = pjc(q->op, q->e.o.left, q->e.o.right, tb);
	else
		tb = pjc(LNOTEQ, q, &zeterm, tb);
	plabel(fb);
	return (tb);
	}

/*	put a jump
 */
LABEL pjump(to, at)
	FAST LABEL to, at;
	{
	if (!to)
		to = crs();
	pcode(GJUMP);
	ptlab(to);
	plabel(at);
	return (to);
	}

/*	put a label
 */
LABEL plabel(at)
	FAST LABEL at;
	{
	IMPORT COUNT lineno;

	if (at)
		{
		pcode(GLABEL);
		ptlab(at);
		ptlab(lineno);
		}
	return (at);
	}

/*	put a move
 */
VOID pmove(left, right)
	TERM *left, *right;
	{
	INTERN TERM mvterm;

	mvterm.op = DGETS;
	mvterm.at = NULL;
	mvterm.e.o.left = left;
	mvterm.e.o.right = right;
	pvoid(typify(&mvterm));
	}

/*	put a reference
 */
VOID pref(s)
	TEXT *s;
	{
	pcode(GREF);
	ptname(s);
	}

/*	pass the current reg high water mark
 *	NB: REGSET & 0200 => r & 02
 */
VOID pregs(r)
	FAST BITS r;
	{
	pcode(GREGS);
	pcode(r & 034 | (r & ~034) >> 6);
	}

/*	put a return
 */
VOID pret()
	{
	pcode(GRET);
	}

/*	put a space request
 */
VOID pspace(by)
	LONG by;
	{
	if (0 < by)
		{
		pcode(GSPACE);
		ptint(by);
		}
	}

/*	put out a string
 */
VOID pstr(s, n)
	FAST TEXT *s;
	FAST COUNT n;
	{
	for (; 256 <= n; --n, ++s)	/* HOT PATCH */
		pint((LONG)*s, 1);
	pcode(GSTRING);
	pcode(n);
	for (; 0 < n; --n)
		pcode(*s++);
	}

/*	put a switch statement
 */
VOID pswitch(tab)
	LABEL tab;
	{
	pcode(GSWITCH);
	ptlab(tab);
	}

/*	put a switch table
 */
VOID pswtab(ptab, def, tab)
	FAST CASE **ptab;
	LABEL def, tab;
	{
	IMPORT BITS tunsign;
	IMPORT BYTES intsize;
	FAST CASE *p;

	pcode(GSWTAB);
	pcode(bound(tunsign, 0));
	ptname(lblname(tab));
	for (p = *ptab; p != ptab; p = free(p, p->next))
		{
		paddr(lblname(p->clabel), 0L, 4);
		pint(p->cvalue, intsize);
		}
	pint(0L, intsize);
	paddr(lblname(def), 0L, 4);
	pcode(GSWEND);
	}

/*	put an expression
 */
VOID ptexpr(p)
	FAST TERM *p;
	{
	IMPORT BITS tfield;
	IMPORT TINY typtab[];
	FAST COUNT i;
	FAST TEXT *s;
	INTERN TINY xtab[] {XCHAR, XUCHAR, XSHORT, XSFIELD, XUSHORT, XLONG,
		XLFIELD, XULONG, XFLOAT, XDOUBLE, XPTRTO, XUSHORT};

	pcode(p->op);
	if ((i = xtab[scnstr(typtab, type(p->ty))]) != XPTRTO)
		pcode(i);
	else
		pcode(XPTRTO | bound(dety(p->ty), p->at) << 4);
	if (p->ty == tfield)
		{
		pcode(p->at->a.b.boff);
		pcode(p->at->a.b.bsize);
		}
	if (p->op)
		{
		ptexpr(p->e.o.left);
		if (p->op == LQUERY)
			ptexpr(p->e.o.mid);
		if (p->op & BINOP)
			ptexpr(p->e.o.right);
		}
	else
		{
		if (p->ty == TDOUBLE && !p->e.v.refs && !p->e.v.idx)
			{
			pcode(8);
			for (i = 8, s = p->n.an; 0 <= --i; )
				pcode(*s++);
			}
		else
			ptname(p->n.an);
		ptint(p->e.v.bias);
		pcode(p->e.v.idx);
		pcode(p->e.v.refs);
		}
	}

/*	put an integer
 */
VOID ptint(lo)
	LONG lo;
	{
	FAST COUNT i;
	FAST TEXT *q;

	for (q = &lo, i = sizeof (lo); 0 < i; --i)
		pcode(*q++);
	}

/*	put a label
 */
VOID ptlab(arglab)
	LABEL arglab;
	{
	FAST COUNT i;
	FAST TEXT *q;
	LABEL lab = arglab;

	for (q = &lab, i = sizeof (lab); 0 < i; --i)
		pcode(*q++);
	}

/*	put a name
 */
VOID ptname(s)
	FAST TEXT *s;
	{
	IMPORT BOOL cflag;
	IMPORT BYTES nlen;
	FAST COUNT i;

	i = lname(s);
	if (nlen < i)
		i = nlen;
	for (pcode(i); 0 <= --i; ++s)
		pcode(cflag ? tolower(*s) : *s);
	}

/*	put a void expression
 */
VOID pvoid(expr)
	TERM *expr;
	{
	docheck(expr);
	pcode(GVOID);
	ptexpr(expr);
	}

/*	buy a register
 */
TINY rbuy(ty, pset)
	FAST BITS ty, *pset;
	{
	IMPORT BITS tunsign;
	IMPORT BOOL aflag;
	FAST BITS set;
	BITS mask;

	if (!*pset)
		return (0);
	switch (type(ty))
		{
	case TCHAR:
	case TUCHAR:
	case TSHORT:
	case TUSHORT:
	case TPTRTO:
		break;
	case TLONG:
	case TULONG:
		if (ty <= tunsign)
			break;
	default:
		return (0);
		}
	mask = (aflag && (*pset & 034000) && type(ty) == TPTRTO) ? 034000 : -1;
	set = *pset & mask;
	set =^ set & set - 1;
	*pset =& ~set;
	return (set & 034 ? set : set >> 9 | 0200);
	}
