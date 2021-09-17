/*	EXPRESSION PARSER FOR PASS 1
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int12.h"
#include "int012.h"

/*	the binary operator tables
 */
LOCAL TINY binops[] {LTIMES, LDIVIDE, LMODULO, LPLUS, LMINUS, LLSHIFT, LRSHIFT,
	LLESS, LLEQ, LGREAT, LGEQ, LISEQ, LNOTEQ,
	LAND, LXOR, LOR, LANDAND, LOROR, LQUERY,
	LGETS, LGTIM, LGDIV, LGMOD, LGPLU, LGMIN, LGLSH, LGRSH,
	LGAND, LGXOR, LGOR, 0};
LOCAL TINY getsops[] {LGTIM, LGDIV, LGMOD, LGPLU, LGMIN, LGLSH, LGRSH,
	0, 0, 0, 0, 0, 0,
	LGAND, LGXOR, LGOR, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0};
LOCAL TINY lpri[] {14, 14, 14, 13, 13, 12, 12,
	11, 11, 11, 11, 10, 10,
	9, 8, 7, 6, 5, 3,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1};
LOCAL TINY rpri[] {14, 14, 14, 13, 13, 12, 12,
	11, 11, 11, 11, 10, 10,
	9, 8, 7, 6, 5, 4,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2};

/*	the unary operator tables
 */
LOCAL TINY untoks[] {LTIMES, LAND, LPLUS, LMINUS, LNOT, LCOMP, 0};
LOCAL TINY unops[] {DINDIR, DADDR, DPLUS, DMINUS, LNOT, LCOMP};

/*	build a constant and compile time evaluate
 */
LONG const(mand)
	BOOL mand;
	{
	FAST TERM *q;

	if (!(q = gexpr(mand)))
		return (0L);
	else if (!iscons(q))
		{
		perror("constant required");
		return (0L);
		}
	else
		return (q->e.v.bias);
	}

/*	get an elist
 */
TERM *gelist(mand)
	BOOL mand;
	{
	IMPORT TERM *exlist;

	exlist = frelst(exlist, NULL);
	return (reduce(scalify(typify(melist(mand)))));
	}

/*	get an expression
 */
TERM *gexpr(mand)
	BOOL mand;
	{
	IMPORT TERM *exlist;

	exlist = frelst(exlist, NULL);
	return (reduce(scalify(typify(mexpr(mand)))));
	}

/*	get a parenthesised elist
 */
TERM *gtest(mand)
	BOOL mand;
	{
	FAST TERM *q;

	need(LLPAREN);
	q = gelist(mand);
	need(LRPAREN);
	return (q);
	}

/*	make a binary operator
 */
TERM *mbin()
	{
	IMPORT TINY binops[], getsops[];
	FAST TERM *q;
	FAST LEX gop, op;
	TOKEN tok;

	if (!(op = alt(binops)))
		return (NULL);
	else if (op == LQUERY)
		{
		q = buyop(LQUERY, NULL, NULL);
		q->e.o.mid = mexpr(YES);
		need(LCOLON);
		return (q);
		}
	else if (gettok(&tok)->type != LGETS)
		baktok(&tok);
	else if (gop = getsops[scnstr(binops, op)])
		op = gop & BYTMASK;
	else
		baktok(&tok);
	return (buyop(op, NULL, NULL));
	}

/*	make a cast
 */
TERM *mcast()
	{
	FAST SYMBOL *p;
	SYMBOL proto;

	if (!gscty(&proto, 0))
		return (NULL);
	else
		{
		p = gdecl(&proto, YES);
		need(LRPAREN);
		return (free(p, setty(buyop(DCAST, NULL, NULL), p->ty, p->at)));
		}
	}

/*	make an expr, expr list
 */
TERM *melist(mand)
	BOOL mand;
	{
	FAST TERM *q;

	if (!(q = mexpr(mand)))
		return (NULL);
	else
		{
		while (eat(LCOMMA))
			q = buyop(DELIST, q, mexpr(YES));
		return (q);
		}
	}

/*	make an expression from tokens
 */
TERM *mexpr(mand)
	BOOL mand;
	{
	FAST TERM *q;
	TERM *r;

	if (!(q = mterm(mand)))
		return (NULL);
	else if (r = mbin())
		return (mtail(0, q, &r));
	else
		return (q);
	}

/*	make a term from an identifier
 */
TERM *mident(s)
	TEXT *s;
	{
	IMPORT BITS tint;
	IMPORT SYMBOL *symtab;
	FAST SYMBOL *p;
	FAST TERM *q;

	if (!(p = lookup(s, symtab, NULL)))
		{
		p = buysym(0, NULL, s, LEXTERN);
		p->next = symtab, symtab = p;
		}
	q = buyterm(p->ty, p->at, NULL, 0L, 0, 1);
	switch (p->sc)
		{
	case LEXTERN:
	case DEXTERN:
	case LSTATIC:
	case DSTATIC:
		cpynm(q->n.an, p->n.an);
		p->s.reg = YES;
		break;
	case FSTATIC:
		cpynm(q->n.an, lblname(p->s.label));
		break;
	case DARG:
		q->e.v.bias = p->s.offset;
		q->e.v.idx = ARGIDX;
		break;
	case LAUTO:
		q->e.v.bias = p->s.offset;
		q->e.v.idx = AUTIDX;
		break;
	case LREG:
		q->e.v.idx = p->s.reg;
		q->e.v.refs = 0;
		break;
	default:
		nmerr("illegal use of typedef", p->n.an);
		setty(q, tint, NULL);
		}
	return (q);
	}

