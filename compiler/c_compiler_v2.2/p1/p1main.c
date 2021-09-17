/*	STARTUP CODE FOR C PASS I
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"
#include "int12.h"
#include "int012.h"

/*	FLAGS:
	-a	addresses and data are in different registers [yuk]
	-b#	highest enforced bound
	-c	only one case significant on externals
	-d	put non initialized data in separate class
	-e	don't put undef'd and unref'd externs
	-l	int size is 4, not 2
	-m	structure names are local
	-n#	number of significant chars in externals
	-o	output file, errors to STDOUT
	-r#	no of regs [0,7]
	-u	char strings are unsigned chars
 */
BITS iregs {REGSET};
BITS tchar {TCHAR};
BITS tfield {TSFIELD};
BITS tint {TSHORT};
BITS tunsign {TUSHORT};
BOOL aflag {NO};
BOOL cflag {NO};
BOOL dflag {NO};
BOOL eflag {NO};
BOOL mflag {NO};
BOOL uflag {NO};
BYTES bitswd {16};
BYTES bndef {1};
BYTES intsize {2};
BYTES nlen {7};

TEXT *_pname {"p1"};

/*	FILE CONTROL:
	errfd = error file
	ofile = output file
	nerrors  = the number of errors.
	infile = the current file name.
	lineno = the current line number.
 */
GLOBAL FILE errfd {STDERR};
GLOBAL TEXT *ofile {NULL};
GLOBAL COUNT nerrors {0};
GLOBAL TEXT *infile {NULL};
GLOBAL COUNT lineno {0};
GLOBAL FILE outfd {STDOUT};

/*	SYMBOL TABLES:
	casetab = the case table
	exlist = the root of the expression list
	lbltab = the label symbol table
	littab = the string and double tables
	mostab = combined structure/union table
	strtab = the structure tag table
	symend = current symbol table end
	symtab = current symbol table start
	untab = the union tag table
 */
GLOBAL CASE *casetab {NULL};
GLOBAL TERM *exlist {NULL};
GLOBAL SYMBOL *lbltab {NULL};
GLOBAL LITERAL *littab {NULL};
GLOBAL SYMBOL *mostab {NULL};
GLOBAL SYMBOL *strtab {NULL};
GLOBAL SYMBOL *symend {NULL};
GLOBAL SYMBOL *symtab {NULL};
GLOBAL SYMBOL *untab {NULL};

/*	compare the types of two symtab entries
 */
BOOL cmptype(p, q)
	SYMBOL *p, *q;
	{
	FAST ATTR *at1, *at2;
	FAST BITS ty1;
	BITS ty2;

	ty1 = p->ty, ty2 = q->ty;
	at1 = p->at, at2 = q->at;
	for (; (ty1 & 3) && (ty1 & 3) == (ty2 & 3);
		ty1 = dety(ty1), ty2 = dety(ty2))
		if ((ty1 & 3) == TPTRTO)
			;
		else if ((ty1 & 3) == TFNRET)
			at1 = at1->next, at2 = at2->next;
		else if (at1->a.m && at2->a.m && (at1->a.m != at2->a.m))
			return (NO);
		else
			at1 = at1->next, at2 = at2->next;
	if (ty1 != ty2)
		return (NO);
	else if (ty1 != TSTRUCT && ty1 != TUNION)
		return (YES);
	else
		return (at1->a.stab == at2->a.stab);
	}

/*	structure union parsing
 */
