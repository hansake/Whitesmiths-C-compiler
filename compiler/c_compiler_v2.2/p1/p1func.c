/*	FUNCTION INITIALIZING ROUTINES, PART 1
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int12.h"
#include "int012.h"

/*	allocation resources
 */
GLOBAL BITS regset {REGSET};
GLOBAL LONG autoff {0};

/*	case tables
 */
GLOBAL LABEL deflbl {0};

/*	the return term
 */
GLOBAL TERM rterm {NULL};
LOCAL TINY rettab[] {INTRET, INTRET, INTRET, INTRET, INTRET, LONGRET,
			LONGRET, LONGRET, FLTRET, FLTRET, INTRET};

/*	initialize an auto
 */
VOID autinit(p)
	FAST SYMBOL *p;
	{
	IMPORT BITS regset;
	IMPORT LONG autoff;
	FAST TERM *q;
	BOOL curly;
	TERM to;

	eat(LGETS);
	curly = eat(LLCURLY);
	if (q = gexpr(NO))
		{
		pregs(regset);
		pauto(autoff);
		if (p->sc == LAUTO)
			setad(setty(&to, p->ty, p->at), NULL, p->s.offset, AUTIDX, 1);
		else
			setad(setty(&to, p->ty, p->at), NULL, 0L, p->s.reg, 0);
		pmove(&to, q);
		}
	if (curly)
		need(LRCURLY);
	}

/*	parse a {block}
 */
VOID doblock(brk, cont)
	LABEL brk, cont;
	{
	IMPORT BITS regset;
	IMPORT LONG autoff, bytes();
	IMPORT SYMBOL *symtab;
	FAST SYMBOL *p, *q;
	FAST TINY reg;
	BOOL isfn;
	BITS regsave;
	LONG autsave;
	SYMBOL proto, *symsave;
	TOKEN tok;

	regsave = regset;
	autsave = autoff;
	symsave = symtab;
	while (gscty(&proto, LAUTO, LREG, LEXTERN, LSTATIC, LTYPDEF, 0))
		{
		while (p = gdecl(&proto, NO))
			{
			isfn = type(p->ty) == TFNRET && p->sc != LTYPDEF;
			if (isfn && p->sc != LSTATIC)
				p->sc = LEXTERN;
			if (q = lookup(p->n.an, symtab, symsave))
				nmerr("redeclared local", p->n.an);
			else if (!(p->sc == LEXTERN || isfn) ||
				!(q = lookex(p->n.an, symsave)))
				p->next = symtab, symtab = p;
			else if (!q->ty)
				setty(q, p->ty, p->at);
			else if (!cmptype(p, q))
				nmerr("redeclared", p->n.an);
			if (q)
				p = free(p, q);
			if (p->sc == LTYPDEF || isfn)
				;
			else if (p->sc == LSTATIC)
				{
				p->sc = FSTATIC;
				p->s.label = crs();
				datinit(p);
				}
			else if (p->sc == LREG || p->sc == LAUTO)
				{
				if (p->sc == LREG && (reg = rbuy(p->ty, &regset)))
					{
					p->s.reg = reg;
					p->ty = maxify(p->ty);
					}
				else
					{
					p->sc = LAUTO;
					autoff =- bytes(p->ty, p->at);
					autoff =& ~((1 << bound(p->ty, p->at)) - 1L);
					p->s.offset = autoff;
					}
				autinit(p);
				}
			if (!eat(LCOMMA))
				break;
			}
		need(LSCOLON);
		}
	 if (regset != regsave)
		pregs(regset);
	if (autoff != autsave)
		pauto(autoff);
	while (gettok(&tok)->type != LRCURLY)
		{
		baktok(&tok);
		if (tok.type == EOF)
			{
			perror("unexpected EOF");
			break;
			}
		else
			dostat(brk, cont);
		}
	if (regset != regsave)
		pregs(regset = regsave);
	if (autoff != autsave)
		{
		autoff = autsave;
		pauto(autoff);
		}
	perc(symsave);
	}

/*	parse a statement
 */
