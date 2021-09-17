/*	SUBROUTINES FOR PASS 1
 *	copyright 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int12.h"

/*	index for boundaries and bytesizes
 */
#define ITIDX	7
BYTES bndtab[] {0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 1, -1};
BYTES bytab[] {1, 1, 2, 2, 2, 4, 4, 4, 4, 8, 2, 0};
GLOBAL TINY typtab[] {TCHAR, TUCHAR, TSHORT, TSFIELD, TUSHORT, TLONG,
		TLFIELD, TULONG, TFLOAT, TDOUBLE, TPTRTO, 0};

/*	enforce bound based on type, attributes and offset
 */
LONG bndify(ty, at, off)
	BITS ty;
	FAST ATTR *at;
	LONG off;
	{
	BYTES b;

	b = (1 << bound(ty, at)) - 1;
	off = off + b;
	off = off & ~(LONG)b;
	return (off);
	}

/*	take the boundary based on type and attribute chain
 */
BYTES bound(ty, at)
	FAST BITS ty;
	FAST ATTR *at;
	{
	IMPORT BYTES bndef, bndtab[];
	IMPORT TINY typtab[];
	FAST SYMBOL *p;
	BYTES i;

	if ((i = bndtab[scnstr(typtab, type(ty))]) != -1)
		return (min(i, bndef));
	else if (type(ty) == TARRAY)
		return (bound(dety(ty), at->next));
	else if (ty == TSTRUCT || ty == TUNION)
		{
		for (i = 0, p = at->a.stab->s.tagtab; p; p = p->next)
			i = max(i, bound(p->ty, p->at));
		return (i);
		}
	else
		return (4);
	}

/*	buy space for an ATTR
 */
ATTR *buymat(m, link)
	LONG m;
	ATTR *link;
	{
	FAST ATTR *pa;

	pa = alloc(sizeof (*pa), link);
	pa->a.m = m;
	return (pa);
	}

/*	buy space for an op and copy into it
 */
TERM *buyop(op, left, right)
	LEX op;
	TERM *left, *right;
	{
	IMPORT TERM *exlist;
	FAST TERM *q;

	exlist = q = alloc(sizeof (*q), exlist);
	q->ty = 0;
	q->at = NULL;
	q->op = op;
	q->e.o.left = left;
	q->e.o.right = right;
	q->e.o.mid = NULL;
	return (q);
	}

/*	buy space for a symbol ATTR
 */
ATTR *buysat(stab, link)
	FAST TEXT *stab;
	ATTR *link;
	{
	FAST ATTR *pa;

	pa = alloc(sizeof (*pa), link);
	pa->a.stab = stab;
	return (pa);
	}

/*	buy space for a SYMBOL and copy into it
 */
SYMBOL *buysym(ty, at, name, sc)
	BITS ty;
	ATTR *at;
	FAST TEXT *name;
	LEX sc;
	{
	IMPORT TEXT noname[];
	FAST SYMBOL *q;

	q = alloc(sizeof (*q), NULL);
	setty(q, ty, at);
	cpynm(q->n.an, name ? name : noname);
	q->sc = sc;
	q->s.offset = 0L;
	return (q);
	}

/*	buy space for a term and copy into it
 */
TERM *buyterm(ty, at, an, bias, idx, refs)
	BITS ty;
	ATTR *at;
	TEXT *an;
	LONG bias;
	TINY idx, refs;
	{
	IMPORT TERM *exlist;
	IMPORT TEXT noname[];
	FAST TERM *q;

	exlist = q = alloc(sizeof (*q), exlist);
	q->ty = ty;
	q->at = at;
	q->op = 0;
	cpynm(q->n.an, an ? an : noname);
	q->e.v.bias = bias;
	q->e.v.idx = idx;
	q->e.v.refs = refs;
	return (q);
	}

/*	take the sizeof a declarator based on type, attr
 */
LONG bytes(ty, at)
	FAST BITS ty;
	FAST ATTR *at;
	{
	IMPORT BYTES bytab[];
	IMPORT LONG bndify();
	IMPORT TINY typtab[];
	BYTES i;
	FAST SYMBOL *p;
	LONG j, k;

	if (i = bytab[scnstr(typtab, type(ty))])
		return ((LONG)i);
	else switch (type(ty))
		{
	case TARRAY:
		if (!at->a.m)
			perror("array size unknown");
		return (at->a.m * bytes(dety(ty), at->next));
	case TSTRUCT:
		if (!(p = at->a.stab->s.tagtab))
			{
			perror("structure size unknown");
			return (0L);
			}
		else
			{
			while (p->next)
				p = p->next;
			return (bndify(ty, at, bytes(p->ty, p->at) + p->s.offset));
			}
	case TUNION:
		if (!(p = at->a.stab->s.tagtab))
			{
			perror("union size unknown");
			return (0L);
			}
		else
			{
			for (j = 0; p; p = p->next)
				{
				k = bytes(p->ty, p->at);
				j = max(j, k);
				}
			return (bndify(ty, at, j));
			}
	default:
		perror("function size undefined");
		return (0L);
		}
	}

/*	clear a symbol table entry
 */
SYMBOL *clrsym(p)
	FAST SYMBOL *p;
	{
	FAST COUNT i;

	p->next = NULL;
	p->ty = 0;
	p->at = NULL;
	for (i = 0; i < LENNAME; ++i)
		p->n.an[i] = '\0';
	p->sc = 0;
	p->s.offset = 0L;
	}

/*	copy name field
 */
VOID cpynm(d, s)
	FAST TEXT *d, *s;
	{
	FAST COUNT i;

	for (i = LENNAME; 0 <= --i; )
		*d++ = *s++;
	}

