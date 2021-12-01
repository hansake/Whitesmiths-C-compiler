/*	GRAPH REDUCTION FOR IBM SYSTEM/36
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

LOCAL BOOL inreach(/* there, here */);
LOCAL VOID setcode(/* q, inst, hdr, size */);

/*	FLAGS:
	-bss	don't use .bss or .comm
	-catg#	define category number (IBM assembler)
	-ck		check stack growth
	-dcatg#	define category number for data if -s flag
	-dm* 	give assembler output data module name * if -s flag
	-do*	assembly data to file * if -s flag
	-dseq*	use file * as sequence number file for data file if -s flag
	-err	don't return failure on error
	(-i		put function enter and leave code inline)
	+list	emit listing tokens for lm
	-m* 	give assembler output module name *
	-o*		assembly code to file *, errors to stdout
	-p		emit profiler calls on function entries
	-s		split text and data into different files
	-x#		4=> code to .text, 2=> lits to .text, 1=> data to .text
 */
#ifdef DEBUG
GLOBAL FIO stderr = {0};
GLOBAL BITS debug = {0};
#endif
GLOBAL BOOL asmflag = {NO};
GLOBAL BOOL bssflag = {NO};
GLOBAL BOOL ckflag = {NO};
GLOBAL BOOL errflag = {NO};
GLOBAL BOOL inemit = {NO};
GLOBAL BOOL lstflag = {NO};
GLOBAL BOOL keepline = {NO};
GLOBAL BOOL pflag = {NO};
GLOBAL BOOL preverr = {NO};
GLOBAL BOOL sflag = {NO};
GLOBAL BYTES entsize = {0};
GLOBAL BYTES retsize = {0};
GLOBAL BYTES widefun = {0};
GLOBAL BYTES yellow = {20};
GLOBAL BYTES xmask = {4};
GLOBAL COUNT categ = {0};
GLOBAL COUNT dcateg = {0};
GLOBAL COUNT lineno = {1};
GLOBAL COUNT lnmarker = {1};
GLOBAL COUNT swskips = {0};
GLOBAL TEXT dblbuf[LENNAME + 8] = {""};
GLOBAL TEXT errbuf[128] = {""};
GLOBAL TEXT funname[LENNAME+2] = {""};
GLOBAL TEXT fname[LENNAME+2] = {""};
GLOBAL TEXT *dmodule = {NULL};
GLOBAL TEXT *module = {NULL};
GLOBAL UTINY dcbyte = {0};
GLOBAL UTINY regbyte = {0};

GLOBAL SPACE fnspace = {0};

GLOBAL TEXT *_pname = {"p2.36"};

/*	FILE CONTROL:
	errfd = error output file descriptor
	ofile = name of assembly output file
	outfd = assembly language output file descriptor
	dofile = name of assembly data output file (if split text/data)
	dseqfile = sequence number file for data references (if split text/data)
	wrkfd = work output from code generator
 */
GLOBAL FILE errfd = {STDERR};
GLOBAL FILE outfd = {STDOUT};
GLOBAL FILE wrkfd = {STDOUT};
GLOBAL TEXT *dofile = {NULL};
GLOBAL TEXT *ofile = {NULL};
GLOBAL TEXT *dseqfile = {"sequence.36"};

/*	tables
 */
GLOBAL EXPR *exlist = {NULL};
GLOBAL HEADER *labtab = {NULL};

/*	available blocks
 */
GLOBAL CODE *tcode = {NULL};
GLOBAL HEADER *theader = {NULL};

/*	buy a code element
 */
CODE *buycode(inst, hdr, size)
	LEX inst;
	HEADER *hdr;
	BYTES size;
	{
	IMPORT CODE *tcode;
	FAST CODE *q;

	if (tcode)
		q = tcode, tcode = q->next;
	else
		q = (CODE *)alloc(sizeof (*q), NULL);
	q->next = NULL;
	q->inst = inst;
	q->c.hdr = hdr;
	q->size = size;
	q->linenum = 0;
	return (q);
	}

/*	create a symbol
 */
LABEL crs()
	{
	INTERN LABEL orgcrs;

	orgcrs += 2;
	return (orgcrs);
	}

/*	set up the function entry code, compute retsize/leave sequence
 */