VOID dostat(brk, cont)
	LABEL brk, cont;
	{
	IMPORT BITS tint;
	IMPORT CASE *casetab;
	IMPORT LABEL deflbl;
	IMPORT LONG const();
	IMPORT SYMBOL *lbltab;
	IMPORT TERM rterm;
	FAST CASE *r;
	FAST SYMBOL *p;
	FAST TERM *q;
	LABEL top, bot, mid;
	INTERN BOOL done;	/* static to reduce recursion load */
	INTERN LABEL inc;
	INTERN TERM caseterm;
	INTERN TOKEN tok;

	for (done = NO; !done; )
		switch (gettok(&tok)->type)
			{
		case LCASE:
			casetab = alloc(sizeof (*casetab), casetab);
			casetab->cvalue = const(YES);
			need(LCOLON);
			for (r = casetab->next; r && r != &casetab; r = r->next)
				if (r->cvalue == casetab->cvalue)
					break;
			if (r != &casetab)
				perror("illegal case");
			casetab->clabel = pcase(crs());
			break;
		case LDFAULT:
			need(LCOLON);
			if (!casetab || deflbl)
				perror("illegal default");
			deflbl = pcase(crs());
			break;
		default:
			if (tok.type != LIDENT || !eat(LCOLON))
				done = YES;
			else
				{
				if (!(p = lookup(tok.t.an, lbltab, NULL)))
					{
					p = buysym(0, NULL, tok.t.an, 0);
					p->next = lbltab, lbltab = p;
					}
				if (p->sc)
					nmerr("redefined label", p->n.an);
				p->sc = FSTATIC;
				if (!p->s.label)
					p->s.label = crs();
				plabel(p->s.label);
				}
			}
	switch (tok.type)
		{
	case LLCURLY:
		doblock(brk, cont);
		break;
	case LSCOLON:
		break;
	case LIF:
		bot = pjf(gtest(NO), crs(), 0);
		dostat(brk, cont);
		if (eat(LELSE))
			{
			bot = pjump(0, bot);
			dostat(brk, cont);
			}
		plabel(bot);
		break;
	case LWHILE:
		top = plabel(crs());
		bot = pjf(gtest(NO), crs(), 0);
		dostat(bot, top);
		pjump(top, bot);
		break;
	case LDO:
		top = plabel(crs());
		dostat(bot = crs(), mid = crs());
		plabel(mid);
		need(LWHILE);
		pjt(gtest(NO), bot, top);
		need(LSCOLON);
		break;
	case LFOR:
		need(LLPAREN);
		if (q = gelist(NO))
			pvoid(q);
		need(LSCOLON);
		top = plabel(crs());
		bot = pjf(gelist(NO), crs(), 0);
		need(LSCOLON);
		if (q = gelist(NO))
			{
			mid = pjump(0, inc = crs());
			pvoid(q);
			pjump(top, mid);
			top = inc;
			}
		need(LRPAREN);
		dostat(bot, top);
		pjump(top, bot);
		break;
	case LBREAK:
		if (brk)
			pjump(brk, 0);
		else
			perror("illegal break");
		need(LSCOLON);
		break;
	case LCONTIN:
		if (cont)
			pjump(cont, 0);
		else
			perror("illegal continue");
		need(LSCOLON);
		break;
	case LRETURN:
		if (q = gelist(NO))
			pmove(&rterm, q);
		pret();
		need(LSCOLON);
		break;
	case LGOTO:
		if (!ident(&tok))
			perror("missing goto label");
		else if (p = lookup(tok.t.an, lbltab, NULL))
			pjump(p->s.label, 0);
		else
			{
			p = buysym(0, NULL, tok.t.an, 0);
			p->next = lbltab, lbltab = p;
			p->s.label = pjump(0, 0);
			}
		need(LSCOLON);
		break;
	case LSWITCH:
		caseterm.ty = tint;
		caseterm.e.v.idx = INTRET;
		pmove(&caseterm, gtest(YES));
		r = casetab, casetab = &casetab;
		top = deflbl, deflbl = 0;
		pswitch(mid = crs());
		dostat(bot = crs(), cont);
		pswtab(&casetab, deflbl ? deflbl : bot, mid);
		pcase(bot);
		deflbl = top;
		casetab = r;
		break;
	default:
		baktok(&tok);
		if (q = gelist(NO))
			{
			pvoid(q);
			need(LSCOLON);
			}
		else
			recover("illegal statement");
		}
	}

