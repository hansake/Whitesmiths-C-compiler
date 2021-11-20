/*	DATA INITIALIZERS
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int012.h"

GLOBAL BOOL decflag {NO};

/*	initialize an array
 */
LONG arinit(ty, at, q, list)
	BITS ty;
	ATTR *at;
	FAST TERM *q;
	BOOL list;
	{
	IMPORT LITERAL *littab;
	IMPORT LONG dinit();
	FAST COUNT i;
	FAST LITERAL *l;
	LONG size;

	if ((ty == TCHAR || ty == TUCHAR) && q &&
		(l = lookup(q->n.an, littab, NULL)))
		{
		i = l->e.l.a.m;
		pstr(l->e.l.next, i - 1);
		l->sc = 0;
		if (at->a.m == 0)
			at->a.m = i;
		else if (at->a.m < i)
			perror("string initializer too long");
		size = i;
		}
	else
		{
		for (size = 0L, i = 0; (at->a.m == 0 || i < at->a.m) &&
			(q || eat(LLCURLY) || (q = gexpr(NO)));
			++i, q = NULL)
			{
			size =+ dinit(ty, at->next, q, list);
			if (!q)
				need(LRCURLY);
			if (list)
				eat(LCOMMA);
			}
		if (at->a.m == 0)
			at->a.m = i;
		}
	return (size);
	}

/*	initialize a data declaration
 */
VOID datinit(p)
	FAST SYMBOL *p;
	{
	IMPORT BOOL decflag;
	IMPORT TERM *gexpr();
	FAST LEX ate, lgets;
	FAST TERM *q;
	BOOL ndata;
	BYTES bnd;

	decflag = YES;
	lgets = eat(LGETS);
	bnd = bound(p->ty, p->at);
	if ((q = gexpr(NO)) || (ate = eat(LLCURLY)) || p->sc == FSTATIC)
		{
		if ( !lgets || ( ate && peek(LRCURLY) ) )
			ndata = YES;
		else
			ndata = NO;
		if (p->sc == DEXTERN || p->sc == DSTATIC)
			nmerr("redefined", p->n.an);
		else if (p->sc == LEXTERN)
			{
			p->sc = DEXTERN;
			pdata(p->n.an, bnd, ndata);
			}
		else if (p->sc == LSTATIC)
			{
			p->sc = DSTATIC;
			pdata(p->n.an, bnd, ndata);
			}
		else if (p->sc == FSTATIC)
			pdata(lblname(p->s.label), bnd, ndata);
		dinit(p->ty, p->at, q ? q : gexpr(NO), !q);
		if (!q && ate && !eat(LRCURLY))
			recover("too many initializers");
		pend();
		}
	decflag = NO;
	}

/*	parse a data initializer and return the number of bytes initialized
 */
LONG dinit(ty, at, q, list)
	BITS ty;
	ATTR *at;
	FAST TERM *q;
	BOOL list;
	{
	IMPORT LONG arinit(), bytes(), stinit();
	FAST COUNT i;
	DOUBLE dnum;
	LONG n, size;

	size = 0L;
	if (type(ty) == TARRAY)
		size = arinit(dety(ty), at, q, list);
	else if (type(ty) == TSTRUCT)
		size = stinit(at->a.stab->s.tagtab, q, list);
	else if (q || (q = gexpr(NO)))
		{
		switch (type(ty))
			{
		case TUNION:
			if (at->a.stab->s.tagtab)
				size = dinit(at->a.stab->s.tagtab->ty,
					at->a.stab->s.tagtab->at, q, list);
			break;
		case TCHAR:
		case TUCHAR:
		case TSHORT:
		case TUSHORT:
		case TLONG:
		case TULONG:
			if (!iscons(q))
				perror("illegal integer initializer");
			else
				{
				i = bytes(ty, at);
				size = pint(q->e.v.bias, i);
				}
			break;
		case TPTRTO:
			if (q->op || 1 < q->e.v.refs || q->e.v.idx)
				perror("illegal pointer initializer");
			else
				size = paddr(q->n.an, q->e.v.bias, bound(dety(ty), at));
			break;
		case TFLOAT:
		case TDOUBLE:
			i = bytes(ty, at);
			if (dlit(q))
				size = pfloat(&q->n.dn, i);
			else if (iscons(q))
				{
				dnum = q->e.v.bias;
				size = pfloat(&dnum, i);
				}
			else
				perror("illegal double initializer");
			break;
		default:
			perror("cannot initialize");
			}
		if (list)
			eat(LCOMMA);
		}
	n = bytes(ty, at);
	pspace(n - size);
	return (n);
	}

/*	initialize a structure
 */
LONG stinit(p, q, list)
	FAST SYMBOL *p;
	FAST TERM *q;
	BOOL list;
	{
	IMPORT BITS tfield;
	IMPORT LONG bytes(), dinit();
	FAST TERM *r;
	LONG n, size;

	for (size = 0L; p && (q || eat(LLCURLY) || (q = gexpr(NO))); p = p->next)
		{
		while (p && !p->n.an[0])
			p = p->next;
		if (!p)
			break;
		pspace(p->s.offset - size);
		if (p->ty != tfield)
			size = p->s.offset + dinit(p->ty, p->at, q, list);
		else
			{
			n = 0L;
			for (r = q; r || (r = gexpr(NO)); p = p->next)
				{
				if (p->n.an[0])
					{
					if (!iscons(r))
						perror("illegal field initializer");
					else
						n =| (r->e.v.bias & ((1L << p->at->a.b.bsize) - 1))
							<< (LONG) p->at->a.b.boff;
					if (list)
						eat(LCOMMA);
					r = NULL;
					}
				if (!p->next || p->next->ty != tfield ||
					p->next->s.offset != p->s.offset)
					break;
				}
			size = p->s.offset + pint(n, (BYTES)bytes(tfield, NULL));
			}
		if (!q)
			need(LRCURLY);
		if (list)
			eat(LCOMMA);
		q = NULL;
		}
	return (size);
	}