ATTR *decsu(lty)
	LEX lty;
	{
	IMPORT BITS tfield, tint, tunsign;
	IMPORT BOOL mflag;
	IMPORT BYTES bitswd;
	IMPORT LONG bndify(), bytes(), const();
	IMPORT SYMBOL *mostab, *strtab, *untab;
	FAST COUNT boff, bsize;
	FAST SYMBOL *p;
	BOOL ffld;
	LONG lastoff, suoff;
	SYMBOL proto, *q, **qb, *tbl;
	TOKEN tag;

	tbl = NULL;
	qb = &tbl;
	suoff = 0L;
	lastoff = 0L;
	boff = 0;
	ident(&tag);
	if (eat(LLCURLY))
		while (!eat(LRCURLY) && gscty(&proto, 0))
			{
			while ((p = gdecl(&proto, NO)) || peek(LCOLON))
				{
				if (!p)
					{
					p = buysym(tint, NULL, NULL, 0);
					*qb = p;
					qb = &p->next;
					}
				else if (!(q = lookup(p->n.an, tbl, NULL)))
					{
					*qb = p;
					qb = &p->next;
					}
				else
					{
					nmerr("member redefined", p->n.an);
					p = free(p, q);
					}
				if (!eat(LCOLON))
					{
					boff = 0;
					ffld = YES;
					}
				else
					{
					if (lty != LSTRUCT)
						perror("illegal field");
					bsize = const(YES);
					if (p->ty != tint && p->ty != tunsign)
						perror("illegal bitfield");
					p->ty = tfield;
					if (bsize < 0 || bitswd < bsize)
						{
						perror("bad field width");
						bsize = bitswd;
						}
					p->at = buysat(NULL, NULL);
					p->at->a.b.bsize = bsize;
					if (bsize == 0)
						{
						boff = 0;
						ffld = NO;
						}
					else if (boff == 0 || bitswd < (bsize + boff))
						{
						p->at->a.b.boff = 0;
						boff = bsize;
						ffld = YES;
						}
					else
						{
						p->at->a.b.boff = boff;
						boff =+ bsize;
						ffld = NO;
						}
					}
				if (lty == LUNION)
					p->s.offset = 0L;
				else if (!ffld)
					p->s.offset = lastoff;
				else
					{
					p->s.offset = bndify(p->ty, p->at, suoff);
					suoff = p->s.offset + bytes(p->ty, p->at);
					lastoff = p->s.offset;
					}
				if (!eat(LCOMMA))
					break;
				}
			need(LSCOLON);
			}
	*qb = NULL;
	if (!mflag)
		for (p = tbl; p; p = p->next)
			if (!(q = lookup(p->n.an, mostab, NULL)))
				{
				q = buysym(p->ty, p->at, p->n.an, 0);
				q->s.offset = p->s.offset;
				q->next = mostab, mostab = q;
				}
			else if (!cmptype(p, q)
				|| p->s.offset != q->s.offset)
				nmerr("member conflict", p->n.an);
	if (!tag.type)
		{
		p = buysym(0, NULL, NULL, 0);
		p->s.tagtab = tbl;
		if (!tbl)
			perror("no structure definition");
		}
	else if (!(p = lookup(tag.t.an,
			(lty == LSTRUCT) ? strtab : untab, NULL)))
		{
		p = buysym(0, NULL, tag.t.an, 0);
		if (lty == LSTRUCT)
			p->next = strtab, strtab = p;
		else
			p->next = untab, untab = p;
		p->s.tagtab = tbl;
		}
	else if (tbl && !p->s.tagtab)
		p->s.tagtab = tbl;
	else if (!tbl && p->s.tagtab)
		tbl = p->s.tagtab;
	else if (tbl && p->s.tagtab)
		nmerr("redefined tag", tag.t.an);
	return (buysat(p, NULL));
	}

/*	parse the declaration term
 */
