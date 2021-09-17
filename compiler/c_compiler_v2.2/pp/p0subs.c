/*	DEFINE/INCLUDE SUBROUTINES
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int0.h"

/*	the hash table definitions
 */
LOCAL DEF *htable[NHASH] {0};

/*	allocate a new token for the tlist and copy a previous into it
 */
TLIST *buytl(p, link)
	FAST TLIST *p, *link;
	{
	FAST TLIST *q;

	q = alloc(sizeof (*q), link);
	q->type = p->type;
	q->white = p->white;
	q->nwhite = p->nwhite;
	q->text = p->text;
	q->ntext = p->ntext;
	return (q);
	}

/*	expand a single define line
 */
TLIST *dodef(pd, qn, args)
	TLIST *pd, *qn;
	ALIST *args;
	{
	FAST TLIST **qb, *r, *s;
	ALIST *pa;
	COUNT i, nargs;
	TLIST *pe, *px, *qs;

	qb = &qs;
	nargs = 0;
	if (pd->nwhite || !punct(pd, '('))
		r = pd;
	else
		{
		r = pd->next;
		if (r->type == PIDENT)
			{
			nargs = 1;
			r = r->next;
			}
		while (r && !punct(r, ')'))
			if (punct(r, ',') && r->next->type == PIDENT)
				{
				r = r->next->next;
				++nargs;
				}
			else
				r = NULL;
		if (!r)
			{
			perror("bad #define arguments");
			r = pd;
			}
		else
			r = r->next;
		}
	for (pe = r; r->type != PEOL; )
		{
		if (r->type == PIDENT)
			for (s = pd->next, i = 0; i < nargs; ++i)
				{
				if (r->ntext == s->ntext && cmpbuf(r->text, s->text, s->ntext))
					break;
				s = s->next->next;
				}
		if (r->type == PIDENT && i < nargs)
			{
			for (pa = args; 0 < i && pa; --i)
				pa = pa->next;
			if (pa)
				for (px = pa->astart; px != pa->aend; px = px->next)
					{
					*qb = buytl(px, r->next);
					if (px == pa->astart)
						{
						(*qb)->white = r->white;
						(*qb)->nwhite = r->nwhite;
						}
					qb = &(*qb)->next;
					}
			r = free(r, r->next);
			}
		else
			{
			*qb = r;
			qb = &(*qb)->next;
			r = r->next;
			}
		}
	while (pd != pe)
		pd = free(pd, pd->next);
	*qb = qn;
	return (qs);
	}

/*	expand #defines in a line of tokens
 */
TLIST *doexp(qs)
	TLIST *qs;
	{
	FAST TLIST *pd, *q, **qb;
	ALIST *args;
	TEXT *s;
	TLIST *qn;

	for (qb = &qs; q = *qb; )
		if (q->type != PIDENT || !(s = lookup(q->text, q->ntext))
			|| *s == '(' && (q->next->nwhite || !punct(q->next, '(')))
			qb = &q->next;
		else
			{
			pd = stotl(s);
			args = NULL;
			if (*s == '(')
				qn = getargs(q->next, &args);
			else
				qn = q->next;
			*qb = dodef(pd, qn, args);
			if (*qb != qn)
				{
				(*qb)->white = q->white;
				(*qb)->nwhite = q->nwhite;
				}
			frelst(args, NULL);
			frelst(q, qn);
			}
	return (qs);
	}

/*	format error messages
 */
VOID errfmt(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT FILE errfd;
	FAST BYTES *p;
	FAST TEXT *f, *s;
	TEXT buf[10];

	for (p = &args, f = fmt; *f; ++f)
		if (*f != '%')
			write(errfd, f, 1);
		else if (*++f == 'c')
			{
			*buf = *p++;
			write(errfd, buf, 1);
			}
		else if (*f == 's')
			write(errfd, buf, itob(buf, *p++, 10));
		else
			{
			s = *p++;
			write(errfd, s, (*f == 'b') ? *p++ : lenstr(s));
			}
	}

/*	get an argument list
 */
