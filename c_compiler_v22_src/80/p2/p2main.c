/*	GRAPH REDUCTION
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	FLAGS:
	-ck	check stack growth
	-f	enable far function calls
	-o*	assembly code to *, errors to stdout
	-p	emit profiler calls on function entries; also enable debug output
	-x#	4 => code to .cseg, 2=> to .lits, 1=> to .data
 */
GLOBAL BOOL ckflag {NO};
GLOBAL BOOL farflag {NO};
GLOBAL BOOL pflag {NO};
GLOBAL BYTES xmask {6};
GLOBAL TEXT *ofile {NULL};

BYTES lineno {0};
BYTES yellow {0100};	/* stack "used" per func in addition to autos */
TEXT funname[LENNAME+2] {""};

TEXT *_pname {"p2.80"};

/*	FILE CONTROL:
	errfd = error output file descriptor
	outfd = assembly language output file descriptor (ofile)
 */
GLOBAL FILE errfd {STDERR};
GLOBAL FILE outfd {STDOUT};

/*	tables
 */
GLOBAL EXPR *exlist {NULL};
LOCAL HEADER *labtab {NULL};
LOCAL COUNT nlabels {0};

/*	buy a code element
 */
CODE *buycode(inst, hdr, size)
	LEX inst;
	CODE *hdr;
	BYTES size;
	{
	FAST CODE *q;

	q = alloc(sizeof (*q), NULL);
	q->inst = inst;
	q->c.hdr = hdr;
	q->size = size;
	return (q);
	}

/*	create a symbol
 */
LABEL crs()
	{
	INTERN LABEL orgcrs;

	orgcrs =+ 2;
	return (orgcrs);
	}

/*	get body of a function, counting labels
 */
LEX gtbody(qb)
	FAST CODE **qb;
	{
	IMPORT BITS regmin, regset;
	IMPORT BYTES lineno;
	IMPORT COUNT nlabels;
	IMPORT BYTES autmin, autoff;
	FAST HEADER *p;
	COUNT swskips;
	EXPR *r, *s;
	LABEL l;
	LEX tok;
	LONG lo;

	nlabels = 0;
	swskips = 0;
	while ((tok = gcode()) != GFUNC && tok != EOF && tok)
		switch (tok)
			{
		case GAUTOFF:
			gint(&lo);
			autoff = (BYTES)lo - AUTOFF;
			if (autoff < autmin)
				autmin = autoff;
			break;
		case GREGS:
			regset = needch();
			regmin =& regset;
			break;
		case GJUMP:
			l = glabel();
			if (qb)
				{
				*qb = buycode(GJUMP, lookup(l), JMPSIZE);
				++(*qb)->c.hdr->nrefs;
				}
			qb = NULL;
			break;
		case GSWITCH:
			l = glabel();
			if (qb)
				*qb = buycode(GSWITCH, l, SWTSIZE);
			if (0 < swskips || !qb)
				++swskips;
			qb = NULL;
			break;
		case GSWEND:
			if (0 < swskips)
				--swskips;
			break;
		case GCASE:
		case GLABEL:
			++nlabels;
			p = lookup(glabel());
			lineno = glabel();
			p->lineno = lineno;
			if (qb)
				{
				*qb = buycode(GJUMP, p, JMPSIZE);
				++p->nrefs;
				}
			if (tok != GCASE)
				qb = &p->first;
			else
				{
				p->flags =| ISCASE;
				++p->nrefs;
				qb = (0 < swskips) ? NULL : &p->first;
				}
			break;
		case GRET:
			if (qb)
				*qb = buycode(GRET, NULL, RETSIZE);
			qb = NULL;
			break;
		case GVOID:
			r = gexpr();
			if (qb)
				qb = void(qb, r);
			break;
		case GLESS:
		case GLEQ:
		case GGREAT:
		case GGEQ:
		case GISEQ:
		case GNOTEQ:
			p = lookup(glabel());
			r = gexpr();
			s = gexpr();
			if (qb)
				qb = jcond(qb, tok, p, r, s);
			break;
		default:
			panic("BAD INPUT");
			}
	return (tok);
	}

