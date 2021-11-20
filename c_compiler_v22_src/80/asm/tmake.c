/*	MAKE THINGS FOR A-NATURAL
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "t.h"

/*	index lists
 */
GLOBAL LEX bincodes[] {
	GETS, POP, SWAP, PLUS, MINUS, PLUSC, MINUSC,
	AND, XOR, OR, CMP, ROT, ROTC, ROTO, ROTZ,
	JMP, JCOND, GETSUP, GOESUP, GETSBYA, GETSNOT, 0};
GLOBAL TINY crlist[] {B, C, D, E, H, L, IHL, A, 0};
GLOBAL TINY irlist[] {BC, DE, HL, SP, 0};

/*	the literal table
 */
GLOBAL LIT *littab {NULL};

/*	token control
 */
LOCAL COUNT nback {0};
LOCAL META chstk[3] {0};

/*	add a symbol to table
 */
TERM *addsym(s)
	FAST TEXT *s;
	{
	IMPORT TERM *symtab;
	FAST COUNT i;
	FAST TERM *q;

	q = alloc(sizeof (*q), symtab);
	symtab = q;
	q->ty = INN;
	q->val = 0;
	q->base = q;
	for (i = 0; i < LENNAME; ++i)
		if (!(q->nm[i] = *s++))
			break;
	while (i < LENNAME)
		q->nm[i++] = '\0';
	return (q);
	}

/*	put back a char
 */
VOID bchar(c)
	META c;
	{
	IMPORT COUNT nback;
	IMPORT META chstk[];

	chstk[nback++] = c;
	}

/*	match a binary operand
 */
BOOL bmatch(code, p)
	LEX code;
	FAST TERM *p;
	{
	IMPORT TINY crlist[], irlist[];
	FAST LEX pty;

	pty = p->ty & TMASK;
	switch (code)
		{
	case -1:
		return ((nn(pty) && !p->base) && p->val == -1);
	case 1:
		return ((nn(pty) && !p->base) && p->val == 1);
	case Cr:
		return (crlist[scnstr(crlist, pty)]);
	case Ir:
		return (irlist[scnstr(irlist, pty)]);
	case Jc:
		return (YES);
	case Nn:
		return (nn(pty));
	default:
		return (pty == code);
		}
	}

/*	do a subscript []
 */
VOID dosub(q, p)
	FAST TERM *q, *p;
	{
	FAST LEX pty;
	INTERN TINY is[] {IIX, IIY, N, NN, INN, 0};
	INTERN TINY was[] {IX, IY, N, NN, INN, 0};

	pty = p->ty & TMASK;
	if (p->base || !nn(pty))
		err("illegal subscript");
	else if (!(pty = is[scnstr(was, q->ty & TMASK)]))
		err("illegal []");
	else
		{
		q->ty = q->ty & ~TMASK | pty;
		q->val =+ p->val;
		}
	}

/*	do a unary operator
 */
VOID dounop(p, t)
	TERM *p;
	LEX t;
	{
	IMPORT TERM *symtab;
	FAST LEX pseg, pty;
	FAST TERM *q;

	p->ty =& ~PUBF;
	pseg = (LEX)p->base;
	pty = p->ty & TMASK;
	switch (t)
		{
	case AND:
		if (pty != INN && !nn(pty))
			err("illegal &");
		else
			p->ty = p->ty & ~TMASK | NN;
		break;
	case INDIR:
		if (pty == N)
			p->ty = p->ty & ~TMASK | INN;
		else if (pty < NN || IY < pty)
			err("illegal *");
		else
			p->ty =| INDF;
		break;
	case PLUS:
		break;
	case MINUS:
		if (pseg || !nn(pty))
			err("illegal -");
		else
			p->val = -p->val;
		break;
	case XOR:
		if (pseg || !nn(pty))
			err("illegal !");
		else
			p->val = ~p->val;
		break;
	case GETS:
		if (pty != INN && !nn(pty) && p->ty != STRING)
			err("illegal =");
		else
			dolit(p);
		break;
	case PUBLIC:
		for (q = symtab; q; q = q->next)
			if (cmpbuf(q->nm, p->nm, LENNAME))
				break;
		if (!q)
			err("illegal public");
		else
			q->ty =| PUBF;
		p->ty =| PUBF;
		break;
	default:
		dobin(p, p, t);
		p->ty =| PUBF;
		}
	}

/*	put an error message
 */
VOID err(fmt, arg)
	TEXT *fmt, *arg;
	{
	IMPORT COUNT lno, nerrors;
	IMPORT FIO stderr;

	putf(&stderr, "%i: ", lno);
	putf(&stderr, fmt, arg);
	putf(&stderr, "\n");
	++nerrors;
	}

