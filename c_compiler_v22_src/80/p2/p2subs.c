/*	INPUT AND UTILITY ROUTINES
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	type tables
 */
GLOBAL TINY bytype[] {0, 2, 2, 2, 2, 2, 4, 4, 4, 4, 8};
GLOBAL TINY equtype[] {0, XUCHAR, XUCHAR, XUSHORT, XSFIELD, XUSHORT,
		XULONG, XLFIELD, XULONG, XFLOAT, XDOUBLE};
GLOBAL TINY regtype[] {0, XSHORT, XSHORT, XSHORT, XUSHORT, XUSHORT,
		XLONG, XULONG, XULONG, XDOUBLE, XDOUBLE};

/*	the nameless name
 */
GLOBAL TEXT noname[] {0, 0, 0, 0, 0, 0, 0, 0};

/*	relational operator mapping tables
 */
GLOBAL TINY cmpops[] {GLESS, GLEQ, GGREAT, GGEQ, GISEQ, GNOTEQ, 0};
GLOBAL TINY flpops[] {GGEQ, GGREAT, GLEQ, GLESS, GNOTEQ, GISEQ};
GLOBAL TINY lexops[] {LLESS, LLEQ, LGREAT, LGEQ, LISEQ, LNOTEQ, 0};

/*	the character pool stuff
 */
#define CHBUF struct chbuf

struct chbuf {
	TEXT *next;
	TEXT cbuf[01000];
	} *chbase {NULL};

/*	put strings to character pool
 */
VOID chput(args)
	TEXT *args;
	{
	FAST TEXT **p;

	for (p = &args; *p; ++p)
		chwrite(*p, lenstr(*p));
	}

/*	write buffer to character pool
 */
VOID chwrite(s, n)
	TEXT *s;
	BYTES n;
	{
	IMPORT BYTES choff;
	IMPORT CHBUF *chbase;
	FAST COUNT i;
	FAST CHBUF **qb, *q;

	for (; 0 < n; n =- i)
		{
		for (i = choff >> 9, qb = &chbase; 0 <= i; --i)
			{
			if (!*qb)
				{
				*qb = alloc(sizeof (**qb), NULL);
				(*qb)->cbuf[0777] = '\0';
				}
			q = *qb;
			qb = &(*qb)->next;
			}
		i = choff & 0777;
		i = cpybuf(&q->cbuf[i], s, min(0777 - i, n));
#ifdef DEBUG
putfmt("<<%i|%b>>", choff, s, i);
#endif
		choff =+ i;
		if ((choff & 0777) == 0777)
			++choff;
		s =+ i;
		}
	}

/*	read from character pool
 */
TEXT *chread()
	{
	IMPORT BYTES choff;
	IMPORT CHBUF *chbase;
	FAST COUNT i;
	FAST CHBUF *q;
	FAST TEXT *s;

	for (i = choff >> 9, q = chbase; q && 0 < i; --i)
		q = q->next;
	if (q)
		{
		s = &q->cbuf[choff & 0777];
		choff = choff + 01000 & ~0777;
		}
	else
		panic("CHREAD");
	return (s);
	}

/*	copy address structure
 */
VOID cpyad(pl, pr)
	FAST ADDR *pl, *pr;
	{
	pl->ty = pr->ty;
	cpybuf(pl->nm, pr->nm, LENNAME);
	pl->bias = pr->bias;
	pl->idx = pr->idx;
	pl->refs = pr->refs;
	}

/*	convert floating to DEC format
 */
BOOL decflt(d, dble)
	FAST TEXT *d;
	BOOL dble;
	{
	FAST COUNT i, t;
	COUNT exp;

	exp = _fcan(d) + 0200;
	if (!d[1])
		return (NO);
	if (!dble)
		for (i = 4; i < 8; ++i)
			d[i] = 0;
	d[0] =| exp >> 1 & 0177;
	d[1] = exp << 7 | d[1] & 0177;
	for (i = 0; i < 8; i =+ 2)
		t = d[i], d[i] = d[i + 1], d[i + 1] = t;
	return (d[4] || d[5] || d[6] || d[7]);
	}

/*	get a function token, putting data
 */