/*	lookup a label in the header
 */
HEADER *lookup(label)
	FAST LABEL label;
	{
	IMPORT HEADER *labtab;
	FAST HEADER *p, **qb;

	for (qb = &labtab; p = *qb; qb = &p->next)
		if (p->label == label)
			return (p);
	p = alloc(sizeof (*p), NULL), *qb = p;
	p->first = NULL;
	p->label = label;
	p->offset = 0;
	p->nrefs = 0;
	p->flags = 0;
	return (p);
	}

/*	build graphs and reduce them
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BITS regmin, regset;
	IMPORT BOOL ckflag, farflag, pflag;
	IMPORT BYTES choff, xmask, yellow;
	IMPORT COUNT nlabels;
	IMPORT EXPR exzero;
	IMPORT FILE errfd;
	IMPORT HEADER *labtab;
	IMPORT BYTES autmin, autoff;
	IMPORT TEXT funname[], *ofile;
	FAST HEADER *p;
	COUNT size;
	LABEL prof;
	LEX tok;
	TEXT *leave;

	getflags(&ac, &av, "ck,f,o*,p,x#:F <file>",
		&ckflag, &farflag, &ofile, &pflag, &xmask);
	if (0 < ac)
		{
		close(STDIN);
		if (open(av[0], READ, 1) != STDIN)
			panic("bad input file");
		}
	if (ofile)
		if ((outfd = create(ofile, WRITE, 0)) < 0)
			panic("bad output file");
		else
			errfd = STDOUT;
	exzero.e.v.ty = XSHORT;
	for (tok = gcode(); tok != EOF && tok; )
		{
		if (tok != GFUNC)
			panic("NO FUNC");
		gname(funname);
		regset = REGSET;
		regmin = REGSET|TS;
		autmin = -AUTOFF;
		autoff = -AUTOFF;
		for (p = labtab; p; p = free(p, p->next))
			frelst(p->first, NULL);
		labtab = NULL;
		lookup(0);
		labtab->flags =| ISCASE;
		choff = 1;
		tok = gtbody(&labtab->first);
		for (p = labtab; p; p = p->next)
			if ((p->flags & (ISCASE|VISITED)) == ISCASE)
				{
				p->flags =| VISITED|TOEMIT;
				visit(p->first, nlabels);
				}
		shorten(labtab);
		if (pflag)
			{
			prof = crs();
			csect(ISDATA);
			putasm("%n:\n&0\n", prof);
			}
		csect(ISTEXT);
		putasm("%p:\n", funname);
		autmin =& ~1;
		if (ckflag)
			{
			putasm("hl = &%o\ncall c.entx\n", autmin - yellow);
			leave = "jmp c.rets\n";
			}
		else if ((regmin & REGSET) != REGSET)
			{
			autmin =+ AUTOFF;
			if (farflag)
				{
				putasm("hl = &%o\ncall c.fents\n", autmin);
				leave = "jmp c.frets\n";
				}
			else
				{
				putasm("call c.ents\n");
				leave = "jmp c.rets\n";
				}
			}
		else
			{
			if (autmin == -AUTOFF)
				autmin = 0;
			if (farflag)
				{
				putasm("hl = &%o\ncall c.fent\n", autmin);
				leave = "jmp c.fret\n";
				}
			else
				{
				putasm("call c.ent\n");
				leave = "jmp c.ret\n";
				}
			}
		if (autmin && !farflag)
			putasm("%p\n", msp(autmin));
		if (ckflag && farflag)
			putasm("hl = &%o\ncall c.fmsp\n", autmin);
		if (pflag)
			putasm("hl = &%n\ncall c.count\n", prof);
		emit(labtab, leave);
		}
	putch(-1);
	return (YES);
	}

/*	modify stack pointer
 */