/*	parse function declaration
 */
BOOL fninit(pd)
	FAST SYMBOL *pd;
	{
	IMPORT BITS iregs, tint;
	IMPORT LONG bndify(), bytes();
	IMPORT SYMBOL *lbltab, *symtab;
	IMPORT TERM rterm;
	IMPORT TINY rettab[], typtab[];
	FAST SYMBOL *p, *q;
	BOOL defined;
	LONG argoff;
	SYMBOL *lcltab, proto, **qb, *symsave;
	TERM *pfn, to;
	TINY reg;

	defined = NO;
	lcltab = pd->at->a.stab;
	symsave = symtab;
	while (gscty(&proto, DARG, LREG, 0))
		{
		defined = YES;
		for (; (p = gdecl(&proto, NO)); free(p, NULL))
			{
			if (!(q = lookup(p->n.an, lcltab, NULL)))
				nmerr("not an argument", p->n.an);
			else if (q->sc)
				nmerr("redeclared argument", p->n.an);
			else
				setty(q, p->ty, p->at)->sc = p->sc;
			if (!eat(LCOMMA))
				break;
			}
		need(LSCOLON);
		}
	if (defined)
		need(LLCURLY);
	else if (eat(LLCURLY))
		defined = YES;
	if (!defined)
		;
	else if (pd->sc == DEXTERN || pd->sc == DSTATIC)
		nmerr("function redefined", pd->n.an);
	else
		{
		pd->sc = (pd->sc == LEXTERN) ? DEXTERN : DSTATIC;
		pfunc(pd->n.an);
		}
	if (defined)
		{
		setty(&rterm, maxify(dety(pd->ty)), pd->at->next);
		if (!stype(rterm.ty))
			{
			perror("illegal return type");
			rterm.ty = tint;
			}
		rterm.e.v.idx = rettab[scnstr(typtab, type(rterm.ty))];
		regset = iregs;
		autoff = 0;
		argoff = 0;
		for (qb = &lcltab; q = *qb; qb = &q->next)
			{
			if (!q->sc)
				setty(q, tint, DARG);
			else if ((q->ty & 03) == TFNRET)
				retype(q, TPTRTO);
			else if ((q->ty & 03) == TARRAY)
				setty(q, (q->ty & ~03) | TPTRTO, q->at->next);
			else
				q->ty = maxify(q->ty);
			q->s.offset = bndify(q->ty, q->at, argoff);
			argoff = q->s.offset + bytes(q->ty, q->at);
			if (q->sc == LREG && (reg = rbuy(q->ty, &regset)))
				{
				pfn = buyterm(q->ty, q->at, NULL, q->s.offset, ARGIDX, 1);
				setad(setty(&to, q->ty, q->at), NULL, 0L, reg, 0);
				pregs(regset);
				pauto(autoff);
				pmove(&to, pfn);
				q->s.reg = reg;
				}
			else
				q->sc = DARG;
			}
		*qb = symtab, symtab = lcltab;
		lbltab = NULL;
		doblock(0, 0);
		pret();
		for (q = lbltab; q; q = free(q, q->next))
			if (!q->sc)
				nmerr("missing label", q->n.an);
		perc(symsave);
		pend();
		}
	return (defined);
	}

/*	lookup an external variable
 */
SYMBOL *lookex(nm, p)
	TEXT *nm;
	FAST SYMBOL *p;
	{
	for ( ; p = lookup(nm, p, NULL); p = p->next)
		if (p->sc == LEXTERN || p->sc == DEXTERN ||
			p->sc == LSTATIC || p->sc == DSTATIC)
			return (p);
	return (NULL);
	}

/*	restitch symbol table and list of attributes
 */
VOID perc(symsave)
	FAST SYMBOL *symsave;
	{
	IMPORT SYMBOL *symtab;
	FAST SYMBOL *p, *q;

	for (q = symtab, symtab = symsave; q != symsave; )
		if (q->sc == LEXTERN || q->sc == LSTATIC)
			{
			p = q->next, q->next = symtab;
			symtab = q, q = p;
			}
		else
			q = free(q, q->next);
	}