LEX gcode()
	{
	FAST COUNT i, n;
	FAST LEX c;
	COUNT num;
	LONG lo;
	TEXT name[LENNAME+1];

	FOREVER
		switch (c = getch())
			{
		case GADDR:
		case GADDR+1:
		case GADDR+2:
		case GADDR+3:
		case GADDR+4:
			gname(name);
			gint(&lo);
			num = lo;
			if (name[0] && num)
				putasm("&%p%p%o%p\n", name, "[", num, "]");
			else if (name[0])
				putasm("&%p\n", name);
			else
				putasm("&%o\n", num);
			break;
		case GDEF:
		case GREF:
			putasm("public %p\n", gname(name));
			break;
		case GDATA:
		case GNDATA:
		case GLITS:
		case GSWTAB:
			n = needch();
			if ( c == GDATA )
				csect(ISDATA);
			else if ( c == GNDATA )
				csect(ISNDATA);
			else
				csect(ISLITS);
			putasm("%p:\n", gname(name));
			break;
		case GINT:
			n = needch();
			gint(&lo);
			num = lo;
			if (n == 1)
				putasm("%o\n", num);
			else if (n == 2)
				putasm("&%o\n", num);
			else
				putasm("&%o,&%o\n", (COUNT)(lo >> 16), num);
			break;
		case GFLOAT:
			n = needch();
			for (i = 0; i < 8; ++i)
				name[i] = needch();
			decflt(name, n == 8);
			putasm("&%o,&%o", lstoi(&name[0]),
				lstoi(&name[2]));
			if (4 < n)
				putasm(",&%o,&%o", lstoi(&name[4]),
					lstoi(&name[6]));
			putasm("\n");
			break;
		case GSTRING:
			n = needch();
			while (0 <= n)
				{
				putasm("%o", (n <= 0) ? 0 : needch());
				for (i = 1; 0 <= --n && i < 8; ++i)
					putasm(",%o", (n <= 0) ? 0 : needch());
				putasm("\n");
				}
			break;
		case GSPACE:
			gint(&lo);
			num = lo;
			putasm(".:=.[%o]\n", num);
			break;
		default:
			return (c);
			}
	}

/*	get an expression tree
 */
EXPR *gexpr()
	{
	IMPORT EXPR *exlist, exzero;
	FAST COUNT n;
	FAST EXPR *q;
	FAST TEXT *s;
	LONG bias;
	INTERN TINY gidx[] {XBC, X2, X3, X4, XDE, XDE, X0, X0, 0};
	INTERN TINY grx[] {INTRET, R2, R3, R4, ARGIDX, AUTIDX, FLTRET, LONGRET, 0};

	q = alloc(sizeof (*q), exlist), exlist = q;
	q->got = 0;
	q->op = needch();
	if ((q->e.v.ty = needch()) == XSFIELD || q->e.v.ty == XLFIELD)
		{
		q->e.v.ty =| needch() << 4;
		q->e.v.ty =| needch() << 10;
		}
	else if ((q->e.v.ty & 017) == XPTRTO)
		q->e.v.ty = XUSHORT;
	if (q->op)
		{
		q->e.o.left = gexpr();
		if (q->op == LQUERY)
			q->e.o.mid = gexpr();
		q->e.o.right = (q->op & BINOP) ? gexpr() : &exzero;
		}
	else
		{
		for (n = needch(), s = q->e.v.nm; 0 < n; --n)
			*s++ = needch();
		while (s < &q->e.v.nm[LENNAME])
			*s++ = '\0';
		gint(&bias);
		q->e.v.bias = bias;
		if ((n = needch()) == ARGIDX)
			q->e.v.bias =+ 4;
		else if (n == AUTIDX)
			q->e.v.bias =- AUTOFF;
		q->e.v.idx = gidx[scnstr(grx, n)];
		q->e.v.refs = needch();
		s = q->e.v.nm;
		if (q->e.v.ty == XDOUBLE && !q->e.v.refs && !q->e.v.idx)
			{
			decflt(s, YES);
			csect(ISLITS);
			s = ln(crs());
			putasm("%p:&%o,&%o,&%o,&%o\n", s,
				lstoi(&q->e.v.nm[0]), lstoi(&q->e.v.nm[2]),
				lstoi(&q->e.v.nm[4]), lstoi(&q->e.v.nm[6]));
			cpystr(q->e.v.nm, s, NULL);
			q->e.v.bias = 0;
			q->e.v.refs = 1;
			}
		else if (*s == '>')
			*s = '#';
		if ((q->e.v.ty == XLONG || q->e.v.ty == XULONG) && iscons(q))
			{
			csect(ISLITS);
			cpystr(q->e.v.nm, ln(crs()), NULL);
			putasm("%p:&%o,&%o\n", q->e.v.nm, 
				(COUNT)(bias >> 16), q->e.v.bias);
			q->e.v.bias = 0;
			q->e.v.refs = 1;
			}
		}
	return (q);
	}

/*	get an integer
 */
VOID gint(s)
	FAST TEXT *s;
	{
	FAST BYTES i;

	for (i = 0; i < 4; ++i)
		*s++ = needch();
	}

/*	get a label
 */
LABEL glabel()
	{
	FAST TEXT *p;
	LABEL l;

	p = &l;
	*p++ = needch();
	*p = needch();
	return (l);
	}

/*	get a name
 */
