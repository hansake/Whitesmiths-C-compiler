/*	EXPRESSION TYPING
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int12.h"
#include "int012.h"

/*	the protean integer symbol
 */
LOCAL SYMBOL intsym {NULL, TCHAR};

/*	the typing tables
	0200	determine type of right operand
	0100	force scalar type for right operand
	0020	determine type of left operand
	0010	force scalar type for left operand
	0040	test left operand for lvalueness
	0004	type is max(left, right), int or double
	0002	type is max(left, right), integer
	0001	type is that of left operand
	0400	type is at least int (or double if float)
 */
LOCAL BITS tycodes[] {020, 021, 0400, 030, 0430, 030, 0430,
	0431, 0431, 0431, 0771, 0771, 0400,
	0330, 0330, 0371, 0371, 0371, 0375, 0375, 0373,
	0373, 0373, 0373, 0373, 0373, 0730, 0730,
	0734, 0734, 0732, 0732, 0732, 0732, 0733, 0733,
	0730, 0730, 0730, 0730, 0730, 0730, 0730, 0730, 0151, 0};
GLOBAL TINY tyops[] {LSIZEOF, DADDR, DFNCALL, LPOINTS, DCAST, DINDIR, LNOT,
	LCOMP, DMINUS, DPLUS, DGPLU, DGMIN, DFNONLY,
	DELIST, LQUERY, LGETS, LGPLU, LGMIN, LGTIM, LGDIV, LGMOD,
	LGAND, LGXOR, LGOR, LGLSH, LGRSH, LPLUS, LMINUS,
	LTIMES, LDIVIDE, LMODULO, LAND, LOR, LXOR, LLSHIFT, LRSHIFT,
	LLESS, LLEQ, LGREAT, LGEQ, LISEQ, LNOTEQ, LANDAND, LOROR, DGETS, 0};

/*	check whether there is a meaningful expression
 */
VOID docheck(q)
	FAST TERM *q;
	{
	while (q->op == DELIST)
		q = q->e.o.right;
	switch (q->op)
		{
	case DGPLU:
		q->op = LGPLU;
		break;
	case DGMIN:
		q->op = LGMIN;
		break;
	case LQUERY:
	case DFNCALL:
	case DFNONLY:
	case LGETS:
	case LGPLU:
	case LGMIN:
	case LGTIM:
	case LGDIV:
	case LGMOD:
	case LGAND:
	case LGXOR:
	case LGOR:
	case LGLSH:
	case LGRSH:
		break;
	default:
		perror("useless expression");
		}
	}

/*	get maximum legal type
 */
BITS maxty(lty, rty, mty)
	BITS lty, rty, mty;
	{
	IMPORT TINY typtab[];
	FAST COUNT il, ir, im;

	il = scnstr(typtab, lty);
	ir = scnstr(typtab, rty);
	im = scnstr(typtab, mty);
	if (im < il || im < ir)
		perror("illegal operand type");
	else if (il < ir)
		im = ir;
	else
		im = il;
	return (typtab[im]);
	}

/*	pointify q according to the type of ty,at
 */
TERM *ptify(q, ty, at)
	TERM *q;
	BITS ty;
	ATTR *at;
	{
	IMPORT BITS tint, tunsign;
	IMPORT LONG bytes();

	if (tunsign < q->ty)
		setty(q = buyop(DCAST, q, NULL), tunsign, NULL);
	return (setty(buyop(LTIMES, q, buyterm(tint, NULL, NULL,
		bytes(dety(ty), at), 0, 0)), tint, NULL));
	}

/*	make q into a scalar
 */
TERM *scalify(q)
	FAST TERM *q;
	{
	FAST TERM *l;

	if (!q)
		return (NULL);
	switch (type(q->ty))
		{
	case TARRAY:
		setty(q, q->ty & ~3 | TPTRTO, q->at->next);
		break;
	case TFNRET:
		retype(q, TPTRTO);
		break;
	case TSTRUCT:
	case TUNION:
		perror("illegal structure reference");
		break;
	default:
		return (q);
		}
	if (!q->op)
		--q->e.v.refs;
	else
		{
		q->e.o.left = l = buyop(q->op, q->e.o.left, q->e.o.right);
		q->op = DADDR;
		l->e.o.mid = q->e.o.mid;
		l->ty = TPTRTO|TCHAR<<2;
		}
	return (q);
	}

