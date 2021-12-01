/*	INPUT AND UTILITY ROUTINES
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	set the correct argument offset
 */
BYTES argbias()
	{

	return (ARGBIAS);
	}

/*	build a format string
 */
TEXT *bldfmt(s, t, u, v)
	FAST TEXT *s;
	TEXT *t;
	BYTES u;
	TEXT *v;
	{
	return ((TEXT *)cpystr(s, t, nn((u & BYTMASK), 10), v, NULL));
	}

/*	compute bit field mask
 */
BITS bmask(ty)
	FAST TYPE ty;
	{
	return ((1 << (ty >> 10)) - 1 << (ty >> 4 & 077));
	}

/*	copy address structure
 */
VOID cpyad(pl, pr)
	FAST ADDR *pl, *pr;
	{
	cpybuf(pl, pr, sizeof (ADDR));		/* SHDB struct assignment */
	}

/*	convert local float or double to IBM-format
 */
VOID ibmflt(pd, dble)
	FAST TEXT *pd;
	BOOL dble;
	{
	FAST COUNT i, exp;
	
	exp = _fcan(pd);
	if (!(pd[1] & 0x80))
		;
	else
		{
		exp += (64 << 2);
		if (!dble)
			for (i = 4; i < 8; i++)
				pd[i] = 0;
		for (; exp & 0x3; exp++)
			{
			for (i = (dble) ? 7 : 3; i > 1; i--)
				pd[i] = ((pd[i] & BYTMASK) >> 1) | (pd[i-1] << 7);
			pd[i] = (pd[i] & BYTMASK) >> 1;
			}
		pd[0] |= (exp >> 2);
		}
	}

/*	choose default space bits selection
 */
SPACE defspace(space, idx)
	SPACE space;
	TINY idx;
	{
	SPACE ret;

	ret = space;
	if (space & SBUI)
		{
		ret = space & SBUI;
		}
	else if (space & SFAR)
		{
		ret = space & SFAR;
		}
	return (ret);
	}

/*	list lines up to the num
 */
VOID dolist(num)
	FAST COUNT num;
	{
	IMPORT BOOL keepline, lstflag, asmflag, sflag, upcase;
	IMPORT COUNT osect, lnmarker;
	IMPORT TEXT fname[];
	FAST COUNT i;
	COUNT savsect;
	INTERN TEXT lnbuf[] = {"          "};

	if (keepline || !lstflag || num <= lnmarker || !asmflag)
		return;
	savsect = osect;
	csect(ISTEXT);
	upcase = NO;
	putbytes("#line=", 6);
	putbytes(fname, lenstr(fname));
	putbytes(":", 1);
	i = itob(lnbuf, num, 10);
	putbytes(lnbuf, i);
	putchr('\n');
	upcase = YES;
	putchr(-1);
	csect(savsect);
	lnmarker = num;
	}

/*	init an expr
 */
EXPR *exzap(q)
	FAST EXPR *q;
	{
	q->op = 0;
	q->got = 0;
	q->name[0] = '\0';
	fill(&q->f, sizeof (ADDR), '\0');
	fill(&q->e, sizeof (q->e), '\0');
	return (q);
	}

/*	hang the fat stuff to the "middle leg" of the function returning
 */
EXPR *fatgfn(q)
	FAST EXPR *q;
	{
	FAST EXPR *l = q->e.o.left;

	if ((q->e.o.right->op == DFNCALL || q->e.o.right->op == DFNONLY) &&
		q->e.o.ty == q->e.o.right->e.o.ty &&
		(!l->op || l->op == DINDIR && l->e.o.left->e.v.ty == XUSHORT))
		{
		q->e.o.right->e.o.mid = addrof(q->e.o.left);
		q = q->e.o.right;
		}
	return (q);
	}

/*	set up the function returning structure
 */