SYMBOL *dterm(pro, abstract)
	FAST SYMBOL *pro;
	BOOL abstract;
	{
	IMPORT LONG const();
	FAST ATTR **qb;
	FAST SYMBOL *p, *r;
	LEX lex;
	SYMBOL *table;
	TOKEN tok;
	INTERN TINY endalt[] {LLPAREN, LLBRACK, 0};

	if (eat(LTIMES))
		return (retype(dterm(pro, abstract), TPTRTO));
	else if ((lex = eat(LLPAREN)) && (!abstract || !(lex = eat(LRPAREN))))
		{
		if (!(p = dterm(pro, abstract)))
			{
			perror("bad (declaration)");
			p = buysym(pro->ty, pro->at, pro->n.an, pro->sc);
			}
		need(LRPAREN);
		}
	else if (lex == LRPAREN)
		{
		tok.type = LRPAREN;
		baktok(&tok);
		tok.type = LLPAREN;
		baktok(&tok);
		p = buysym(pro->ty, pro->at, pro->n.an, pro->sc);
		}
	else
		{
		ident(&tok);
		if (!tok.type && !abstract)
			return (NULL);
		else
			{
			if (tok.type && abstract)
				nmerr("identifier not allowed", tok.t.an);
			p = buysym(pro->ty, pro->at, pro->n.an, pro->sc);
			cpynm(p->n.an, tok.t.an);
			}
		}
	for (qb = &p->at; *qb; qb = &(*qb)->next)
		;
	for (; lex = alt(endalt); qb = &(*qb)->next)
		if (lex == LLPAREN)
			{
			table = NULL;
			if (ident(&tok))
				for (table = r = buysym(0, NULL, tok.t.an, 0); eat(LCOMMA); )
					{
					if (!ident(&tok))
						perror("missing argument");
					r = r->next = buysym(0, NULL, tok.t.an, 0);
					}
			*qb = buysat(table, NULL);
			need(LRPAREN);
			retype(p, TFNRET);
			}
		else
			{
			*qb = buymat(const(NO), NULL);
			need(LRBRACK);
			retype(p, TARRAY);
			}
	return (p);
	}

/*	parse a declaration and flip the symbol table entry
 */
SYMBOL *gdecl(pro, abstract)
	SYMBOL *pro;
	BOOL abstract;
	{
	FAST ATTR **qb;
	FAST COUNT i;
	FAST SYMBOL *p;
	ATTR *at;
	BITS nty;

	at = pro->at, pro->at = NULL;
	p = dterm(pro, abstract);
	pro->at = at;
	if (!p)
		return (NULL);
	for (qb = &p->at; *qb; qb = &(*qb)->next)
		;
	*qb = at;
	for (i = 0, nty = p->ty; nty != pro->ty; ++i, nty =>> 2)
		;
	for (; 0 < i; --i, p->ty =>> 2)
		nty = nty << 2 | p->ty & 3;
	p->ty = nty;
	return (p);
	}

/*	get sc/ty into proto symbol
 */
BOOL gscty(pro, defsc)
	FAST SYMBOL *pro;
	LEX defsc;
	{
	IMPORT BITS tint, tunsign;
	IMPORT SYMBOL *symtab;
	FAST LEX sc, ty;
	ARGINT *r;
	LEX mod, uns;
	SYMBOL *p;
	TOKEN tok;
	INTERN BITS tutok[] {TUCHAR, TUSHORT, TULONG, 0, 0,
			0, 0, 0};
	INTERN BITS tytok[] {TCHAR, TSHORT, TLONG, TFLOAT, TDOUBLE,
			TSTRUCT, TUNION, 0};
	INTERN TINY tylex[] {LCHAR, LSHORT, LLONG, LFLOAT, LDOUBLE,
			LSTRUCT, LUNION, 0};
	INTERN TINY basalt[] {LINT, LFLOAT, LDOUBLE, 0};
	INTERN TINY modalt[] {LCHAR, LSHORT, LLONG, 0};
	INTERN TINY scalt[] {LEXTERN, LSTATIC, LAUTO, LREG, LTYPDEF, 0};
	INTERN TINY sualt[] {LSTRUCT, LUNION, 0};

	clrsym(pro);
	sc = alt(scalt);
	uns = eat(LUNSIGN);
	mod = alt(modalt);
	if (ty = alt(basalt))
		;
	else if (ty = alt(sualt))
		pro->at = decsu(ty);
	else if (ident(&tok) && (p = lookup(tok.t.an, symtab, NULL))
		&& p->sc == LTYPDEF)
		{
		setty(pro, p->ty, p->at);
		ty = LSTRUCT;	/* innocuous, non-zero */
		}
	else if (tok.type)
		baktok(&tok);
	for (r = &defsc; *r && *r != sc; ++r)
		;
	if (*r != sc)
		perror("illegal storage class");
	pro->sc = (sc) ? sc : defsc;
	if (pro->ty && !mod && !uns)
		return (YES);
	else if (!ty || ty == LINT)
		{
		if (mod)
			ty = mod;
		}
	else if (mod == LLONG && ty == LFLOAT && !uns)
		ty = LDOUBLE;
	else if (mod || uns)
		perror("illegal type modifier");
	if (!ty || ty == LINT)
		pro->ty = uns ? tunsign : tint;
	else if (!(pro->ty = (uns ? tutok : tytok)[scnstr(tylex, ty)]))
		pro->ty = tint;
	return (sc || ty || uns || mod);
	}