/*	typify a function call
 */
VOID tfn(q)
	FAST TERM *q;
	{
	IMPORT BITS tint;
	FAST TERM *l, *r;
	INTERN ATTR fnattr;

	l = q->e.o.left;
	r = q->e.o.right;
	if (!l->op && !l->ty)
		setty(l, TFNRET | tint << 2, &fnattr);
	else if (type(typify(l)->ty) != TFNRET)
		perror("function required");
	else if (stype(dety(l->ty)))
		setty(q, maxify(dety(l->ty)), l->at->next);
	else
		perror("illegal return type");
	for (; r; r = r->e.o.right)
		if (r->op != DALIST)
			{
			reduce(scalify(typify(r)));
			break;
			}
		else
			reduce(scalify(typify(r->e.o.left)));
	}

/*	typify the pointsat operator
 */
VOID tpoints(l, r, q)
	FAST TERM *l, *r;
	TERM *q;
	{
	IMPORT BOOL mflag;
	IMPORT SYMBOL intsym, *mostab;
	FAST SYMBOL *p;
	SYMBOL *tab;

	tab = mostab;
	if (mflag && (l->ty == (TPTRTO|TSTRUCT<<2) || l->ty == (TPTRTO|TUNION<<2)))
		tab = l->at->a.stab->s.tagtab;
	else if (mflag)
		perror("illegal member");
	else if (itype(l->ty))
		{
		l = setty(buyop(DCAST, l, NULL), TPTRTO|TCHAR<<2, NULL);
		q->e.o.left = l;
		}
	else if (!ptype(l->ty))
		perror("illegal selection");
	if (!r->n.an[0])
		p = &intsym;
	else if (!(p = lookup(r->n.an, tab, NULL)))
		{
		nmerr("unknown member", r->n.an);
		p = &intsym;
		}
	q->e.o.left = setty(buyop(LPLUS, l, r), TPTRTO|TCHAR<<2, NULL);
	q->op = DINDIR;
	setty(q, p->ty, p->at);
	setad(r, NULL, p->s.offset, 0, 0);
	}

/*	determine type of ?:
 */
VOID tquery(q)
	FAST TERM *q;
	{
	IMPORT BITS tint;
	IMPORT LONG bytes();
	FAST TERM *l, *r;

	r = q->e.o.right;
	l = scalify(typify(q->e.o.mid));
	if (ptype(l->ty) && itype(r->ty))
		setty(q, l->ty, l->at);
	else if (itype(l->ty) && ptype(r->ty))
		setty(q, r->ty, r->at);
	else if (ptype(l->ty) && ptype(r->ty))
		setty(q, l->ty, l->at);
	else
		q->ty = maxty(l->ty, r->ty, TDOUBLE);
	q->ty = maxify(q->ty);
	if (l->ty != q->ty)
		q->e.o.mid = setty(buyop(DCAST, l, NULL), q->ty, q->at);
	if (r->ty != q->ty)
		q->e.o.right = setty(buyop(DCAST, r, NULL), q->ty, q->at);
	}

/*	determine type of expression
 */
