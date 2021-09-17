/*	EXPRESSION REDUCTION
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int12.h"
#include "int012.h"

/*	the reducing tables
	0200	reduce right
	0100	reduce left
	0040	if (L is a term) commute
	0020	if (R is zero) take as L and exit
	0010	if (R is zero) take as 0 and exit
	0004	if (R is one) take as L and exit
	0002	if (R not const) exit
	0001	if (L not const) exit
 */
LOCAL TINY recodes[] {0, 0100, 0100, 0, 0101, 0100, 0101,
	0101, 0100, 0100, 0300, 0300, 0100,
	0300, 0300, 0300, 0300, 0300, 0300, 0300, 0300,
	0300, 0300, 0300, 0300, 0300, 0360, 0320,
	0354, 0307, 0303, 0353, 0363, 0363, 0323, 0323,
	0303, 0303, 0303, 0303, 0343, 0343, 0317, 0323, 0, 0};

/*	check if can do arithmetic
 */
BOOL cachk(p)
	FAST TERM *p;
	{
	return (!p->op && !p->e.v.refs && p->ty != TFLOAT && p->ty != TDOUBLE);
	}

/*	add if you can
 */
BOOL canadd(l, r)
	FAST TERM *l, *r;
	{
	if (cachk(l) && cachk(r) &&
		(!l->n.an[0] || !r->n.an[0]) &&
		(!l->e.v.idx || !r->e.v.idx))
		{
		if (!l->n.an[0])
			cpynm(l->n.an, r->n.an);
		l->e.v.bias =+ r->e.v.bias;
		l->e.v.idx =+ r->e.v.idx;
		if (type(l->ty) == TPTRTO)
			;
		else if (type(r->ty) == TPTRTO)
			{
			l->ty = r->ty;
			l->at = r->at;
			}
		else
			l->ty = maxty(l->ty, r->ty, TULONG);
		return (YES);
		}
	return (NO);
	}

/*	multiply if you can
 */
BOOL canmul(l, r)
	FAST TERM *l, *r;
	{
	if (iscons(l) && iscons(r))
		{
		l->e.v.bias =* r->e.v.bias;
		l->ty = maxty(l->ty, r->ty, TULONG);
		return (YES);
		}
	return (NO);
	}

/*	subtract if you can
 */
BOOL cansub(l, r)
	FAST TERM *l, *r;
	{
	IMPORT TEXT noname[];

	if (cachk(l) && cachk(r) &&
		(!r->n.an[0] || cmpbuf(l->n.an, r->n.an, LENNAME)) &&
		(!r->e.v.idx || l->e.v.idx == r->e.v.idx))
		{
		if (r->n.an[0])
			cpynm(l->n.an, noname);
		if (r->e.v.idx)
			l->e.v.idx = 0;
		l->e.v.bias =- r->e.v.bias;
		return (YES);
		}
	return (NO);
	}

/*	copy a term
 */
TERM *cpyterm(to, from)
	FAST TERM *to, *from;
	{
	if (!from->op)
		setad(to, from->n.an, from->e.v.bias, from->e.v.idx,
			from->e.v.refs);
	else
		{
		to->op = from->op;
		to->e.o.left = from->e.o.left;
		to->e.o.right = from->e.o.right;
		to->e.o.mid = from->e.o.mid;
		}
	setty(to, from->ty, from->at);
	return (to);
	}

/*	reduce an expression
 */