TLIST *getargs(ip, ap)
	TLIST *ip;
	ALIST **ap;
	{
	FAST ALIST **qb, *q;
	FAST TLIST *p;
	COUNT npar;

	p = ip;
	for (qb = ap, p = p->next; p && !punct(p, ')'); qb = &q->next)
		{
		*qb = q = alloc(sizeof (**ap), NULL);
		q->astart = p;
		for (npar = 0; p && (0 < npar || !punct(p, ',')
			&& !punct(p, ')')); p = p->next)
			if (punct(p, '('))
				++npar;
			else if (punct(p, ')'))
				--npar;
		if (p)
			{
			q->aend = p;
			if (punct(p, ','))
				p = p->next;
			}
		}
	if (p)
		return (p->next);
	else
		{
		perror("bad macro arguments");
		*ap = NULL;
		return (ip);
		}
	}

/*	get an included file name
 */
TEXT *getfname(p)
	FAST TLIST *p;
	{
	IMPORT TEXT *iprefix;
	FAST TEXT *q, *s;
	BYTES n;
	COUNT i;
	FILE fd;

	if (p->type == PIDENT)
		{
		q = buybuf(p->text, p->ntext + 1);	/* DIRTY */
		q[p->ntext] = '\0';
		}
	else if (p->type == PSTRING)
		{
		q = buybuf(p->text + 1, p->ntext - 1);
		q[p->ntext - 2] = '\0';
		}
	else if (punct(p, '<'))
		{
		for (s = p->next->text; *s != '>' && !iswhite(*s); ++s)
			;
		n = s - p->next->text;
		q = alloc(lenstr(iprefix) + n + 1, 0);
		for (s = iprefix; *s; s =+ (s[i]) ? i + 1 : i) 
			{
			cpybuf(q, s, i = scnstr(s, '|'));
			cpybuf(q + i, p->next->text, n);
			q[i + n] = '\0';
			if (0 <= (fd = open(q, READ, 1)))
				{
				close(fd);
				break;
				}
			}
		}
	else
		q = NULL;
	return (q);
	}

/*	hash into symbol table
 */
DEF **hash(s, n)
	FAST TEXT *s;
	FAST BYTES n;
	{
	IMPORT DEF *htable[];
	FAST COUNT sum;

	for (sum = 0; 0 < n; --n)
		sum =+ *s++;
	return (&htable[sum % NHASH]);
	}

/*	install definition in symbol table
 */
VOID install(s, n, pd)
	TEXT *s;
	FAST BYTES n;
	TEXT *pd;
	{
	FAST DEF **qb, *q;

	n = min(n, LENNAME);
	qb = hash(s, n);
	*qb = q = alloc(sizeof (*q), *qb);
	cpybuf(q->dname, s, n);
	q->dnlen = n;
	q->defn = pd;
	}

/*	lookup a definition in symbol table
 */
TLIST *lookup(s, n)
	FAST TEXT *s;
	FAST BYTES n;
	{
	FAST DEF *q;

	n = min(n, LENNAME);
	for (q = *hash(s, n); q; q = q->next)
		if (n == q->dnlen && cmpbuf(s, q->dname, n))
			return (q->defn);
	return (NULL);
	}

/*	get next file argument
 */
INCL *nxtfile()
	{
	IMPORT BOOL pflag;
	IMPORT BYTES argc;
	IMPORT TEXT **argv;
	FAST FILE fd;
	FAST INCL *pi;

	while ((fd = getfiles(&argc, &argv, STDIN, STDERR)) == STDERR)
		perror("can't open %p", argv[-1]);
	if (fd < 0)
		return (NULL);
	else
		{
		pi = alloc(sizeof(*pi), NULL);
		if (fd == STDIN)
			pi->fname = NULL;
		else
			pi->fname = buybuf(argv[-1], lenstr(argv[-1]) + 1);
		pflag = YES;
		pi->nline = 0;
		finit(&pi->pfio, fd, READ);
		return (pi);
		}
	}

/*	put the argument defines into the buffer
 */