TERM *typify(q)
	FAST TERM *q;
	{
	IMPORT BITS tfield, tint, tycodes[];
	IMPORT BOOL decflag;
	IMPORT LONG bytes();
	IMPORT TINY tyops[];
	FAST TERM *l, *r;
	COUNT code;

	if (!q)
		return (NULL);
	l = q->e.o.left;
	r = q->e.o.right;
	code = tycodes[scnstr(tyops, q->op)];
	if (code & 0200)
		typify(r);
	if (code & 0100)
		scalify(r);
	if (code & 0020)
		typify(l);
	if (code & 0010)
		scalify(l);
	if (code & 0040)
		if (!(l->op == DINDIR || l->op == LPOINTS ||
			!l->op && (l->e.v.refs || (l->e.v.idx && !(l->e.v.idx & AUTIDX)))))
			perror("lvalue required");
	if (code & 0004)
		q->ty = maxty(l->ty, r->ty, TDOUBLE);
	if (code & 0002)
		q->ty = maxty(l->ty, r->ty, TULONG);
	if (code & 0001)
		setty(q, l->ty, l->at);
	switch (q->op)
		{
	case 0:
		if (!q->ty && !decflag)
			nmerr("undeclared", q->n.an);
		break;
	case LSIZEOF:
		setad(q, NULL, bytes(l->ty, l->at), 0, 0);
		break;
	case DADDR:
		if (type(l->ty) == tfield ||
			!(l->op == DINDIR || l->op == LPOINTS ||
			!l->op && l->e.v.refs))
			perror("illegal &");
		retype(q, TPTRTO);
		break;
	case DFNCALL:
	case DFNONLY:
		tfn(q);
		break;
	case LPOINTS:
		tpoints(l, r, q);
		break;
	case DCAST:
		if (!stype(q->ty))
			{
			perror("illegal cast");
			setty(q, tint, NULL);
			}
		break;
	case DINDIR:
		if (type(l->ty) != TPTRTO)
			perror("illegal indirection");
		else
			setty(q, dety(l->ty), l->at);
		break;
	case LCOMP:
		if (!itype(q->ty))
			perror("integer type required");
		break;
	case DMINUS:
	case DPLUS:
		if (!itype(q->ty) && !dtype(q->ty))
			perror("arithmetic type required");
		break;
	case DELIST:
		docheck(l);
		setty(q, r->ty, r->at);
		break;
	case LQUERY:
		tquery(q);
		break;
	case DGETS:
		q->op = LGETS;
	case LGETS:
		if (ptype(l->ty) && dtype(r->ty) || (itype(l->ty) || dtype(l->ty))
			&& ptype(r->ty))
			perror("illegal assignment");
		break;
	case DGPLU:
	case DGMIN:
		if (dtype(q->ty))
			perror("illegal postop");
	case LGPLU:
	case LGMIN:
		if (ptype(l->ty) && itype(r->ty))
			q->e.o.right = ptify(r, l->ty, l->at);
		else if (ptype(l->ty) || ptype(r->ty))
			perror("illegal =+");
		break;
	case LPLUS:
		if (ptype(l->ty) && itype(r->ty))
			{
			q->e.o.right = ptify(r, l->ty, l->at);
			setty(q, l->ty, l->at);
			}
		else if (itype(l->ty) && ptype(r->ty))
			{
			q->e.o.left = ptify(l, r->ty, r->at);
			setty(q, r->ty, r->at);
			}
		else
			q->ty = maxty(l->ty, r->ty, TDOUBLE);
		break;
	case LMINUS:
		if (ptype(l->ty) && itype(r->ty))
			{
			q->e.o.right = ptify(r, l->ty, l->at);
			setty(q, l->ty, l->at);
			}
		else if (ptype(l->ty) && ptype(r->ty) &&
			bytes(dety(l->ty), l->at) == bytes(dety(r->ty), r->at))
			{
			q->e.o.left = setty(buyop(LMINUS, l, r), tint, NULL);
			q->e.o.right = buyterm(tint, NULL, NULL,
				bytes(dety(l->ty), l->at), 0, 0);
			q->op = LDIVIDE;
			q->ty = tint;
			}
		else
			q->ty = maxty(l->ty, r->ty, TDOUBLE);
		break;
	case LLESS:
	case LGEQ:
	case LLEQ:
	case LGREAT:
		if (q->op == LLESS || q->op == LGEQ)
			untest(l, r);
		else
			untest(r, l);
	case LISEQ:
	case LNOTEQ:
		if (ptype(l->ty) && itype(r->ty) || itype(l->ty) && ptype(r->ty)
			|| ptype(l->ty) && ptype(r->ty))
			;
		else if (ptype(l->ty) || ptype(r->ty))
			perror("illegal comparison");
		break;
		}
	if (!q->ty)
		q->ty = tint;
	else if (code & 0400)
		q->ty = maxify(q->ty);
	return (q);
	}

/*	test for reasonable unsign compare
 */
VOID untest(l, r)
	FAST TERM *l, *r;
	{
	if (iscons(r) && r->e.v.bias == 0L && (ptype(l->ty) || l->ty == TUCHAR
		|| l->ty == TUSHORT || l->ty == TULONG))
		perror("illegal unsigned compare");
	}