VOID fatsfn(q, size)
	FAST EXPR *q;
	BYTES size;
	{
	IMPORT LONG autbuy();
	IMPORT TINY witype[];
	FAST EXPR *p;

	p = mexpr(0);
	q->e.o.mid = p;
	p->e.v.ty = YUSHORT;
	p->e.v.idx = XVXR1;
	p->e.v.pspace = defspace(0, XXR1);
	p->e.v.bias = autbuy(size);
	}

/*	release a header block
 */
HEADER *frehlst(q)
	FAST HEADER *q;
	{
	IMPORT HEADER *theader;
	FAST HEADER *p;

	p = q->next;
	q->next = theader, theader = q;
	return (p);
	}

/*	release a code block
 */
VOID freqlst(q)
	FAST CODE *q;
	{
	IMPORT CODE *tcode;
	FAST CODE *p;

	if (!q)
		return;
	for (p = q; p->next; p = p->next)
		;
	p->next = tcode, tcode = q;
	}

/*	does name has (\020\022\024)<
 */
TEXT funtype(s)
	FAST TEXT *s;
	{
	FAST BYTES n;

	if ((n = scnstr(s, '<')) && s[n] && scnstr("\020\022\024", s[n - 1]) < 3)
		return (s[n - 1]);
	else
		return ('\0');
	}

/*	classify address of fixed expression
 */
WANTS gotten(p)
	FAST EXPR *p;
	{
	FAST WANTS got;
	FAST TINY refs;
	TINY idx;

	refs = p->f.refs;
	idx = p->f.idx;
dbprint("/gotten %I %E", idx, p);
	if (1 < refs)
		{
		if (!idx || (idx == XXR1 && !p->f.nm[0]))
			got = WPMEM;
		else
			got = WTERM;
		}
	else if (refs)
		{
		if (!idx || idx == XXR2)
			got = WMEM;
		else if (idx == XXR1)
			got = (p->got & GVOL) ? WVMEM : WMEM;
		else if (idx == XAC0)
			got = WPMEM;
		else
			panic("GOTTEN: refs and F0");
		}
	else if (!idx)
		got = WIMM;
	else if (p->f.nm[0] || p->f.bias || idx == XXR1)
		got = WREREF;
	else if (idx == XXR2)
		got = (p->got & GVOL) ? WVXR2 : WXR2;
	else if (idx == XAC0)
		got = (p->got & GVOL) ? WVAC0 : WAC0;
	else
		panic("GOTTEN");
dbprint("/gotten-returns %W\n", got);
	return (got);
	}

/*	test for constant term
 */
BOOL iscons(p)
	FAST EXPR *p;
	{
	return (!p->op && (p->e.v.ty & TYPEMASK) <= YULONG &&
		!p->e.v.nm[0] && !p->e.v.idx && !p->e.v.refs);
	}

/*	test for adressable gotten
 */
BOOL gotadr(p)
	FAST EXPR *p;
	{
	IMPORT TINY bytype[];

dbprint("/gotadr %E", p);
	if ((p->f.refs == 1 &&
		(!p->f.idx ||
		!p->f.nm[0] && 
		0 <= p->f.bias && (p->f.bias + bytype[p->f.ty & TYPEMASK]-1) < 256  &&
		(p->f.idx == XXR1 || p->f.idx == XXR2))) ||
		(!p->f.refs && p->f.idx == XAC0 && !p->f.nm[0] && !p->f.bias))
		{
dbprint("/gotadr-return: YES\n");
		return (YES);
		}
	else
		{
dbprint("/gotadr-return: NO\n");
		return (NO);
		}
	}

/*  test of got pointer in XR2
 */
BOOL gpxr2(p)
	FAST EXPR *p;
	{
	return(p->f.idx == XXR2 && p->f.refs == 1 && !p->f.bias && !p->f.nm[0]);
	}

/*	test if in a register
 */
RESOURCE isreg(p)
	FAST EXPR *p;
	{
	return ((p->op || p->e.v.nm[0] || p->e.v.bias ||
		!p->e.v.idx || p->e.v.refs) ? 0 : xtor(p->e.v.idx));
	}

/*	make a name from label
 */
