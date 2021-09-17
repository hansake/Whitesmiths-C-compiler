/*	DEFINE/INCLUDE PREPROCESSOR FOR C
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int0.h"

/*	DATA FLOW:
	getl(line)->
	getln(logical line)-> getin(includes)->
	getex(expanded)-> main()-> putgr(if group)->
	putns(non skipped)-> putls(less sharps)-> lex
 */
/*	FLAGS:
	-c => don't drop comments, continues.
	-d name => define name as YES.
	-i name => change prefix for < > files.
	-o name => output to name, errors to STDERR
	-px => use x instead of #.
	-sx => use x in addition to px.
	-x => put lexemes, not lines.
	-6 => put SOH, extra newlines for v6 compiler
 */
LOCAL BOOL cflag {NO};
LOCAL ARGS pdefs {NDARGS};
LOCAL TEXT *ofile {NULL};
GLOBAL BOOL xflag {NO};
GLOBAL BOOL v6flag {NO};
GLOBAL BYTES pchar {'#'};
GLOBAL BYTES schar {'@'};
GLOBAL TEXT *iprefix {"|"};
GLOBAL TEXT *_pname {"pp"};

/*	FILE CONTROL:
	argc = no of file args left
	argv => next filename
	errfd = error file
	pincl-> current I/O structure
	nerrors = no of errors seen
	pflag => name has changed
*/
GLOBAL TEXT **argv {NULL};
GLOBAL BYTES argc {0};
GLOBAL FILE errfd {STDERR};
GLOBAL INCL *pincl {NULL};
GLOBAL COUNT nerrors {0};
GLOBAL BOOL pflag {NO};

/*	the table of predefined #keywords
 */
#define NPPS	18
LOCAL PRETAB pptab[] {
	"\2IF", PIF,
	"\2if", PIF,
	"\4ELSE", PELSE,
	"\4LINE", PLINE,
	"\4else", PELSE,
	"\4line", PLINE,
	"\5ENDIF", PENDIF,
	"\5IFDEF", PIFDEF,
	"\5UNDEF", PUNDEF,
	"\5endif", PENDIF,
	"\5ifdef", PIFDEF,
	"\5undef", PUNDEF,
	"\6DEFINE", PDEFINE,
	"\6IFNDEF", PIFNDEF,
	"\6define", PDEFINE,
	"\6ifndef", PIFNDEF,
	"\7INCLUDE", PINCLUD,
	"\7include", PINCLUD,
	};

/*	get an expanded token list
 */
TLIST *getex()
	{
	IMPORT BYTES pchar, schar;
	IMPORT PRETAB pptab[];
	FAST TLIST *p;
	FAST LEX tok;

	if (p = getin())
		{
		if (punct(p, pchar) || punct(p, schar))
			{
			if (tok = scntab(pptab, NPPS, p->next->text, p->next->ntext))
				{
				p = free(p, p->next);
				p->type = tok;
				}
			else
				p->type = PSHARP;
			}
		switch (p->type)
			{
		case PDEFINE:
		case PUNDEF:
		case PINCLUD:
		case PSHARP:
			break;
		case PIFDEF:
		case PIFNDEF:
			if (p->next->type != PIDENT)
				perror("bad #%b", p->text, p->ntext);
			else if (!lookup(p->next->text, p->next->ntext))
				p->next = frelst(p->next, NULL);
			break;
		default:
			p = doexp(p);
			}
		}
	return (p);
	}

/*	get an included line, as a token list
 */
TLIST *getin()
	{
	IMPORT BOOL pflag;
	IMPORT INCL *pincl;
	FAST TEXT *s;

	FOREVER
		{
		if (!pincl)
			pincl = nxtfile();
		if (!pincl)
			return (NULL);
		else if (s = getln(pincl))
			return (stotl(s));
		else
			{
			fclose(&pincl->pfio);
			free(pincl->fname, NULL);
			pincl = free(pincl, pincl->next);
			pflag = YES;
			}
		}
	}

/*	get a full line into the I/O buffer
 *		sans continuations and comments
 */
TEXT *getln(pi)
	INCL *pi;
	{
	IMPORT BOOL cflag;
	FAST COUNT i;
	FAST FIO *pf;
	FAST TEXT *s;
	COUNT j, k;
	INTERN TEXT buf[BUFSIZE];
	TEXT strchar, *savs;

	strchar = '\0';
	s = buf;
	pf = &pi->pfio;
	for (i = getl(pf, buf, BUFSIZE); 0 < i; )
		if (!cflag && *s == '\\' && 1 < i && s[1] == '\n')
			{
			++pi->nline;
			i = getl(pf, s, BUFSIZE - (s - buf));
			for (savs = s; 0 < i && *s != '\n' && iswhite(*s); --i)
				++s;
			if (savs != s)
				{
				cpybuf(savs, s, i);
				s = savs;
				}
			}
		else if (!cflag && *s == '\\')
			{
			s =+ 2;
			i =- 2;
			}
		else if (!cflag && !strchar && *s == '/' && 1 < i && s[1] == '*')
			{
			for (j = 2; j < i - 1; ++j)
				if (s[j] == '*' && s[j + 1] == '/')
					break;
			if (j < i - 1)
				{
				*s++ = ' ';
				i =- j + 2;
				for (k = 0; k < i; ++k)
					s[k] = s[k + j + 1];
				}
			else if ((i = getl(pf, s + 2, BUFSIZE - (s + 2 - buf))) <= 0)
				{
				perror("missing */");
				break;
				}
			else
				{
				++pi->nline;
				i =+ 2;
				}
			}
		else if (*s == '\n')
			{
			++pi->nline;
			return (buf);
			}
		else
			{
			if (!cflag && (*s == '"' || *s == '\''))
				{
				if (strchar == *s)
					strchar = '\0';
				else if (!strchar)
					strchar = *s;
				else
					;
				}
			++s;
			--i;
			}
	if (s == buf)
		return (NULL);
	else
		{
		perror("truncated line");
		buf[BUFSIZE - 1] = '\n';
		return (buf);
		}
	}