TEXT *doenter(dblab)
	LABEL dblab;
	{
	IMPORT BOOL ckflag, lstflag, pflag;
	IMPORT BYTES entsize, retsize, yellow;
	IMPORT LONG autmin;
	IMPORT TEXT cword[], cwordend[], funname[];
	IMPORT TEXT *module;
	COUNT entlab, prof, size;
	TEXT *s;
	INTERN TEXT nn1[4], nn3[4];
	INTERN TEXT sbuf[128];

	entsize = 0;
	prof = pflag ? crs() : 0;
	if (prof)
		{
		csect(ISDATA);
		genlabel(lbln(prof));
		genword(0);
		}
	csect(ISTEXT);
	genlabel(funname);
	size = autmin + AUTBIAS - FPOFFSET;
	if (NO) /* inline return code */
		{
		putasm("^a @fp+1,#xr1;");
		putasm("^st %i(,#xr1),#arr;", FPOFFSET + 3);
		putasm("^mvc %i(2,#xr1),@fp+1;", FPOFFSET + 1);
		putasm("^st @fp+1,#xr1;");
		entsize += 16;
		}
	else
		{
		entlab = crs();
		putasm("^la %n,#xr2;", entlab);
		if (ckflag)
			putcall("^bd", "cents");
		else
			putcall("^bd", "cent");
		putasm("%n:", entlab);
		entsize += 8;
		}
	if (prof)
		{
		putasm("^la %n,#wr4;", prof);
		putcall("^b", "count");
		entsize += 8;
		}
	if (dblab)
		{
		putasm("^la %i,#wr6;^la %p,#wr7;", size, lbln(dblab));
		putcall("^b", "dbfe");
		s = sjmp("^b", "dbfl");
		}
	else
		{
		if (NO) /* inline return code */
			{
			cpystr(&nn1[0], nn(FPOFFSET + 1, -10), NULL);
			cpystr(&nn3[0], nn(FPOFFSET + 3, -10), NULL);
			cpystr(&sbuf[0],
				"^l ", &nn3[0], "(,#xr1),#arr;",
				"^l ", &nn1[0], "(,#xr1),#xr1;",
				"^st @fp+1,#xr1;",
				"^b 0(,#arr);",
	 			NULL);
			s = sbuf;
			retsize = 13;
			}
		else
			{
			s = sjmp("^b", "cret");
			retsize = 4;
			}
		}
	return (s);
	}

/*	test if address in reach of short branch
 */
BOOL inreach(there, here)
	BYTES there, here;
	{
	FAST COUNT diff;

	if (!there)
		return (NO);
	else
		{
		diff = there - here;
		return (-250 <= diff && diff <= 258);
		}
	}

/*	lookup a label in the header
 */
HEADER *lookup(label)
	FAST LABEL label;
	{
	IMPORT HEADER *labtab, *theader;
	FAST HEADER *p, **qb;

	for (qb = &labtab; p = *qb; qb = &p->next)
		if (p->label == label)
			return (p);
	if (theader)
		p = theader, theader = p->next;
	else
		p = (HEADER *)alloc(sizeof (*p), NULL);
	p->next = NULL;
	*qb = p;
	p->first = NULL;
	p->label = label;
	p->offset = 0;
	p->nrefs = 0;
	p->flags = NEWLAB;
	return (p);
	}

