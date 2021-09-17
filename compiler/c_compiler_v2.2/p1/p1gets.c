/*	INPUT FOR PASS 1
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int1.h"
#include "int01.h"

/*	the null name
 */
GLOBAL TEXT noname[LENNAME] {""};

/*	token stacking mechanism
 */
#define NTOKQ	3

LOCAL COUNT ntoks {0};
LOCAL TOKEN tokstk[NTOKQ] {0};

/*	is the next token any of a set?
 */
LEX alt(p)
	FAST TINY *p;
	{
	FAST LEX lex;
	TOKEN tok;

	gettok(&tok);
	if (!(lex = p[scnstr(p, tok.type)]))
		baktok(&tok);
	return (lex & BYTMASK);
	}

/*	put a token back onto the input stream stack
 */
VOID baktok(t)
	FAST TOKEN *t;
	{
	IMPORT COUNT ntoks;
	IMPORT TOKEN tokstk[];

	if (ntoks < NTOKQ)
		cpytok(&tokstk[ntoks++], t);
	else
		{
		perror("!TOKEN OVERFLOW");
		exit(NO);
		}
	}

/*	eat a token from input stream
 */
LEX eat(lex)
	FAST LEX lex;
	{
	TOKEN tok;

	if (gettok(&tok)->type != lex)
		{
		baktok(&tok);
		return (0);
		}
	else
		return (lex);
	}

/*	get a char buffered
 */
COUNT getch()
	{
	INTERN COUNT nin {0};
	INTERN TEXT buf[128], *bnext;

	if (!nin)
		nin = read(STDIN, bnext = buf, 128);
	if (nin <= 0)
		return (nin = EOF);
	--nin;
	return (*bnext++ & BYTMASK);
	}

/*	get a series of characters
 */
VOID getstr(s, n)
	FAST TEXT *s;
	FAST COUNT n;
	{
	for (; 0 < n; --n)
		*s++ = needc();
	}

/*	get a token from the stack or input
 */
TOKEN *gettok(t)
	FAST TOKEN *t;
	{
	IMPORT COUNT ntoks;
	IMPORT TOKEN tokstk[];

	if (0 < ntoks)
		return (cpytok(t, &tokstk[--ntoks]));
	else
		return (gtok(t));
	}

/*	get a token from STDIN
 */
TOKEN *gtok(t)
	FAST TOKEN *t;
	{
	IMPORT COUNT lineno;
	IMPORT TEXT *infile;
	FAST COUNT n, i;
	TEXT *f;

	while ((t->type = getch()) == LLINENO || t->type == LIFILE)
		if (t->type == LLINENO)
			getstr(&lineno, 2);
		else
			{
			n = needc();
			f = alloc(n + 1, NULL);
			getstr(f, n);
			f[n] = '\0';
			infile = free(infile, f);
			}
	if (t->type == EOF || !t->type)
		t->type = EOF;
	else if ((t->type & MASK) != CONS)
		;
	else switch (t->type)
		{
	case LDNUM:
		getstr(&t->t.dn, 8);
		break;
	case LIDENT:
		if (LENNAME < (n = needc()))
			n = LENNAME;
		getstr(t->t.an, n);
		while (n < LENNAME)
			t->t.an[n++] = '\0';
		break;
	case LCNUM:
	case LUCNUM:
	case LSNUM:
	case LUSNUM:
	case LLNUM:
	case LULNUM:
		getstr(&t->t.ln, 4);
		break;
	case LSTRING:
		getstr(&t->t.x.slen, 2);
		t->t.x.sptr = alloc(t->t.x.slen, NULL);
		getstr(t->t.x.sptr, t->t.x.slen);
		break;
	default:
		perror("!BAD CHAR");
		exit(NO);
		}
	return (t);
	}

/*	if the next token is an identifier return it
 */
TOKEN *ident(t)
	FAST TOKEN *t;
	{
	if (gettok(t)->type != LIDENT)
		{
		baktok(t);
		t->type = 0;
		return (NULL);
		}
	else
		return (t);
	}

/*	need a particular token
 */
LEX need(lex)
	FAST LEX lex;
	{
	INTERN TINY neelex[] {LLPAREN, LRPAREN, LSCOLON, LCOLON, LLCURLY, LRCURLY,
		LWHILE, LRBRACK, 0};
	INTERN TEXT *neestr[] {"(", ")", ";", ":", "{", "}", "while", "]", "???"};

	if (!eat(lex))
		{
		nmerr("missing", neestr[scnstr(neelex, lex)]);
		return (0);
		}
	return (lex);
	}

/*	need a character; if EOF complain and exit
 */
COUNT needc()
	{
	FAST COUNT c;

	if ((c = getch()) == EOF)
		{
		perror("!EOF");
		exit(NO);
		}
	return (c);
	}

/*	put error message and name
 */
VOID nmerr(s, name)
	TEXT *s, *name;
	{
	IMPORT COUNT lineno, nerrors;
	IMPORT FILE errfd;
	IMPORT TEXT *infile;
	TEXT nbuf[6];

	nbuf[itob(nbuf, lineno, 10)] = '\0';
	putstr(errfd, infile, infile[0] ? " " : "", nbuf,
			": ", s, " ", NULL);
	write(errfd, name, inbuf(name, LENNAME, "\0"));
	write(errfd, "\n", 1);
	++nerrors;
	}

/*	peek ahead in the input stream
 */
LEX peek(lex)
	FAST LEX lex;
	{
	TOKEN tok;

	baktok(gettok(&tok));
	return (tok.type == lex ? lex : 0);
	}

/*	put out error message
 */
VOID perror(s)
	TEXT *s;
	{
	IMPORT COUNT lineno, nerrors;
	IMPORT FILE errfd;
	IMPORT TEXT *infile;
	TEXT nbuf[6];

	nbuf[itob(nbuf, lineno, 10)] = '\0';
	putstr(errfd, infile, infile[0] ? " " : "", nbuf,
			": ", s, "\n", NULL);
	++nerrors;
	}

/*	put a character buffered
 */
VOID putch(c)
	COUNT c;
	{
	IMPORT FILE outfd;
	INTERN COUNT nout {0};
	INTERN TEXT buf[128];

	if (nout == 128 || c < 0 && nout)
		{
		if (write(outfd, buf, nout) != nout)
			{
			perror("can't write!");
			exit (NO);
			}
		nout = 0;
		}
	buf[nout++] = c;
	}

/*	recover to the next valid token and put out error msg
 */
VOID recover(s)
	TEXT *s;
	{
	TOKEN tok;

	perror(s);
	while (gettok(&tok)->type != EOF && tok.type != LSCOLON)
		if (tok.type == LRCURLY)
			return;
	baktok(&tok);
	}