/*	expand define/include/if, perform lexical analysis
 */
BOOL main(ac, av)
	COUNT ac;
	TEXT **av;
	{
	IMPORT ARGS pdefs;
	IMPORT BOOL cflag, xflag, v6flag;
	IMPORT BYTES argc, pchar, schar;
	IMPORT COUNT nerrors;
	IMPORT FILE errfd;
	IMPORT FIO stdout;
	IMPORT INCL *pincl;
	IMPORT TEXT **argv, *iprefix, *ofile;
	FAST TLIST *p;

	argv = av;
	argc = ac;
	getflags(&argc, &argv, "c,d*>i*,o*,p?,s?,x,6:F <files>", &cflag, &pdefs,
		&iprefix, &ofile, &pchar, &schar, &xflag, &v6flag);
	if (pincl = nxtfile())
		predef(&pdefs);
	if (ofile)
		if ((stdout._fd = create(ofile, WRITE, xflag != 0)) < 0)
			error("bad output file");
		else
			{
			errfd = STDOUT;
			stdout._fmode = BWRITE;
			}
	while (p = putgr(getex(), NO))
		{
		perror("misplaced #%b", p->text, p->ntext);
		frelst(p, NULL);
		}
	fclose(&stdout);
	return (nerrors == 0);
	}

/*	put an if group
 */
TLIST *putgr(p, skip)
	FAST TLIST *p;
	FAST BOOL skip;
	{
	FAST BOOL doit;

	while (p && p->type != PELSE && p->type != PENDIF)
		if (p->type != PIF && p->type != PIFDEF && p->type != PIFNDEF)
			{
			if (skip)
				frelst(p, NULL);
			else
				putns(p);
			p = getex();
			}
		else
			{
			if (p->type == PIF)
				doit = skip ? NO : eval(p->next);
			else if (p->type == PIFDEF)
				doit = (p->next != 0);
			else
				doit = (p->next == 0);
			frelst(p, NULL);
			p = putgr(getex(), skip || !doit);
			if (p && p->type == PELSE)
				{
				frelst(p, NULL);
				p = putgr(getex(), skip || doit);
				}
			if (p && p->type == PENDIF)
				{
				frelst(p, NULL);
				p = getex();
				}
			else
				perror("missing #endif");
			}
	return(p);
	}

/*	put a non skipped token line
 */
VOID putns(p)
	FAST TLIST *p;
	{
	IMPORT BOOL pflag;
	IMPORT INCL *pincl;
	FAST TEXT *fname;
	FAST TLIST *q;
	FILE fd;
	COUNT i;

	switch (p->type)
		{
	case PDEFINE:
		if (p->next->type != PIDENT)
			perror("bad #define");
		else
			{
			for (q = p->next; q->next; q = q->next)
				;
			install(p->next->text, p->next->ntext,
				buybuf(p->next->next->white,
				q->text + q->ntext - p->next->next->white));
			}
		break;
	case PUNDEF:
		if (p->next->type != PIDENT)
			perror("bad #undef");
		else
			undef(p->next->text, p->next->ntext);
		break;
	case PINCLUD:
		if (!(fname = getfname(p->next)))
			perror("bad #include");
		else if ((fd = open(fname, READ, 0)) < 0)
			{
			perror("can't #include %p", fname);
			free(fname, NULL);
			}
		else
			{
			pincl = alloc(sizeof (*pincl), pincl);
			pincl->fname = fname;
			pincl->nline = 0;
			finit(&pincl->pfio, fd, READ);
			pflag = YES;
			}
		break;
	case PLINE:
		if (p->next->type != PNUM)
			perror("bad #line");
		else
			{
			btos(p->next->text, p->next->ntext, &i, 10);
			pincl->nline = i;
			if (fname = getfname(p->next->next))
				{
				pflag = YES;
				if (pincl->fname)
					free(pincl->fname, NULL);
				pincl->fname = fname;
				}
			}
		break;
	case PSHARP:
		if (p->next->type != PEOL)
			perror("bad #xxx");
		break;
	default:
		putls(p);
		}
	frelst(p, NULL);
	}