VOID pargs(s, n)
	FAST TEXT *s;
	FAST COUNT n;
	{
	IMPORT INCL *pincl;
	FAST FIO *p;

	p = &pincl->pfio;
	for (; 0 < n && p->_nleft < (BUFSIZE - 1); --n, ++p->_nleft)
		if ((p->_buf[p->_nleft] = *s++) == '\n')
			--pincl->nline;
	if (0 < n && p->_nleft < BUFSIZE)
		{
		p->_buf[p->_nleft++] = '\n';
		perror("too many -d arguments");
		}
	p->_pnext = p->_buf;
	}

/*	put an error message with line numbers
 */
VOID perror(fmt, a0, a1)
	TEXT *fmt, *a0, *a1;
	{
	IMPORT COUNT nerrors;
	IMPORT INCL *pincl;

	if (!pincl)
		errfmt("EOF: ");
	else if (!pincl->fname)
		errfmt("%s: ", pincl->nline);
	else
		errfmt("%p %s: ", pincl->fname, pincl->nline);
	errfmt(fmt, a0, a1);
	errfmt("\n");
	++nerrors;
	}

/*	install predefined args
 */
VOID predef(p)
	FAST ARGS *p;
	{
	IMPORT BYTES pchar;
	FAST COUNT i;
	FAST TEXT *s;
	COUNT n;
	TEXT pch = pchar;

	for (i = 0; p->ntop < NDARGS - i; ++i)
		{
		s = p->anames[(NDARGS - 1) - i];
		pargs(&pch, 1);
		pargs("define ", 7);
		n = scnstr(s, '=');
		pargs(s, n);
		pargs(" ", 1);
		if (s[n] == '\0')
			pargs("1", 1);
		else
			{
			s =+ n + 1;
			pargs(s, lenstr(s));
			}
		pargs("\n", 1);
		}
	}

/*	is this a punctuation mark
 */
BOOL punct(p, c)
	FAST TLIST *p;
	TEXT c;
	{
	return (p->ntext == 1 && *p->text == c);
	}

/*	scan a table of predefined strings
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

/*	convert a line to token list
 */
TLIST *stotl(s)
	FAST TEXT *s;
	{
	FAST TLIST *q, **qb;
	TLIST *qs;

	qb = &qs;
	FOREVER
		{
		q = alloc(sizeof (*q), NULL);
		q->white = s;
		while (iswhite(*s) && *s != '\n')
			++s;
		q->nwhite = s - q->white;
		q->text = s;
		if (*s == '\n')
			{
			++s;
			q->type = PEOL;
			}
		else if (isalpha(*s) || *s == '_' || LENNAME != 8 && *s == '$')
			{
			while (isalpha(*s) || *s == '_' || LENNAME != 8 && *s == '$'
				|| isdigit(*s))
				++s;
			q->type = PIDENT;
			}
		else if (isdigit(*s))
			{
			while (isalpha(*s) || isdigit(*s))
				++s;
			q->type = PNUM;
			}
		else if (*s == '"' || *s == '\'')
			{
			for (; *++s != *q->text && *s != '\n'; )
				if (*s == '\\' && s[1] != '\n')
					++s;
				else if (*s == '\\')
					break;
			if (*s == *q->text)
				++s;
			else
				perror("unbalanced %c", *q->text);
			q->type = (*q->text == '"') ? PSTRING : PCHCON;
			if (STRSIZE < s - q->text)
				{
				perror("string too long");
				q->text = s - STRSIZE;
				}
			}
		else
			{
			++s;
			q->type = PPUNCT;
			}
		q->ntext = s - q->text;
		*qb = q;
		qb = &q->next;
		if (q->type == PEOL)
			return (qs);
		}
	}

/*	remove latest definiton of a symbol
 */
VOID undef(s, n)
	FAST TEXT *s;
	BYTES n;
	{
	FAST DEF **qb, *q;

	n = min(n, LENNAME);
	for (qb = hash(s, n); q = *qb; qb = &q->next)
		if (n == q->dnlen && cmpbuf(s, q->dname, n))
			break;
	if (q)
		{
		free(q->defn, NULL);
		*qb = free(q, q->next);
		}
	}