/*	get a (possibly putback) char
 */
META gchar()
	{
	IMPORT COUNT lno, nback;
	IMPORT META chstk[];
	FAST META c;

	if (0 < nback)
		return (chstk[--nback]);
	else if ((c = getch()) == '\n')
		{
		++lno;
		return ('\n');
		}
	else
		return (c);
	}

/*	get an escaped character
 */
META getesc(tchar)
	META tchar;
	{
	FAST COUNT i;
	FAST META c;

	switch (c = gchar())
		{
	case EOF:
	case '\n':
		break;
	case '\\':
		if ((c = gchar()) == EOF || c == '\n')
			break;
		else if ((i = scnstr("btnvfr", tolower(c))) < 6)
			return ('\010' + i);
		else if (isdigit(c))
			{
			i = c - '0';
			if (isdig(c = gchar()))
				{
				i = i << 3 | c - '0';
				if (isdig(c = gchar()))
					return (i << 3 | c - '0');
				}
			bchar(c);
			return (i);
			}
		else
			return (c & BYTMASK);
	default:
		return ((c == tchar) ? -1 : c & BYTMASK);
		}
	err("missing %ac", tchar);
	bchar(c);
	return (-1);
	}

/*	get a token from STDIN
 */
LEX gtok()
	{
	IMPORT COUNT tval;
	FAST COUNT shift;
	FAST LEX t;
	FAST META c;

	FOREVER
		{
		c = gchar();
		while (c != '\n' && c != EOF && iswhite(c))
			c = gchar();
		switch (c)
			{
		case EOF:
			return (EOF);
		case '/':
			while ((c = gchar()) != '\n' && c != EOF)
				;
			bchar(c);
			break;
		case ';':
		case '\n':
			return (EOL);
		case '(':
			return (LPAREN);
		case ')':
			return (RPAREN);
		case '[':
			return (LBRACK);
		case ']':
			return (RBRACK);
		case '"':
			while (0 <= (c = getesc('"')))
				strcat(c);
			return (STRING);
		case '\'':
			for (tval = 0; 0 <= (c = getesc('\'')); )
				tval = (tval << 8) + c;
			return (N);
		case '0':
			if (tolow((c = gchar())) == 'x')
				shift = 4, c = '0';
			else
				shift = 3;
			for (tval = 0; isdig(c) || shift == 4 &&
				('a' <= c && c <= 'f' || 'A' <= c && c <= 'F'); c = gchar())
				tval = (tval << shift) + c
					- (('a' <= c) ? 'a'-10 : ('A' <= c) ? 'A'-10 : '0');
			bchar(c);
			return (N);
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			for (tval = c - '0'; isdig(c = gchar()); )
				tval = tval * 10 + c - '0';
			bchar(c);
			return (N);
		case ':':
		case '=':
		case '<':
		case '>':
		case '+':
		case ',':
		case '-':
		case '^':
		case '*':
		case '&':
		case '|':
		case '!':
			if (t = getop(c))
				return (t);
			break;
		default:
			if (isal(c))
				return (getal(c));
			else
				err("illegal character %ac", c);
			}
		}
	}

/*	test for alphanumeric
 */
BOOL isal(c)
	FAST COUNT c;
	{
	return (isalpha(c) || c == '.' || c == '_');
	}

/*	test for digit
 */
BOOL isdig(c)
	FAST COUNT c;
	{
	return (isdigit(c));
	}

/*	test for N or NN
 */
BOOL nn(ty)
	LEX ty;
	{
	return (ty == N || ty == NN);
	}

/*	scan sorted table for name match
 */
LEX scntab(p, hi, s, n)
	PRETAB *p;
	BYTES hi;
	TEXT *s;
	BYTES n;
	{
	FAST BYTES j;
	FAST TEXT *q, *r;
	BYTES i, lo;
	COUNT x;

	for (lo = 0; lo < hi; )
		{
		i = (hi + lo) >> 1;
		r = p[i].prename;
		if (!(x = *r++ - n))
			for (j = 0, q = s; j < n; ++j)
				if (x = *r++ - *q++)
					break;
		if (x < 0)
			lo = i + 1;
		else if (x == 0)
			return (p[i].pretype);
		else
			hi = i;
		}
	return (0);
	}

/*	concatenate to static string
 */
VOID strcat(c)
	COUNT c;
	{
	IMPORT TEXT string[];

	if (string[0] < MAXSTR-1)
		string[++string[0]] = c;
	else
		{
		err("string too long");
		string[0] = 0;
		}
	}

/*	convert to lowercase
 */
META tolow(c)
	FAST META c;
	{
	return (tolower(c));
	}