/*	parse external declarations
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BITS iregs, tchar;
	IMPORT BOOL aflag, cflag, dflag, eflag, mflag, uflag;
	IMPORT BYTES nlen;
	IMPORT COUNT nerrors;
	IMPORT FILE errfd, outfd;
	IMPORT SYMBOL *symtab;
	IMPORT TEXT *infile, *ofile;
	FAST BYTES i;
	FAST SYMBOL *p, *q;
	BOOL prog;
	SYMBOL proto;
	INTERN BITS ir[8]
	/*   0  r1   r2   r3   r4/a1  r5/a2   r6/a3   r7		f(REGSET) */
		{0, 004, 014, 034, 04034, 014034, 034034, 034234};
	INTERN BYTES nregs {3};

	infile = buybuf("", 1);
	getflags(&ac, &av, "a,c,d,e,l,m,n#,o*,b#,r#,u:F <file>",
		&aflag, &cflag, &dflag, &eflag, &intsize, &mflag,
		&nlen, &ofile, &bndef, &nregs, &uflag);
	if (intsize != 2)
		fixlint();
	bndef =& 03;
	if (uflag)
		tchar = TUCHAR;
	if (aflag)
		nregs =+ 3;
	iregs = ir[nregs & 7];
	if (ofile)
		if ((outfd = create(ofile, WRITE, 1)) < 0)
			{
			perror("bad output file");
			exit(NO);
			}
		else
			errfd = STDOUT;
	if (0 < ac)
		{
		close(STDIN);
		if (open(av[0], READ, 1) != STDIN)
			{
			perror("bad input file!");
			exit(NO);
			}
		}
	for (nerrors = 0; !eat(EOF); )
		{
		prog = gscty(&proto, LEXTERN, LSTATIC, LTYPDEF, 0);
		while (p = gdecl(&proto, NO))
			{
			prog = YES;
			if (!(q = lookup(p->n.an, symtab, NULL)))
				p->next = symtab, symtab = p;
			else if (p->sc == LTYPDEF || q->sc == LTYPDEF)
				{
				nmerr("redeclared typedef", p->n.an);
				q = NULL;
				}
			else if (q->ty && !cmptype(p, q))
				{
				nmerr("redeclared external", p->n.an);
				q = NULL;
				}
			else if (p->sc == LSTATIC && q->sc == LEXTERN)
				q->sc = LSTATIC;
			else if (p->sc == LSTATIC && q->sc == DEXTERN)
				q->sc = DSTATIC;
			if (q)
				{
				if (!q->ty)
					setty(q, p->ty, p->at);
				else if (type(q->ty) == TARRAY)
					{
					if (!q->at->a.m)
						q->at->a.m = p->at->a.m;
					}
				else if (type(q->ty) == TFNRET)
					q->at->a.stab = p->at->a.stab;
				p = free(p, q);
				}
			if (p->sc == LTYPDEF)
				;
			else if (type(p->ty) != TFNRET)
				datinit(p);
			else if (fninit(p))
				break;
			if (!eat(LCOMMA))
				break;
			}
		if (!eat(LSCOLON) && !prog)
			recover("bad external syntax");
		}
	for (q = symtab; q; q = q->next)
		switch(q->sc)
			{
		case LEXTERN:
			if (!eflag || q->s.reg)
				pref(q->n.an);
			exchk(q);
			break;
		case DEXTERN:
			pdef(q->n.an);
		case DSTATIC:
			exchk(q);
			break;
		case LSTATIC:
			nmerr("undefined static", q->n.an);
			}
	putch(-1);
	return (nerrors == 0);
	}