TEXT *gname(s)
	TEXT *s;
	{
	FAST BYTES n;
	FAST TEXT *q;

	q = s;
	if (n = needch())
		{
		if ((*q = needch()) != '>')
			{
			q[1] = q[0];
			*q++ = '$';
			}
		else
			*q = '#';
		++q;
		while (0 < --n)
			*q++ = needch();
		}
	*q = '\0';
	return (s);
	}

/*	classify address of fixed expression
 */
BITS gotten(p)
	FAST EXPR *p;
	{
	FAST BITS got;
	FAST COUNT refs;

	refs = p->f.refs;
	switch (p->f.idx)
		{
	case 0:
		got = (refs || p->f.nm[0]) ? WMEM : WBC;
		break;
	case XDE:
	case XBC:
		got = refs ? WPBC : WBC;
		break;
	case XHL:
		got = refs ? WPHL : WHL;
		break;
	case XP0:
	case XP1:
	case XSP:
		got = refs ? WPSTK : WSTACK;
		break;
	case X0:
	case X1:
		got = refs ? WTERM : WVMEM;
		break;
	default:
		got = refs ? WTERM : WMEM;
		}
	return (got);
	}

/*	test for constant term
 */
BOOL iscons(p)
	FAST EXPR *p;
	{
	return (!p->op && !p->e.v.nm[0] && !p->e.v.idx && !p->e.v.refs);
	}

/*	make a name from label
 */
TEXT *ln(label)
	FAST LABEL label;
	{
	FAST TEXT *s;
	INTERN TEXT name[8];

	name[0] = '#';
	for (s = name + 1; label; ++s)
		{
		*s = (label & 07) + '0';
		label =>> 3;
		}
	*s = '\0';
	return (name);
	}

/*	need a character
 */
LEX needch()
	{
	FAST LEX c;

	if ((c = getch()) == EOF)
		{
		panic("EOF");
		exit(NO);
		}
	else
		return (c);
	}

/*	put a panic error message
 */
VOID panic(s)
	TEXT *s;
	{
	IMPORT BYTES lineno;
	IMPORT FILE errfd;
	IMPORT TEXT *_pname, funname[];
	TEXT buf[10], *f;

	buf[itob(buf, lineno, 10)] = '\0';
	if (*funname == '$')
		f = funname + 1;
	else
		f = funname;
	putstr(errfd, _pname, ": PANIC ", s, " ", f,
		*f ? "() " : "", "line ", buf, "!\n", NULL);
#ifdef DEBUG
	chput("/PANIC ", s, ";", NULL);
#else
	exit(NO);
#endif
	}

/*	prefer wants but look to needs
 */
BITS pref(want, need)
	BITS want, need;
	{
	return (((want =& need) ? want : need) & ~GJUNK);
	}

/*	convert register mask to index number
 */
TINY rtox(r)
	BITS r;
	{
	IMPORT BITS rtab[];
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
	BITS ty;
	TINY idx, refs;
	{
	IMPORT TEXT noname[];
	IMPORT TINY regtype[];

	p->got = 0;
	p->f.ty = regtype[ty & 017];
	cpybuf(p->f.nm, noname, LENNAME);
	p->f.bias = 0;
	p->f.idx = idx;
	p->f.refs = refs;
	}

/*	liberate a register, if possible
 */
BITS tempify(p, want, set)
	FAST EXPR *p;
	BITS want, set;
	{
	FAST BITS ty;

#ifdef DEBUG
putfmt("/tempify %o %o\n", want, set);
putterm(p, 1);
#endif
	if (!(p->got & GVOL))
		return (0);
	else if (p->f.refs)
		{
		if (xtor(p->f.idx) & RS)
			return (treg(p, want, set));
		--p->f.refs;
		ty = p->f.ty, p->f.ty = XUSHORT;
		set = force(p, WSTACK | (want & GJUNK), set, XUSHORT);
		p->f.ty = ty;
		++p->f.refs;
		return (set);
		}
	else
		return (force(p, ((want & VOLSET) ? WSTACK : WPSTK)
			| want & GJUNK, set, p->f.ty));
	}

/*	tempify a long or double on the stack
 */
BITS treg(p, want, set)
	FAST EXPR *p;
	BITS want, set;
	{
	IMPORT BYTES autmin, autoff;

	if ((set & (HL|BC)) != (HL|BC))
		return (0);
	set =| xtor(p->f.idx);
	autoff =- 8;
	if (autoff < autmin)
		autmin = autoff;
	chput("hl=", NULL);
	putnb("", (BYTES)autoff);
	chput("+de->bc;sp=>hl<=bc;\4\n", NULL);
	p->f.idx = XSP;
	return (set);
	}

/*	convert index number to register mask
 */
TINY xtor(x)
	TINY x;
	{
	IMPORT BITS rtab[];
	IMPORT TINY xtab[];

	return (!x ? 0 : rtab[scnstr(xtab, x & 077)] & ~AUTIDX);
	}
