/*	GEN CODE FOR PASS2
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

#define YBYTE	1
#define XBYTE	2
#define DECHL	4
#define DECBC	010
#define SWPHL	020
#define SWPDE	040
#define XPOPHL	0100
#define XPOPBC	0200
#define YPOPHL	0400
#define YPOPBC	01000

/*	register masks to index numbers
 */
GLOBAL TINY ctab[] {030, 030, 030, 030, 030, 020, 040, 010, 000, 000, 000, 030};
GLOBAL BITS rtab[] {R0, R1, R2, R3, R4, BC, AUTIDX, HL, TS, R0, R1, 0};
GLOBAL TINY xtab[] {X0, X1, X2, X3, X4, XBC, XDE, XHL, XSP, XP0, XP1, 0};
LOCAL TEXT *xname[] {"\20", "\21", "\22", "\23", "\24",
		"bc", "de", "hl", "SP"};
LOCAL TEXT *uptab[]  {
		"-1-1-1", "-1-1", "-1", "", "+1", "+1+1", "+1+1+1"};
LOCAL TINY cxtab[] {XBC, XDE, XHL, 0};
LOCAL TEXT *dnname[] {"c", "e", "l", "DN?"};
LOCAL TEXT *upname[] {"b", "d", "h", "UP?"};

/*	break functions
 */
GLOBAL TEXT *(*gfun[])() {NULL, utobits, bitstou};

/*	move bit field to uppable location
 */
TEXT *bitstou(p)
	FAST EXPR *p;
	{
	putos(p->e.o.right->e.o.ty, "hl=>sp=0", "=>sp\n");
	switch (gotten(p->e.o.left))
		{
	case WBC:
		return ("\x\x\xKbtou; bc<=sp");
	case WMEM:
	case WVMEM:
		return ("\x\x\xKbtou; hl<=sp->WX");
	default:
		return ("\x\x\xKbtou; hl<=sp; a=l->X=h->UX");
		}
	}

/*	generate code sequences
 */
BITS gen(p, l, r, tab, set)
	FAST EXPR *p;
	FAST EXPR *l, *r;
	TEXT *tab;
	BITS set;
	{
	IMPORT TEXT *(*gfun[])();
	IMPORT TINY equtype[], regtype[];
	BITS code, flags, ty;
	TINY idx;

#ifdef DEBUG
putfmt("/gen %o %o %o %o\n", p->op, tab, set, p->e.o.ty);
putterm(l, 1);
putterm(r, 1);
#endif
	code = tab[2];
	idx = l->f.idx;
	if (!(flags = genfl(p, l, r, tab, set)))
		return (0);
	if (idx != l->f.idx)
		set =& ~xtor(l->f.idx);
	if ((code & 0200) && !(tab = (*gfun[tab[3]])(p, set)))
		return (0);
	if (code & 0100)
		setad(p, p->e.o.ty, 0, 0);
	else
		{
		cpyad(&p->f, &l->f);
		ty = regtype[p->e.o.ty & 017];
		if (equtype[p->f.ty & 017] == equtype[ty])
			p->f.ty = ty;
		p->got = l->got & GVOL;
		}
	if ((l->got & (GJUNK|GVOL)) == (GJUNK|GVOL))
		set =| xtor(l->f.idx);
	if ((r->got & (GJUNK|GVOL)) == (GJUNK|GVOL))
		set =| xtor(r->f.idx);
	if (p->op == LGETS && adequ(&l->f, &r->f) &&
		r->idx != XP0 && r->idx != XP1)
		return (set);
	if (p->op == LPLUS && !l->f.refs && !r->f.refs &&
		l->f.idx != XSP && r->f.idx != XSP)
		if ((!l->f.nm[0] || !r->f.nm[0]) &&
			(!l->f.idx || !r->f.idx) && (p->e.o.ty & 017) <= XUSHORT)
			{
			if (r->f.nm[0])
				cpybuf(p->f.nm, r->f.nm, LENNAME);
			p->f.bias = p->f.bias + r->f.bias;
			p->f.idx =+ r->f.idx;
			if (r->got & GVOL)
				{
				p->got =| GVOL;
				set =& ~xtor(p->f.idx);
				}
			if (!p->f.nm[0] && -4 <= (ARGINT)p->f.bias &&
				(ARGINT)p->f.bias <= 4 && (p->f.idx == XHL ||
				p->f.idx == XBC))
				p->f.bias = 0;
			else
				return (set);
			}
	if (code & 040)
		p->got =| GZSET;
	if (code & 004)
		p->got =| GNSET;
#ifdef DEBUG
putfmt("/gen = %p\n", &tab[3]);
#endif
	return (genput(p, tab, set, flags));
	}