TEXT *lbln(label)
	FAST LABEL label;
	{
	FAST TEXT *s;
	INTERN TEXT name[8];

	name[0] = '>';
	for (s = name + 1; label; ++s)
		{
		*s = (label & 07) + '0';
		label >>= 3;
		}
	*s = '\0';
	return (name);
	}

/*	make an expression tree
 */
EXPR *mexpr(namsize)
	BYTES namsize;
	{
	IMPORT EXPR *exlist;
	FAST EXPR *q;

	q = (EXPR *)alloc(sizeof (*q) - sizeof (q->name) + namsize + 2, exlist);
	exlist = q;
	return (exzap(q));
	}

/*	get ms byte first integer
 */
COUNT mstoi(s)
	FAST UTINY *s;
	{
	COUNT val;

	val = s[0] << 8 | s[1];
	return (val);
	}

/*	make a printable number
 */
TEXT *nn(n, base)
	BYTES n, base;
	{
	INTERN TEXT buf[16];

	buf[stob(buf, n, base)] = '\0';
	return (buf);
	}

/*	make a long printable number
 */
TEXT *nnl(lo, base)
	LONG lo;
	BYTES base;
	{
	INTERN TEXT buf[32];

	buf[ltob(buf, lo, base)] = '\0';
	return (buf);
	}

/*	put a panic error message
 */
VOID panic(s)
	TEXT *s;
	{
	perror("PANIC! ", s);
	chput(" /PANIC ", s, "\n", NULL);
	putchr(-1);
	exit(NO);
	}

/*	put an error message
 */
VOID perror(s1, s2)
	TEXT *s1, *s2;
	{
	IMPORT COUNT lineno;
	IMPORT BOOL preverr, upcase;
	IMPORT FILE errfd;
	IMPORT TEXT *_pname, funname[], fname[];
	TEXT errbuf[128];
	TEXT *f, *s;

	f = (*funname == '<') ? funname + 1 : funname;
	cpystr(errbuf, "#error ", _pname, fname,  ":", nn(lineno, 10),
		"\t", f, (*f) ? "() " : "", s1, (s2) ? s2 : "", "\n", NULL);
	s = &errbuf[0];
	putstr(errfd, s, NULL);
	upcase = NO;
	for (; *s; s++)
		putchr(*s);
	upcase = YES;
	preverr = YES;
	}

/*	prefer wants but look to needs
 */
WANTS pref(want, need)
	WANTS want, need;
	{
	return ((want &= need) ? want : need);
	}

/*	put n bytes
 */
VOID putbytes(s, n)
	FAST TEXT *s;
	FAST COUNT n;
	{
	for ( ; n; --n)
		putchr(*s++);
	}

typedef struct literal {
	struct literal *next;
	COUNT type;
	LABEL label;
	LONG bias;
	TEXT name[1];
	} LITERAL;

/*	put literal in list if not already defined
 */
TEXT *putlit(p, littype, offset)
	FAST EXPR *p;
	COUNT littype;
	LONG *offset;
	{
	FAST LITERAL *lit;
	COUNT lbl;
	COUNT num;
	DOUBLE dbias;
	INTERN LITERAL *litroot = {NULL};

	lbl = 0;
	if (p)
		{
		switch (littype)
			{
		case TAGW:
		case TAGL:
			if (-4 <= p->f.bias && p->f.bias <= 4)
				{
				num = p->f.bias;
				*offset = (LONG)(num * 4 + ((littype == TAGW) ? 2 : 0));
				return ("@const");
				}
			}
		}
	else
		csect(ISTEXT);
	for (lit = litroot; lit; lit = lit->next)
		{
		if (!p)
			{
			genlabel(lbln(lit->label));
			num = lit->bias;
			switch (lit->type)
				{
			case TAGA:
				genaddr(lit->name, num);
				break;
			case TAGW:
				genword(num);
				break;
			case TAGL:
				genlong(lit->bias);
				break;
			case TAGF:
			case TAGD:
				dbias = (DOUBLE)lit->bias;
				ibmflt(&dbias, YES);
				genconst(&dbias, (lit->type == TAGD) ? 8 : 4);
				break;
				}
			}
		else
			{
			if (littype == lit->type &&
				p->f.bias == lit->bias &&
				((littype != TAGA) ? YES : cmpstr(p->f.nm, lit->name)))
				{
				lbl = lit->label;
				break;
				}
			}
		}
	if (p && !lbl)
		{
		litroot = (LITERAL *) alloc(sizeof (*litroot) +
			((littype == TAGA) ? lenstr(p->f.nm) : 0), litroot);
		litroot->type = littype;
		lbl = litroot->label = crs();	
		litroot->bias = p->f.bias;
		if (littype == TAGA)
			cpystr(litroot->name, p->f.nm, NULL);
		}
	return(lbln(lbl));
	}

