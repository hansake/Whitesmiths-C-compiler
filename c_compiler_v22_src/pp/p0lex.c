/*	LEXICAL ANALYSIS FOR C
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int0.h"
#include "int01.h"

/*	the keyword table
 */
LOCAL PRETAB keytab[] {
	"\2do", LDO,
	"\2if", LIF,
	"\3for", LFOR,
	"\3int", LINT,
	"\4auto", LAUTO,
	"\4case", LCASE,
	"\4char", LCHAR,
	"\4else", LELSE,
	"\4goto", LGOTO,
	"\4long", LLONG,
	"\5break", LBREAK,
	"\5float", LFLOAT,
	"\5short", LSHORT,
	"\5union", LUNION,
	"\5while", LWHILE,
	"\6double", LDOUBLE,
	"\6extern", LEXTERN,
	"\6return", LRETURN,
	"\6sizeof", LSIZEOF,
	"\6static", LSTATIC,
	"\6struct", LSTRUCT,
	"\6switch", LSWITCH,
	"\7default", LDFAULT,
	"\7typedef", LTYPDEF,
	"\10continue", LCONTIN,
	"\10register", LREG,
	"\10unsigned", LUNSIGN,
	};
#define NKEYS	27

/* find first non-base digit
 */
BYTES firnon(s, n, base)
	FAST TEXT *s;
	BYTES n;
	COUNT base;
	{
	FAST COUNT dig, i;

	for (i = 0; i < n; ++i, ++s)
		{
		if (isdigit(*s))
			dig = *s - '0';
		else if (isalpha(*s))
			dig = tolower(*s) + (10 - 'a');
		else
			break;
		if (base <= dig)
			break;
		}
	return (i);
	}

/*	accumulate double number
 */
BYTES flaccum(s, pd, nd)
	FAST TEXT *s;
	DOUBLE *pd;
	FAST COUNT nd;
	{
	IMPORT DOUBLE _dtens[];
	FAST BYTES n;

	for (n = 0; n < nd && isdigit(*s); ++n, ++s)
		*pd = *pd * _dtens[1] + (*s - '0');
	return (n);
	}

/*	put lexeme for '...'
 */
TLIST *lexchars(p)
	FAST TLIST *p;
	{
	FAST BYTES i, n;
	LONG lo;
	TINY sbuf[STRSIZE];
	TINY ty;

	n = doesc(sbuf, p->text, p->ntext) - 2;
	for (i = (n < 4) ? 1 : n - 3, lo = 0; i <= n; ++i)
		lo = lo << 8 | sbuf[i] & BYTMASK;
	switch (n)
		{
	case 0:
		ty = LCNUM;
		break;
	case 1:
		ty = (0 <= sbuf[1]) ? LCNUM : LSNUM;
		break;
	case 2:
		ty = (0 <= sbuf[1]) ? LSNUM : LUSNUM;
		break;
	default:
		ty = (lo < 0) ? LULNUM : LLNUM;
		}
	putcode("c4", ty, &lo);
	return (p->next);
	}

/*	put out floating constant
 */
TLIST *lexfloat(p)
	TLIST *p;
	{
	IMPORT DOUBLE _dzero, dtento();
	FAST BYTES n;
	FAST TEXT *s;
	BOOL minus;
	COUNT exp, x;
	DOUBLE dnum;

	s = p->text;
	dnum = _dzero;
	s =+ flaccum(s, &dnum, p->ntext);
	s = lexfnxt(&p, s);
	if (*s == '.')
		{
		p = p->next, s = p->text;
		n = flaccum(s, &dnum, p->ntext);
		exp = -n;
		s =+ n;
		}
	else
		exp = 0;
	s = lexfnxt(&p, s);
	if (tolower(*s) == 'e')
		{
		++s;
		s = lexfnxt(&p, s);
		minus = NO;
		if (*s == '+')
			++s;
		else if (*s == '-')
			{
			++s;
			minus = YES;
			}
		s = lexfnxt(&p, s);
		s =+ btos(s, p->ntext - (s - p->text), &x, 10);
		if (minus)
			exp =- x;
		else
			exp =+ x;
		}
	dnum = dtento(dnum, exp);
	putcode("c8", LDNUM, &dnum);
	if (s == p->text)
		;
	else if (p->text + p->ntext == s)
		p = p->next;
	else
		{
		p = p->next;
		perror("illegal float constant");
		}
	return (p);
	}

/*	move to next lexeme for floating constant if necessary
 */
TEXT *lexfnxt(pp, s)
	TLIST **pp;
	TEXT *s;
	{
	FAST TLIST *p;

	p = *pp;
	if (p->text + p->ntext <= s)
		{
		p = p->next;
		*pp = p;
		return (p->text);
		}
	else
		return (s);
	}