/*	generate an address mode
 */
VOID genad(p, up, byte)
	FAST EXPR *p;
	COUNT up;
	BOOL byte;
	{
	IMPORT TEXT *dnname[], *upname[], *uptab[];
	IMPORT TINY cxtab[], xtab[];
	FAST TINY idx, refs;
	BYTES off;
	COUNT i;
	TEXT buf[2], fc, *xn;

	i = p->got + up;	/* YUK! */
	p->got = i;
	up = i;
	idx = p->f.idx;
	refs = p->f.refs;
	fc = p->f.nm[0];
	xn = xname[scnstr(xtab, idx)];
	off = p->f.bias;
	if (!fc && !idx)
		{
		if (!refs)
			{
			off =>> up << 3;
			if (byte && ((off =& BYTMASK) == 1))
				off = 0401;
			}
		else
			{
			off =+ up;
			chput("*", NULL);
			}
		putnb(p->f.nm, off);
		}
	else if (!idx && !refs && byte)
		{
		chput("(=", NULL);
		putnb(p->f.nm, off);
		chput(")", NULL);
		if (up)
			{
			chput("[", NULL);
			putnb("", up);
			chput("]", NULL);
			}
		}
	else if (!idx)
		{
		if (!refs)
			chput("&", NULL);
		putnb(p->f.nm, off + up);
		}
	else if (refs)
		if (up)
			{
			chput("*(", xn, uptab[up + 3], ")", NULL);
			p->got = 0;
			}
		else
			chput("*", xn, NULL);
	else if (!byte)
		chput(xn, NULL);
	else if (xtor(idx) & RS)
		putnb(xn, up);
	else
		if (up)
			chput(upname[scnstr(cxtab, idx)], NULL);
		else
			chput(dnname[scnstr(cxtab, idx)], NULL);
	}

/*	generate flags for wrapup
 */
BITS genfl(p, l, r, tab, set)
	FAST EXPR *p, *l, *r;
	TEXT *tab;
	BITS set;
	{
	BITS code, flags;

	flags = XBYTE|YBYTE;
	code = tab[2];
	if ((l->f.idx == XSP || l->f.idx == XP0 || l->f.idx == XP1) &&
		!(twant(tab[0]) & (WSTACK|WPSTK)))
		if (set & HL)
			{
			chput("hl<=sp\n", NULL);
			p->f.idx = XHL;
			l->f.idx = XHL;
			}
		else if (set & BC && twant(tab[0]) & (l->f.refs ? WPBC : WBC))
			{
			chput("bc<=sp\n", NULL);
			p->f.idx = XBC;
			l->f.idx = XBC;
			}
		else if (r->f.idx != XHL)
			{
			chput("hl<>*sp\n", NULL);
			l->f.idx = XHL;
			flags =| SWPHL;
			}
		else if (set & BC && twant(tab[1]) & (r->f.refs ? WPBC : WBC))
			{
			chput("hl<>*sp;bc<=sp\n", NULL);
			p->f.idx = XHL;
			l->f.idx = XHL;
			r->f.idx = XBC;
			}
		else
			return (0);
	if ((l->got & (GJUNK|GVOL)) == (GJUNK|GVOL) || !l->f.refs ||
		!(code & 030) || !(xtor(l->f.idx) & (BC|HL)))
		;
	else if (code & 010)
		flags =| (l->f.idx == XHL) ? DECHL : DECBC;
	else if (l->f.idx == XHL)
		{
		chput("hl=>sp\n", NULL);
		flags =| XPOPHL;
		}
	else
		{
		chput("bc=>sp\n", NULL);
		flags =| XPOPBC;
		}
	if ((r->got & (GJUNK|GVOL)) == (GJUNK|GVOL) || !r->f.refs ||
		!(code & 003) || !(xtor(r->f.idx) & (BC|HL)))
		;
	else if (code & 001)
		flags =| (r->f.idx == XHL) ? DECHL : DECBC;
	else if (r->f.idx == XHL)
		{
		chput("hl=>sp\n", NULL);
		flags =| YPOPHL;
		}
	else
		{
		chput("bc=>sp\n", NULL);
		flags =| YPOPBC;
		}
	return (flags);
	}