TEXT *msp(off)
	COUNT off;
	{
	FAST TEXT *s;
	INTERN TEXT buf[20];

	switch (off)
		{
	case -8:
		s = "af=>sp=>sp=>sp=>sp";
		break;
	case -6:
		s = "af=>sp=>sp=>sp";
		break;
	case -4:
		s = "af=>sp=>sp";
		break;
	case -2:
		s = "af=>sp";
		break;
	case 0:
		s = "";
		break;
	case 2:
		s = "af<=sp";
		break;
	case 4:
		s = "af<=sp<=sp";
		break;
	case 6:
		s = "af<=sp<=sp<=sp";
		break;
	case 8:
		s = "af<=sp<=sp<=sp<=sp";
		break;
	default:
		s = cpystr(buf, "hl=0", NULL);
		s =+ stob(s, off, 8);
		cpystr(s, "+sp->sp", NULL);
		s = buf;
		}
	return (s);
	}

/*	set values in code cell
 */
LOCAL VOID setcode(q, inst, hdr, size)
	FAST CODE *q;
	LEX inst;
	CODE *hdr;
	BYTES size;
	{
	q->inst = inst & BYTMASK;
	q->c.hdr = hdr;
	q->size = size;
	}

/*	shorten branches and returns
 */
VOID shorten(tab)
	HEADER *tab;
	{
	IMPORT TINY brops[], jncops[];
	FAST CODE *q;
	FAST HEADER *p;
	BYTES osize, pc;
	COUNT better, i;

	FOREVER
		{
		better = 0;
		pc = JMPSIZE;
		for (p = tab; p; p = p->next)
			{
			if (p->flags & TOEMIT)
				{
				p->offset = pc;
				for (q = p->first; q; q = q->next)
					{
					if ((q->inst == GRET || q->inst == GHDR) && q->c.hdr)
						q->c.hdr->offset = pc;
					pc =+ q->size;
					}
				}
			}
		for (p = tab; p; p = p->next)
			if (p->flags & TOEMIT)
				for (pc = p->offset, q = p->first; q; pc =+ osize, q = q->next)
					{
					osize = q->size;
					switch (q->inst)
						{
					case GSWITCH:
					case GRET:
					case GCODE:
					case GNOP:
					case GHDR:
					case GJUMP:
						break;
					default:
						if (q->next->inst == GJUMP &&
							q->c.hdr->offset == pc + q->size + q->next->size)
							{
							++better;
							--q->c.hdr->nrefs;
							i = scnstr(brops, q->inst);
							setcode(q, jncops[i], q->next->c.hdr, JCSIZE);
							setcode(q->next, GNOP, NULL, 0);
							}
						}
					switch (q->inst)
						{
					case GJUMP:
					case GLESS:
					case GLEQ:
					case GGREAT:
					case GGEQ:
					case GISEQ:
					case GNOTEQ:
					case GLOW:
					case GHIS:
						if (q->c.hdr->offset - pc == q->size)
							{
							++better;
							--q->c.hdr->nrefs;
							setcode(q, GNOP, NULL, 0);
							}
						}
					}
		if (!better)
			return (pc);
		}
	}

/*	visit all reachable code
 */
VOID visit(qs, nlbl)
	CODE *qs;
	COUNT nlbl;
	{
	FAST CODE *q;
	FAST COUNT i;
	FAST HEADER *p;

	for (q = qs; q; q = q->next)
		switch (q->inst)
			{
		case GSWITCH:
		case GRET:
		case GCODE:
		case GNOP:
			break;
		default:
			for (p = q->c.hdr, i = 0; p->first &&
				p->first->inst == GJUMP && i < nlbl; ++i)
				p = p->first->c.hdr;
			if (p != q->c.hdr)
				{
				--q->c.hdr->nrefs;
				++p->nrefs;
				q->c.hdr = p;
				}
			if (q->inst == GJUMP)
				if (p->flags & (NAILED|ISCASE))
					return;
				else
					{
					p->flags = p->flags & ~TOEMIT | NAILED;
					--p->nrefs;
					setcode(q, GHDR, p, 0);
					q->next = p->first, p->first = NULL;
					if (p->flags & VISITED)
						return;
					p->flags =| VISITED;
					}
			else if (!(p->flags & VISITED))
					{
					p->flags =| VISITED|TOEMIT|NAILED;
					visit(p->first, nlbl);
					}
			}
	}