/*	build graphs and reduce them
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
#ifdef DEBUG
	IMPORT BITS debug;
#endif
	IMPORT SPACE fnspace;
	RESOURCE regset;
	IMPORT BOOL bssflag, ckflag, errflag, pflag, lstflag, 
		keepline, preverr, sflag;
	IMPORT BYTES choff, widefun, xmask;
	IMPORT COUNT categ, dcateg, lineno, swskips;
	IMPORT EXPR exzero;
	IMPORT FILE errfd;
#ifdef DEBUG
	IMPORT FIO stderr;
#endif
	IMPORT HEADER *labtab;
	IMPORT LONG autmin, autoff;
	IMPORT TEXT funname[], runname[];
	IMPORT TEXT *dmodule, *dofile, *module, *ofile, *dseqfile;
	IMPORT TEXT *_pname, fname[];
	IMPORT UTINY dcbyte, regbyte;
	FAST HEADER *p;
	FAST CODE **qb;
	CODE **qbb;
	COUNT fnlnmrk, nlabels, swtsize;
	EXPR *r, *s;
	LABEL dblab, l;
	LEX tok;
	LONG savaut;
	TEXT *leave;

#ifdef DEBUG
	finit(&stderr, STDERR, WRITE);
	getflags(&ac, &av,
	  "bss,catg#,ck,dcatg#,do*,dm*,dseq*,+d#,err,+list,m*,o*,p,s,x#:F <file>",
		&bssflag, &categ, &ckflag, &dcateg, &dofile, &dmodule, &dseqfile,
		&debug, &errflag, &lstflag, &module, &ofile, &pflag, &sflag,
		&xmask);
#else
	getflags(&ac, &av,
		"bss,catg#,ck,dcatg#,do*,dm*,dseq*,err,+list,m*,o*,p,s,x#:F <file>",
		&bssflag, &categ, &ckflag, &dcateg, &dofile, &dmodule, &dseqfile,
		&errflag, &lstflag, &module, &ofile, &pflag, &sflag, &xmask);
#endif
	xmask &= 7;
	setmap(xmask);
	if (0 < ac)
		{
		close(STDIN);
		if (open(av[0], READ, 1) != STDIN)
			panic("bad input file");
		}
	if (ofile)
		{
		if ((outfd = create(ofile, WRITE, 0)) < 0)
			panic("bad output file");
		else
			errfd = STDOUT;
		}
	wrkfd = outfd;
	setad(&exzero, YSHORT, 0, 0);
	cpyad(&exzero.e.v, &exzero.f);
	swskips = 0;
	if ((dcbyte = needch()) != P2MAGIC)
		perror("intermediate file < release 3.0 ", av[0]);
	dcbyte = needch();
	if (dcbyte & ~034)
		perror("bad p1 flags for System/36 target ", av[0]);
	swtsize = 7;
	if (preverr)
		return (NO);
	regbyte = needch();
	if (regbyte & ~03)
		perror("-a or -r# too high in p1 flags ", av[0]);
	puthdr();
	csect(ISTEXT);
	for (tok = gcode(); tok != EOF && tok; )
		{
		if (tok != GFUNC)
			panic("NO FUNC");
		gname(funname);
		fnlnmrk = lineno;
		keepline = YES;
		getspinfo(&fnspace, &widefun, funname);
		regset = AC0|XR2|F0;
		autmin = 0;
		autoff = 0;
		for (p = labtab; p; p = frehlst(p))
			freqlst(p->first);
		labtab = NULL;
		nlabels = 0;
		lookup(0);
		labtab->flags |= ISCASE;
		labtab->label = crs();
		qb = &labtab->first;
		dblab = 0;
		choff = 1;
		while ((tok = gcode()) != GFUNC && tok != GFEND && tok != EOF && tok)
			switch (tok)
				{
			case GDBFUNC:
				dblab = glabel();
				break;
			case GDBSTAT:
				l = glabel();
				if (dblab && qb)
					{
					*qb = buycode(GCODE, NULL, 6);
					(*qb)->c.choff = choff;
					chput(" la #wr7,", nn(l, 10), ";^b ",
						runname, "~dbst;`", NULL);
					qb = &(*qb)->next;
					}
				break;
			case GAUTOFF:
				gint(&autoff);
				if (autoff < autmin)
					autmin = autoff;
				break;
			case GREGS:
				regset = needch();
				break;
			case GJUMP:
				l = glabel();
				if (qb)
					{
					*qb = buycode(GJUMP, lookup(l), JMPSIZE);
					++(*qb)->c.hdr->nrefs;
					}
				setlines(qb);
				qb = NULL;
				break;
			case GSWITCH:
			case GLSWITCH:
				l = glabel();
				if (qb)
					{
					*qb = buycode(tok, NULL, swtsize);
					(*qb)->c.lbl = l;
					}
				if (0 < swskips || !qb)
					++swskips;
				setlines(qb);
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
				if (p->flags & NEWLAB)
					relabel(p);
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
					p->flags |= ISCASE;
					if (!swskips)
						++p->nrefs;
					qb = (0 < swskips && !qb) ? NULL : &p->first;
					}
				break;
			case GRET:
				if (qb)
					*qb = buycode(GRET, NULL, RETSIZE);
				setlines(qb);
				qb = NULL;
				break;
			case GVOID:
				savaut = autoff;
				r = gexpr();
				qbb = qb;
				if (qb)
					qb = voidex(qb, r);
				autoff = savaut;
				setlines(qbb);
				break;
			case GLESS:
			case GLEQ:
			case GGREAT:
			case GGEQ:
			case GISEQ:
			case GNOTEQ:
				qbb = qb;
				p = lookup(glabel());
				savaut = autoff;
				r = gexpr();
				s = gexpr();
				dbprint("/compare in main %O %A %A", tok, r, s);
				if (qb)
					qb = jcond(qb, tok, p, r, s);
				autoff = savaut;
				setlines(qbb);
				break;
			case GSDBDATA:
#ifdef DEBUG
				chput("/ start debug data", NULL);
#endif
				break;
			case GEDBDATA:
#ifdef DEBUG
				chput("/ end debug data", NULL);
#endif
				break;
			default:
				panic("BAD INPUT");
				}
		setlines(qb);
		for (p = labtab; p; p = p->next)
			if ((p->flags & (ISCASE|VISITED)) == ISCASE)
				{
				p->flags |= VISITED|TOEMIT;
				visit(p->first, nlabels);
				}
		keepline = NO;
		asmflag = YES;
		dolist(fnlnmrk);
		inemit = YES;
		asmflag = NO;
		leave = doenter(dblab);
		shorten(labtab);
		emit(labtab, leave);
		if (tok == GFEND)
			tok = gcode();
		}
	putlit(NULL);
	xref(NULL);
	putftr();
	putchr(-1);
	return (errflag || lstflag || !preverr);
	}

/*	modify stack pointer
 */