/*	do the actual code output
 */
BITS genput(p, tab, set, flags)
	FAST EXPR *p;
	TEXT *tab;
	BITS set, flags;
	{
	FAST TEXT *q;
	COUNT i;
	LABEL crlab;

	crlab = 0;
	p->e.o.left->got = 0;
	p->e.o.right->got = 0;
	for (q = &tab[3]; *q; ++q)
		switch (*q)
			{
		case 'X':
			genad(p->e.o.left, 0, flags & XBYTE);
			break;
		case 'Y':
			genad(p->e.o.right, 0, flags & YBYTE);
			break;
		case 'U':
		case 'D':
			for (i = 0; *q == 'U'; ++q)
				++i;
			for (; *q == 'D'; ++q)
				--i;
			flags =| (*q == 'X') ? XBYTE : YBYTE;
			genad((*q == 'X') ? p->e.o.left : p->e.o.right, i, YES);
			break;
		case 'W':
			flags =& (q[1] == 'X') ? ~XBYTE : ~YBYTE;
			break;
		case 'T':
			crlab = crs();
			chput(ln(crlab), NULL);
			break;
		case 'K':
			chput("\1c.", NULL);
			while (*++q && *q != ';')
				chwrite(q, 1);
			--q;
			break;
		case 'j':
			chwrite("j", 1);
			break;
		case '0':
			for (; isdigit(*q); ++q)
				chwrite(q, 1);
			--q;
			break;
		default:
			for (i = 0; scnstr("=<>-+^|&:~", *q) < 10; ++i, ++q)
				chwrite(q, 1);
			if (i)
				--q;
			else
				chwrite(q, 1);
			}
	chput("\n", NULL);
	if (crlab)
		chput(ln(crlab), ":\n", NULL);
	if (flags & YPOPBC)
		chput("bc<=sp\n", NULL);
	if (flags & YPOPHL)
		chput("hl<=sp\n", NULL);
	if (flags & XPOPBC)
		chput("bc<=sp\n", NULL);
	if (flags & XPOPHL)
		chput("hl<=sp\n", NULL);
	if (flags & SWPDE)
		{
		chput("hl<>*sp<>de<>*sp\n", NULL);
		p->f.idx = XSP;
		}
	if (flags & SWPHL)
		{
		chput("hl<>*sp\n", NULL);
		p->f.idx = XSP;
		}
	if (flags & DECBC)
		chput("bc-1\n", NULL);
	if (flags & DECHL)
		chput("hl-1\n", NULL);
	return (set);
	}

/*	put name and bias
 */
VOID putnb(s, off)
	FAST TEXT *s;
	FAST BYTES off;
	{
	FAST COUNT i;
	TEXT buf[12];

	if (!(i = inbuf(s, LENNAME, "\0")) || *s == '#')
		;
	else if ('\24' < *s)
		chput("$", NULL);
	chwrite(s, i);
	buf[stob(buf, off, 8)] = '\0';
	if (i && off)
		chput("[0", buf,"]", NULL);
	else if (!i)
		chput("0", off ? buf : NULL, NULL);
	}

/*	put offset and size for bits
 */
VOID putos(ty, s1, s2)
	BITS ty;
	TEXT *s1, *s2;
	{
	TEXT tbuf[10];

	tbuf[stob(tbuf, ty >> 4 & 077 | ty >> 2 & ~BYTMASK, 8)] = '\0';
	chput(s1, tbuf, s2, NULL);
	}

/*	move an uppable to bit field
 */
TEXT *utobits(p)
	FAST EXPR *p;
	{
	
	putos(p->e.o.left->e.o.ty, "hl=0", "=>sp\n");
	return ("\x\x\xKutob");
	}

/*	get the index name
 */
TEXT *xnm(idx)
	FAST TINY idx;
	{
	IMPORT TEXT *xname[];
	IMPORT TINY xtab[];

	return (xname[scnstr(xtab, idx)]);
	}