TERM *reduce(q)
	FAST TERM *q;
	{
	IMPORT TINY recodes[], tyops[];
	FAST TERM *l, *r;
	COUNT code;
	LITERAL *p;

	if (!q)
		return (NULL);
	l = q->e.o.left;
	r = q->e.o.right;
	code = recodes[scnstr(tyops, q->op)];
	if (code & 0200)
		reduce(r);
	if (code & 0100)
		reduce(l);
	if (code & 0040)
		if (!l->op && (r->op || iscons(l)))
			{
			q->e.o.left = r;
			q->e.o.right = l;
			l = r;
			r = q->e.o.right;
			}
	if (code & 0020)
		if (iscons(r) && r->e.v.bias == 0L)
			return (cpyterm(q, l));
	if (code & 0010)
		if (iscons(r) && r->e.v.bias == 0L)
			return (cpyterm(q, r));
	if (code & 0004)
		if (iscons(r) && r->e.v.bias == 1L)
			return (cpyterm(q, l));
	if (code & 0002)
		if (!iscons(r))
			return (q);
	if (code & 0001)
		if (!iscons(l))
			return (q);
	switch (q->op)
		{
	case DADDR:
		if (!l->op)
			--l->e.v.refs;
		else if (l->op == DINDIR)
			{
			cpyterm(q, l->e.o.left);
			return (q);
			}
		else
			return (q);
		break;
	case DCAST:
		if (!l->op && (type(q->ty) == type(l->ty)
			|| !dtype(q->ty) && !dtype(l->ty)))
			;
		else
			return (q);
		break;
	case DINDIR:
		if (!l->op)
			++l->e.v.refs;
		else if (l->op == DADDR)
			{
			cpyterm(q, l->e.o.left);
			return (q);
			}
		else
			return (q);
		break;
	case LNOT:
		l->e.v.bias = !l->e.v.bias;
		break;
	case LCOMP:
		l->e.v.bias = ~l->e.v.bias;
		break;
	case DMINUS:
		if (dlit(l))
			l->n.dn = -l->n.dn;
		else if (iscons(l))
			l->e.v.bias = -l->e.v.bias;
		else
			return (q);
		break;
	case DPLUS:
		if (dlit(l) || iscons(l))
			break;
		else
			return (q);
		break;
	case LQUERY:
		reduce(q->e.o.mid);
		if (iscons(l))
			cpyterm(q, l->e.v.bias ? (TERM *)q->e.o.mid : r);
		return (q);
	case LPLUS:
		if (canadd(l, r))
			break;
		else if (l->op == LPLUS && !dtype(q->ty) && canadd(l->e.o.right, r))
			{
			l->ty = q->ty;
			l->at = q->at;
			return (cpyterm(q, l));
			}
		else
			return (q);
	case LMINUS:
		if (cansub(l, r))
			break;
		else if (iscons(r))
			{
			q->op = LPLUS;
			r->e.v.bias = -r->e.v.bias;
			return (q);
			}
		else
			return (q);
	case LTIMES:
		if (canmul(l, r))
			break;
		else if (l->op == LPLUS && !dtype(q->ty) && canmul(l->e.o.right, r))
			{
			l->op = LTIMES;
			q->op = LPLUS;
			q->e.o.right = l->e.o.right;
			l->e.o.right = r;
			return (q);
			}
		else
			return (q);
	case LDIVIDE:
		if (!r->e.v.bias)
			perror("illegal /");
		else
			l->e.v.bias =/ r->e.v.bias;
		break;
	case LMODULO:
		if (!r->e.v.bias)
			perror("illegal %");
		else
			l->e.v.bias =% r->e.v.bias;
		break;
	case LAND:
		l->e.v.bias =& r->e.v.bias;
		break;
	case LOR:
		l->e.v.bias =| r->e.v.bias;
		break;
	case LXOR:
		l->e.v.bias =^ r->e.v.bias;
		break;
	case LLSHIFT:
		l->e.v.bias =<< r->e.v.bias;
		break;
	case LRSHIFT:
		l->e.v.bias =>> r->e.v.bias;
		break;
	case LLESS:
		l->e.v.bias = (l->e.v.bias < r->e.v.bias);
		break;
	case LLEQ:
		l->e.v.bias = (l->e.v.bias <= r->e.v.bias);
		break;
	case LGREAT:
		l->e.v.bias = (l->e.v.bias > r->e.v.bias);
		break;
	case LGEQ:
		l->e.v.bias = (l->e.v.bias >= r->e.v.bias);
		break;
	case LISEQ:
		l->e.v.bias = (l->e.v.bias == r->e.v.bias);
		break;
	case LNOTEQ:
		l->e.v.bias = (l->e.v.bias != r->e.v.bias);
		break;
	default:
		return (q);
		}
	setad(q, l->n.an, l->e.v.bias, l->e.v.idx, l->e.v.refs);
	return (q);
	}