/* copy a token structure
 */
TOKEN *cpytok(p, q)
	FAST TOKEN *p, *q;
	{
	p->type = q->type;
	cpynm(p->t.an, q->t.an);
	return (p);
	}

/*	pick a type off the ty field
 */
BITS dety(ty)
	BITS ty;
	{
	return ((ty >> 2) & 037777);
	}

/*	test for double literal
 */
BOOL dlit(q)
	FAST TERM *q;
	{
	IMPORT LITERAL *littab;

	return (q->ty == TDOUBLE && !q->op && !q->e.v.refs);
	}

/*	is ty of type float or double
 */
BOOL dtype(ty)
	BITS ty;
	{
	return (ty == TDOUBLE || ty == TFLOAT);
	}

/*	check for duplicate externs
 */
VOID exchk(p)
	FAST SYMBOL *p;
	{
	IMPORT BOOL cflag;
	IMPORT BYTES nlen;
	FAST SYMBOL *q;

	if (LENNAME <= nlen && !cflag)
		return;
	for (q = p; q = q->next; )
		if (exmatch(p, q))
			{
			nmerr("external name conflict", p->n.an);
			return;
			}
	}

/*	compare two external symbols for name match
 */
BOOL exmatch(p, q)
	SYMBOL *p, *q;
	{
	IMPORT BOOL cflag;
	IMPORT BYTES nlen;
	FAST COUNT i;
	FAST TEXT *s, *t;

	if (q->sc != LEXTERN && q->sc != DEXTERN &&
		q->sc != LSTATIC && q->sc != DSTATIC)
		return (NO);
	for (i = min(nlen, LENNAME), s = p->n.an, t = q->n.an; 0 <= --i; ++s, ++t)
		if (cflag && tolower(*s) != tolower(*t) || !cflag && *s != *t)
			break;
	return (i < 0);
	}

/*	fixup tables for long ints
 */
VOID fixlint()
	{
	IMPORT BITS tfield, tint, tunsign;
	IMPORT BYTES bitswd, bndtab[], bytab[], intsize;
	IMPORT TINY typtab[];
	FAST BYTES i = scnstr(typtab, TPTRTO);

	bitswd = 32;
	intsize = 4;
	tfield = TLFIELD;
	tint = TLONG;
	tunsign = TULONG;
	bndtab[i] = 2;
	bytab[i] = 4;
	}

/*	is a term a constant
 */
BOOL iscons(q)
	FAST TERM *q;
	{
	return (!(q->op || q->n.an[0] || q->e.v.idx || q->e.v.refs
		|| q->ty == TDOUBLE));
	}

/*	is ty of type integer
 */
BOOL itype(ty)
	BITS ty;
	{
	IMPORT TINY typtab[];

	return (scnstr(typtab, type(ty)) <= ITIDX);
	}

/*	find the length of a name or double
 */
BYTES lname(s)
	FAST TEXT *s;
	{
	FAST COUNT i;

	for (i = LENNAME; 0 <= --i; )
		if (s[i])
			return (i + 1);
	return (0);
	}

/*	lookup an identifier in the symbol table
 */
SYMBOL *lookup(name, from, to)
	TEXT *name;
	SYMBOL *from, *to;
	{
	FAST SYMBOL *q;
	FAST TEXT *s, *t;
	COUNT i;

	for (q = from; q != to; q = q->next)
		{
		for (s = q->n.an, t = name, i = LENNAME; 0 < i; --i)
			if (*s++ != *t++)
				break;
		if (i == 0)
			return (q);
		}
	return (NULL);
	}

/*	set maximum legal type
 */
BITS maxify(ty)
	BITS ty;
	{
	IMPORT BITS tint, tunsign;

	switch (ty)
		{
	case TSFIELD:
	case TLFIELD:
		return (tunsign);
	case TCHAR:
	case TUCHAR:
	case TSHORT:
	case TUSHORT:
		if (ty < tint)
			return (tint);
		break;
	case TFLOAT:
		return (TDOUBLE);
		}
	return (ty);
	}

/*	is ty of type pointer to
 */
BOOL ptype(ty)
	BITS ty;
	{
	return ((ty & 3) == TPTRTO);
	}

/*	add to the type field
 */
SYMBOL *retype(p, nty)
	FAST SYMBOL *p;
	FAST BITS nty;
	{
	FAST COUNT i;

	if (!p)
		perror("incomplete declaration");
	else
		{
		for (i = 0; (3 << 2 * i) & p->ty; ++i)
			;
		if (5 <= i)
			perror("declaration too complex");
		else
			p->ty = (p->ty << 2) | nty;
		}
	return (p);
	}

/*	set address attributes
 */
TERM *setad(p, name, bias, idx, refs)
	FAST TERM *p;
	FAST TEXT *name;
	LONG bias;
	TINY idx, refs;
	{
	IMPORT TEXT noname[];

	p->op = 0;
	cpynm(p->n.an, name ? name : noname);
	p->e.v.bias = bias;
	p->e.v.idx = idx;
	p->e.v.refs = refs;
	return (p);
	}

/*	set type attributes
 */
SYMBOL *setty(p, ty, at)
	FAST SYMBOL *p;
	BITS ty;
	ATTR *at;
	{
	p->ty = ty;
	p->at = at;
	return (p);
	}

/*	is ty a scalar type ?
 */
BOOL stype(ty)
	BITS ty;
	{
	IMPORT TINY typtab[];

	return (typtab[scnstr(typtab, type(ty))] != 0);
	}

/*	return the type of the symbol
 */
BITS type(ty)
	FAST BITS ty;
	{
	return ((ty & 3) ? ty & 3 : ty);
	}