/*	make an expression from binop tail
 */
TERM *mtail(lpr, lt, pop)
	COUNT lpr;
	FAST TERM *lt;
	TERM **pop;
	{
	IMPORT TINY binops[], lpri[], rpri[];
	FAST TERM *rt;
	FAST COUNT i, mpr;
	TERM *rop;

	for (rop = *pop; rop && lpr < rpri[i = scnstr(binops, rop->op)]; *pop = rop)
		{
		mpr = lpri[i];
		rt = mterm(YES);
		if ((rop = mbin()) && mpr < rpri[scnstr(binops, rop->op)])
			rt = mtail(mpr, rt, &rop);
		(*pop)->e.o.left = lt, lt = *pop;
		(*pop)->e.o.right = rt;
		}
	return (lt);
	}

/*	make a term
 */
TERM *mterm(mand)
	BOOL mand;
	{
	IMPORT BITS tchar, tint;
	IMPORT LITERAL *littab;
	IMPORT LONG bytes();
	IMPORT TINY untoks[], unops[];
	FAST TERM *q;
	FAST LITERAL *l;
	TOKEN tok;
	INTERN TINY lty[] {LCNUM, LUCNUM, LSNUM, LUSNUM, LLNUM, LULNUM};
	INTERN TINY tty[] {TCHAR, TUCHAR, TSHORT, TUSHORT, TLONG, TULONG};

	switch (gettok(&tok)->type)
		{
	case LIDENT:
		q = mident(tok.t.an);
		break;
	case LUSNUM:
		if (TUSHORT < tint)
			tok.type = LLNUM;
	case LCNUM:
	case LUCNUM:
	case LSNUM:
	case LLNUM:
	case LULNUM:
		q = buyterm(tty[scnstr(lty, tok.type)], NULL, NULL, tok.t.ln, 0, 0);
		break;
	case LSTRING:
		l = alloc(sizeof (*l), littab), littab = l;
		l->e.l.next = tok.t.x.sptr;
		l->e.l.a.m = tok.t.x.slen + 1;
		setty(l, TARRAY | (tchar << 2), &l->e.l);
		l->sc = FSTATIC;
		cpynm(l->n.an, lblname(crs()));
		q = buyterm(l->ty, l->at, l->n.an, 0L, 0, 1);
		break;
	case LDNUM:
		q = buyterm(TDOUBLE, NULL, &tok.t.dn, 0L, 0, 0);
		break;
	case LTIMES:
	case LAND:
	case LPLUS:
	case LMINUS:
	case LNOT:
	case LCOMP:
		return (buyop(unops[scnstr(untoks, tok.type)] & BYTMASK,
			mterm(YES), 0));
	case LINCR:
	case LDECR:
		return (buyop((tok.type == LINCR) ? LGPLU : LGMIN,
				mterm(YES), buyterm(tint, NULL, NULL, 1L, 0, 0)));
	case LLPAREN:
		if (q = mcast())
			q->e.o.left = mterm(YES);
		else
			{
			q = melist(YES);
			need(LRPAREN);
			}
		break;
	case LSIZEOF:
		if (gettok(&tok)->type == LLPAREN && (q = mcast()))
			{
			setad(q, NULL, bytes(q->ty, q->at), 0, 0);
			setty(q, tint, NULL);
			}
		else
			{
			baktok(&tok);
			q = buyop(LSIZEOF, mterm(YES), 0);
			}
		break;
	default:
		baktok(&tok);
		if (mand)
			{
			perror("missing expression");
			q = buyterm(tint, NULL, NULL, 0L, 0, 0);
			}
		else
			return (NULL);
		}
	return (mtrail(q));
	}

/*	parse trailing operators of term
 */
TERM *mtrail(q)
	FAST TERM *q;
	{
	IMPORT BITS tint;
	IMPORT TERM *buyop();
	FAST TERM *r;
	TOKEN tok;

	FOREVER
		switch (gettok(&tok)->type)
			{
		case LLPAREN:
			q = buyop(DFNCALL, q, mexpr(NO));
			if (q->e.o.right)
				for (r = q; eat(LCOMMA); r = r->e.o.right)
					r->e.o.right = buyop(DALIST, r->e.o.right, mexpr(YES));
			else
				q->op = DFNONLY;
			need(LRPAREN);
			break;
		case LLBRACK:
			q = buyop(DINDIR, buyop(LPLUS, q, melist(YES)), NULL);
			need(LRBRACK);
			break;
		case LPOINTS:
		case LDOT:
			q = buyop(LPOINTS, (tok.type == LPOINTS) ? q :
				buyop(DADDR, q, NULL), NULL);
			if (!ident(&tok))
				{
				perror("missing member name");
				tok.t.an[0] = '\0';
				}
			q->e.o.right = buyterm(tint, NULL, tok.t.an, 0L, 0, 0);
			break;
		case LDECR:
		case LINCR:
			q = buyop((tok.type == LINCR) ? DGPLU : DGMIN, q,
				buyterm(tint, NULL, NULL, 1L, 0, 0));
			break;
		default:
			baktok(&tok);
			return (q);
			}
	}