/*	put lexeme for identifier or keyword
 */
TLIST *lexident(p)
	FAST TLIST *p;
	{
	IMPORT PRETAB keytab[];
	FAST BYTES n;
	FAST LEX tok;

	n = min(p->ntext, LENNAME);
	if (tok = scntab(keytab, NKEYS, p->text, n))
		putcode("c", tok);
	else
		putcode("ccb", LIDENT, n, p->text, n);
	return (p->next);
	}

/*	put a long or integer constant
 */
TLIST *lexint(p, base, nskip)
	FAST TLIST *p;
	COUNT base;
	BYTES nskip;
	{
	FAST BOOL signed;
	FAST BYTES n;
	COUNT snum, unum;
	LONG lnum;
	TINY cnum, ty;

	signed = (base == 10);
	n = p->ntext - nskip;
	if (btol(p->text + nskip, n, &lnum, base) < n)
		perror("illegal constant %b", p->text, p->ntext);
	unum = (ULONG)lnum >> 16;
	snum = lnum;
	cnum = snum;
	if (tolower(p->text[p->ntext - 1]) == 'l' ||
		signed && snum != lnum || !signed && unum)
		ty = (signed || 0 <= lnum) ? LLNUM : LULNUM;
	else if (signed && cnum != snum || !signed && (snum & ~BYTMASK))
		ty = (signed || 0 <= snum) ? LSNUM : LUSNUM;
	else
		ty = (signed || 0 <= cnum) ? LCNUM : LSNUM;
	putcode("c4", ty, &lnum);
	return (p->next);
	}

/*	put lexeme for integer or float
 */
TLIST *lexnum(p)
	FAST TLIST *p;
	{
	FAST BYTES n;

	n = firnon(p->text, p->ntext, 10);
	if (punct(p, '.') || n < p->ntext && tolower(p->text[n]) == 'e'
		|| punct(p->next, '.'))
		return (lexfloat(p));
	else if (p->text[0] != '0')
		return (lexint(p, 10, 0));
	else if (1 < p->ntext && tolower(p->text[1]) == 'x')
		return (lexint(p, 16, 2));
	else
		return (lexint(p, 8, 1));
	}

/*	put lexeme for punctuation
 */
TLIST *lexpunct(p)
	TLIST *p;
	{
	putcode("c", dopunct(&p));
	return (p);
	}

/*	put lexeme for "..."
 */
TLIST *lexstring(p)
	FAST TLIST *p;
	{
	COUNT n;
	TEXT sbuf[STRSIZE];

	n = doesc(sbuf, p->text, p->ntext) - 2;
	putcode("c2b", LSTRING, &n, &sbuf[1], n);
	return (p->next);
	}

/*	put lexemes to stdout
 */
VOID putcode(fmt, args)
	TEXT *fmt, *args;
	{
	FAST BYTES *p;
	FAST TEXT *f, *q;
	COUNT i;

	for (f = fmt, p = &args; *f; ++f)
		if (*f == 'c')
			putch(*p++ & BYTMASK);
		else
			{
			q = *p++;
			i = (*f == 'b') ? *p++ : (*f == 'p') ? lenstr(q) : *f - '0';
			while (0 <= --i)
				putch(*q++ & BYTMASK);
			}
	}

/*	put a line without #'s
 */
VOID putls(p)
	FAST TLIST *p;
	{
	IMPORT BOOL xflag, v6flag;
	IMPORT INCL *pincl;
	IMPORT BOOL pflag;
	FAST TLIST *q;
	INTERN COUNT xlno;

	if (!xflag)
		{
		if (!v6flag)
			;
		else if (pincl->fname)
			putlin("\1", 1);
		else
			while (++xlno < pincl->nline)
				putlin("\n", 1);
		for (q = p; ; q = q->next)
			{
			putlin(q->white, q->nwhite);
			if (!q->next)
				break;
			putlin(q->text, q->ntext);
			}
		putlin("\n", 1);
		}
	else
		{
		if (p->type != PEOL)
			{
			if (pincl)
				putcode("c2", LLINENO, &pincl->nline);
			if (pflag)
				{
				pflag = NO;
				if (pincl)
					putcode("ccp", LIFILE,
					(pincl->fname) ? lenstr(pincl->fname) : 0,
					(pincl->fname) ? pincl->fname : "");
				}
			}
		for (; p->type != PEOL; )
			if (p->type == PIDENT)
				p = lexident(p);
			else if (p->type == PSTRING)
				p = lexstring(p);
			else if (p->type == PCHCON)
				p = lexchars(p);
			else if (p->type == PNUM ||
				(punct(p, '.') && p->next->type == PNUM))
				p = lexnum(p);
			else
				p = lexpunct(p);
		}
	}