TEXT *msp(off)
	COUNT off;
	{
	IMPORT LONG autoff;

dbfmt("/msp  autoff: %i off: %i\n", (COUNT)autoff, off);
	autoff += off;
dbfmt("  autoff: %i\n", (COUNT)autoff);
	return (NULL);
	}

/*	set values in code cell
 */
VOID setcode(q, inst, hdr, size)
	FAST CODE *q;
	LEX inst;
	HEADER *hdr;
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
	IMPORT BYTES entsize, jmpidx, retsize;
	IMPORT TINY brops[], bncops[], jncops[];
	FAST CODE *q, *retptr;
	FAST HEADER *p;
	BYTES osize, pc, retpc;
	COUNT better, i;

	FOREVER
		{
		better = 0;
		pc = entsize;
		retpc = 0;
		for (p = tab; p; p = p->next)
			{
			if (p->flags & TOEMIT)
				{
				p->offset = pc;
				for (q = p->first; q; q = q->next)
					{
					if ((q->inst == GRET || q->inst == GHDR) && q->c.hdr)
						q->c.hdr->offset = pc;
					if (q->inst == GRET)
						q->size = retsize;
					pc += q->size;
					}
				}
			}
		for (p = tab; p; p = p->next)
			if (p->flags & TOEMIT)
				for (pc = p->offset, q = p->first; q; pc += osize, q = q->next)
					{
					osize = q->size;
					switch (q->inst)
						{
					case BJUMP:
					case GSWITCH:
					case GLSWITCH:
					case GRET:
					case GCODE:
					case GNOP:
					case GHDR:
					case GJUMP:
						break;
					default:
						if ((q->next->inst == BJUMP ||
							q->next->inst == GJUMP) &&
							q->c.hdr->offset == pc + q->size + q->next->size)
							{
							++better;
							--q->c.hdr->nrefs;
							i = scnstr(brops, q->inst);
							if (q->next->inst == GJUMP)
								setcode(q, jncops[i], q->next->c.hdr, JCSIZE);
							else
								setcode(q, bncops[i], q->next->c.hdr, BRSIZE);
							setcode(q->next, GNOP, NULL, 0);
							}
						}
					switch (q->inst)
						{
					case BJUMP:
					case GJUMP:
					case GLESS:
					case GLEQ:
					case GGREAT:
					case GGEQ:
					case GISEQ:
					case GNOTEQ:
					case GLOW:
					case GLOS:
					case GHIGH:
					case GHIS:
						if (!inreach(q->c.hdr->offset, pc))
							;
						else if (q->c.hdr->offset - pc == q->size)
							{
							++better;
							--q->c.hdr->nrefs;
							setcode(q, GNOP, NULL, 0);
							}
						else if (q->inst != BJUMP)
							{
							++better;
							setcode(q, brops[scnstr(&brops[jmpidx], q->inst)],
								q->c.hdr, BRSIZE);
							}
						break;
					case GRET:
						if (retsize < BRSIZE || q->c.hdr || !retptr || !inreach(retpc, pc))
							;
						else if (!retptr->c.hdr)
							{
							++better;
							q->c.hdr = lookup(crs());
							setcode(retptr, BJUMP, q->c.hdr, BRSIZE);
							++q->c.hdr->nrefs;
							}
						else
							{
							++better;
							setcode(q, BJUMP, retptr->c.hdr, BRSIZE);
							++q->c.hdr->nrefs;
							}
						if (q->inst == GRET)
							{
							retptr = q;
							retpc = pc;
							}
						break;
						}
					}
		if (!better)
			return;
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
		case GRET:
		case GSWITCH:
		case GLSWITCH:
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
					p->flags |= VISITED;
					}
			else if (!(p->flags & VISITED))
					{
					p->flags |= VISITED|TOEMIT|NAILED;
					visit(p->first, nlbl);
					}
			}
	}