/*	replace the current label for proper code sequence listing
 */
VOID relabel(p)
	FAST HEADER *p;
	{
	IMPORT BOOL lstflag;
	IMPORT HEADER *labtab;
	FAST HEADER **qb, **qbb;

	if (!lstflag)
		return;
	p->flags &= ~NEWLAB;
	for (qb = &labtab; *qb; qb = &(*qb)->next)
		if (*qb == p)
			qbb = qb;
	if (&(*qbb)->next == qb)
		return;
	*qbb = p->next;
	*qb = p;
	p->next = NULL;
	}

/*	convert register mask to index number
 */
TINY rtox(r)
	RESOURCE r;
	{
	IMPORT RESOURCE rtab[];
	IMPORT TINY xtab[];
	FAST COUNT i;

	if (r)
		for (i = 0; rtab[i]; ++i)
			if (rtab[i] == r)
				return (xtab[i]);
	return (0);
	}

/*	set allocated address
 */
VOID setad(p, ty, idx, refs)
	FAST EXPR *p;
	TYPE ty;
	TINY idx, refs;
	{
	IMPORT TINY regtype[];

dbprint("/setad");
	p->got = 0;
	p->f.ty = regtype[ty & TYPEMASK];
	p->f.pspace = 0;
	p->f.stsize = 0;
	p->f.space = 0;
	p->f.nm = "";
	p->f.bias = 0;
	p->f.idx = idx;
	p->f.refs = refs;
	}

/*	set line number in the code block
 */
VOID setlines(q)
	FAST CODE **q;
	{
	IMPORT BOOL lstflag;
	IMPORT COUNT lineno;

	if (!q || !lstflag)
		return;
	(*q)->linenum = lineno;
	}

/*	set created operator
 */
VOID setop(p, op, ty, pspace, l, r)
	FAST EXPR *p;
	LEX op;
	TYPE ty;
	SPACE pspace;
	EXPR *l, *r;
	{
	p->op = op;
	p->e.o.ty = ty;
	p->e.o.pspace = pspace;
	p->e.o.left = l;
	p->e.o.right = r;
	}

/*	get space of unfixed expression
 */
SPACE spaceof(p)
	FAST EXPR *p;
	{
	if (!p->op)
		return (p->e.v.space);
	else if (p->op == DINDIR)
		return (p->e.o.left->e.v.pspace);
	else
		return (0);
	}
		
/*	get volatile index number
 */
RESOURCE voltor(p)
	FAST EXPR *p;
	{
dbprint("/voltor set: %S", (p->got & GVOL) ? xtor(p->f.idx) : 0);
	return ((p->got & GVOL && (p->f.idx != XXR1)) ? xtor(p->f.idx) : 0);
	}

/*	extract from the register set
 */
RESOURCE xset(reg)
	FAST RESOURCE reg;
	{
	reg ^= (reg - 1) & reg;
	return (reg);
	}

/*	convert index number to register mask
 */
RESOURCE xtor(x)
	TINY x;
	{
	IMPORT RESOURCE rtab[];
	IMPORT TINY xtab[];

	return ((!x) ? 0 : rtab[scnstr(xtab, x)]);
	}
