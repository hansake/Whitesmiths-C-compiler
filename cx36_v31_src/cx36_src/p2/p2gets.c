/*	INPUT AND UTILITY ROUTINES
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

/*	type tables
 */
GLOBAL TINY bytype[] = {
	0, 2, 2, 2, 2, 2, 
	4, 4, 4, 8, 8,
	2, 8, 4, -1};
GLOBAL TINY equtype[] = {
	0, YUCHAR, YUCHAR, YUSHORT, YSFIELD, YUSHORT,
	YULONG, YLFIELD, YULONG, YFLOAT, YDOUBLE,
	YUSHORT, YDOUBLE, YLPTRTO, YSTRUCT};
GLOBAL TINY partype[] = {
	0, YSHORT, YSHORT, YSHORT, YUSHORT, YUSHORT,
	YLONG, YULONG, YULONG, YDOUBLE, YDOUBLE,
	YUSHORT, YDOUBLE, YLPTRTO, YSTRUCT};
GLOBAL TINY regtype[] = {
	0, YSHORT, YSHORT, YSHORT, YUSHORT, YUSHORT,
	YLONG, YULONG, YULONG, YFLOAT, YDOUBLE,
	YUSHORT, YDOUBLE, YLPTRTO, YSTRUCT};
GLOBAL TINY witype[] = {
	0, 1, 1, 2, 2, 2, 
	4, 4, 4, 8, 8,
	2, 8, -1, -1};

/*	relational operator mapping tables
 */
GLOBAL TINY cmpops[] = {GLESS, GLEQ, GGREAT, GGEQ, GISEQ, GNOTEQ, 0};
GLOBAL TINY flpops[] = {GGEQ, GGREAT, GLEQ, GLESS, GNOTEQ, GISEQ};
GLOBAL TINY lexops[] = {LLESS, LLEQ, LGREAT, LGEQ, LISEQ, LNOTEQ, 0};
GLOBAL TINY swpops[] = {GGREAT, GGEQ, GLESS, GLEQ, GISEQ, GNOTEQ};
GLOBAL TINY unsops[] = {GLOW, GLOS, GHIGH, GHIS, GISEQ, GNOTEQ};
GLOBAL TINY unzops[] = {GNOP, GISEQ, GNOTEQ, GJUMP, GISEQ, GNOTEQ};

/*	space mappings
 */
typedef struct spm {
	TEXT *nam;
	BITS p1bit;
	BITS p2bit;
	} SPM;

/*	0 means assigned on the fly
 */
LOCAL SPM spmap[] = {
								/*	foreign to p1 */
	{"port", 0, SPORT},
	{"builtin", 0, SBUI},
								/*	known by p1 */
	{"func", TFFUNC, SFUNC},
	{"vol", TFVOL, SVOL},
	{"cons", TFCONST, SCONST},
	{"rval", TFRVAL, SRVAL},
	{"far", TFFAR, SFAR},
	0};

/*	name length limit flag (bug in p1)
 */
LOCAL BOOL shortname = {YES};

/*	the character pool stuff
 */

typedef struct chbuf {
	struct chbuf *next;
	TEXT cbuf[01000];
	 } CHBUF;

GLOBAL CHBUF *chbase = {NULL};

/*	getting the address of the subtree
 */
EXPR *addrof(q)
	FAST EXPR *q;
	{
	if (!q->op)
		{
dbprint("/addrof %E", q);
		if (!q->e.v.refs)
			panic("ADDROF1");
		--q->e.v.refs;
		q->e.v.ty = YUSHORT;
		}
	else if (q->op == DINDIR)
		q = q->e.o.left;
	else
		panic("ADDROF2");
	return (q);
	}

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

/*dbprint("%D", s, n);*/
	for (; 0 < n; n -= i)
		{
		for (i = choff >> 9, qb = &chbase; 0 <= i; --i)
			{
			if (!*qb)
				{
				*qb = (CHBUF *)alloc(sizeof (**qb), NULL);
				(*qb)->cbuf[0777] = '\0';
				}
			q = *qb;
			qb = &(*qb)->next;
			}
		i = choff & 0777;
		i = cpybuf(&q->cbuf[i], s, min(0777 - i, n));
		choff += i;
		if ((choff & 0777) == 0777)
			++choff;
		s += i;
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

/*	put a short or long case value
 */
VOID doswnum(num, islong)
	ULONG num;
	BOOL islong;
	{
	IMPORT TEXT clong[], clongend[],
		cword[], cwordend[];
	COUNT i;

	if (islong)
		genlong(num);
	else
		{
		i = num;
		genword(i);
		}
	putasm(";");
	}

/*	process switch table
 */
LEX doswtab()
	{
	IMPORT COUNT swskips;
	IMPORT TEXT cdef[], ceven[], cref[], crefbss[];
	BOOL islong;
	BYTES num;
	COUNT c, n;
	LABEL tab, def, lbl;
	LONG lower, val;

	n = needch();
	tab = glabel();
	def = glabel();
	num = glabel();
	islong = (glabel() == 4);
	if (!swskips)
		{
		csect(ISTEXT);
		genlabel(lbln(tab), 0);
		}
	FOREVER
		switch (c = gcode())
			{
		case GJMPTAB:
			num = glabel();
			gint(&lower);
			for ( ; num; --num)
				{
				lbl = glabel();
				if (!swskips)
					{
					genaddr(lbln(lbl), 0);
					doswnum(lower, islong);
					lower++;
					}
				}
			break;
		case GSCNTAB:
			num = glabel();
			for ( ; num; --num)
				{
				gint(&val);
				lbl = glabel();
				if (!swskips)
					{
					genaddr(lbln(lbl), 0);
					doswnum(val, islong);
					}
				}
			c = gcode();
		default:
			if (!swskips)
				{
				genword(0);
				genaddr(lbln(def), 0);
				}
			return (c);
			}
	}
		
/*	get a function token, putting data
 */
LEX gcode()
	{
	IMPORT BOOL asmflag, bssflag, preverr, upcase;
	IMPORT BOOL sflag;
	IMPORT COUNT lineno, lnmarker;
	IMPORT TEXT *module;
	IMPORT TEXT fname[], errbuf[];
	IMPORT TEXT cdef[], ceven[], cref[], crefbss[];
	FAST COUNT i, n;
	FAST LEX c;
	SPACE space;
	BYTES size;
	COUNT num;
	LONG lo;
	TEXT name[LENNAME+2], *s;

	FOREVER
		{
		switch (c = getch())
			{
		case LLINENO:
			lineno = glabel();
			break;
		case LIFILE:
			asmflag = YES;
			dolist(lineno);
			asmflag = NO;
			shortname = NO;
			gname(fname);
			shortname = YES;
			fname[0] = ' ';
			lnmarker = 1;
			break;
		case LERROR:
			preverr = YES;
			for (i = glabel(), s = errbuf; i; --i, *s++ = needch())
				;
			*s = '`';
			upcase = NO;
			putasm("%p", errbuf);
			upcase = YES;
			break;
		case GSPNAME:
			makspace(gname(name));
			break;
		case GADDR:
		case GADDR1:
		case GADDR2:
		case GADDR3:
		case GADDR4:
			gname(name);
			gint(&lo);
			num = lo;
			genaddr(name, num);
			putasm(";");
			break;
		case GREF:
			gname(name);
			getspinfo(&space, &size, name);
			csect(ISTEXT);
			putasm((bssflag || funtype(name)) ? cref : crefbss, name, size);
			if (sflag)
				{
				csect(ISDATA);
				putasm((bssflag || funtype(name)) ? cref : crefbss,
					name, size);
				}
			break;
		case GDEF:
			gname(name);
			getspinfo(&space, &size, name);
			/* moved to genlabel */
			break;
		case GLITS:
			n = needch();
			glabel();
			csect(ISLITS);
			gname(name);
			genlabel(name);
			break;
		case GDATA:
			n = needch();
			gname(name);
			if (!getbss(name, &size))
				{
				csect(ISDATA);
				genlabel(name);
				if (size)
					genspace(size);
				}
			break;
		case GFDATA:
			n = needch();
			i = mapspace(glabel());
			gname(name);
			if (!getbss(name, &size))
				{
				csect(ISDATA);
				genlabel(name);
				if (size)
					genspace(size);
				}
			break;
		case GSWTAB:
			return (doswtab());
			break;
		case GINT:
			n = needch();
			gint(&lo);
			num = lo;
			if (n == 1)
				genbyte(num);
			else if (n == 2)
				genword(num);
			else
				genlong(lo);
			break;
		case GFLOAT:
			n = needch();
			for (i = 0; i < 8; ++i)
				name[i] = needch();
			ibmflt(name, n == 8);
			genconst(name, n);
			break;
		case GSTRING:
			n = needch();
			genstring(n);
			break;
		case GDEND:
			break;
		case GSPACE:
			gint(&lo);
			num = lo;
			genspace(num);
			break;
		default:
			return (c);
			}
		}
	}

/*	get data in bss
 */
BOOL getbss(name, psize)
	TEXT *name;
	BYTES *psize;
	{
	IMPORT BOOL bssflag;
	IMPORT TEXT cspace[], cword[];
	FAST BYTES size;
	FAST LEX c;
	LONG lo;

	*psize = 0;
	if (bssflag)
		return (NO);
	for (size = 0; (c = getch()) == GSPACE; size += lo)
		gint(&lo);
	if (c == GDEND)
		{
		csect(ISBSS);
		genlabel(name);
		genspace(size);
		return (YES);
		}
	else
		{
		backch(c);
		*psize = size;
		return (NO);
		}
	}

/*	get the space information for refs, defs, 
 */
VOID getspinfo(pspace, psize, name)
	SPACE *pspace;
	BYTES *psize;
	TEXT name[];
	{
	LONG lo;

	*pspace = mapspace(glabel());
	gint(&lo);
	if (funtype(name) == '\24')
		lo = 1;			/* ANY WIDE FUNTION */
	else
		lo = 0;
	*psize = lo;
	}

/*	get the type and space info for expression and defs and refs
 */
TYPE gettype(space, pspace, pl)
	SPACE *space, *pspace;
	LONG *pl;
	{
	FAST TYPE inty, ty;

	*pl = 0L;
	*pspace = 0;
	*space = 0;
	inty = needch();
	ty = inty & TYPEMASK;
	if (inty & 0200)
		*space = mapspace(glabel());
	if (ty == XLPTRTO || ty == XPTRTO)
		{
		ty = YUSHORT;
		*pspace = mapspace(glabel());
		}
	else if (ty == XSFIELD || ty == XLFIELD)
		{
		ty |= needch() << 4;
		ty |= needch() << 10;
		}
	else if (ty == XSTRUCT)
		gint(pl);
	return (ty);
	}

/*	get an expression tree
 */
EXPR *gexpr()
	{
	IMPORT SPACE fnspace;
	IMPORT BYTES widefun;
	IMPORT EXPR exzero;
	IMPORT TEXT ceven[], cword[], dblbuf[];
	FAST COUNT n;
	FAST EXPR *q;
	FAST TEXT *s;
	UTINY reg;
	SPACE space, pspace;
	TYPE ty;
	EXPR *qq;
	LEX op;
	LONG stsize;

dbprint("{");
	op = needch();
	ty = gettype(&space, &pspace, &stsize);
	if (op)
		n = 0;
	else
		{
		s = gname(NULL);
		n = lenstr(s);
		}
	q = mexpr(n);
	q->op = op;
	q->e.v.ty = ty;
	if (op == DADDR)
		return (gexpr());
	else if (op)
		{
		q->e.o.left = gexpr();
		if (q->op == LQUERY)
			q->e.o.mid = gexpr();
		q->e.o.right = (q->op & BINOP) ? gexpr() : &exzero;
		}
	else
		{
		q->e.v.space = space;
		q->e.v.pspace = pspace;
		q->e.v.nm = q->name;
		cpystr(q->name, s, NULL);
		gint(&q->e.v.bias);
		reg = needch();
		q->e.v.refs = needch();
		if (reg == LONGRET)
			q->e.v.idx = XAC0;
		else if (reg == ARGIDX)
			{
			q->e.v.idx = XXR1;
			q->e.v.bias += argbias();
			if (widefun)
				q->e.v.bias += 2;
			}
		else if (reg == AUTIDX)
			{
			q->e.v.idx = XXR1;
			q->e.v.bias += AUTBIAS;
			}
		else if (reg != RETIDX)
			q->e.v.idx = rtox(reg);
		else if (widefun)
			{
			q->e.v.idx = XXR1;
			q->e.v.refs = 1;
			q->e.v.bias = argbias();
			q->e.v.space = defspace(0, XXR1);
			qq = mexpr(0);
			cpyad(&qq->e.v, &q->e.v);
			qq->e.v.ty = YUSHORT;
			qq->e.v.pspace = defspace(0, XXR1);
			q->op = DINDIR;
			q->e.o.left = qq;
			q->e.o.mid = NULL;
			}
		else if (q->e.v.ty == YDOUBLE || q->e.v.ty == YFLOAT)
			q->e.v.idx = XAC0 /*XF0*/;
		else
			q->e.v.idx = XAC0;
		if ((q->e.v.ty == YDOUBLE || q->e.v.ty == YFLOAT) &&
			!q->e.v.refs && !q->e.v.idx && !q->e.v.bias)
			{
			csect(ISLITS);
			s = lbln(crs());
			genlabel(s);
			ibmflt(dblbuf, YES);
			genconst(dblbuf, 8);
			q->e.v.refs = 1;
			qq = mexpr(lenstr(s));			/* ! */
			cpybuf(&qq->e, &q->e, sizeof (q->e));
			cpystr(qq->name, s, NULL);
			q = qq;
			q->e.v.nm = q->name;
			}
		}
	if (q->e.v.ty == YSTRUCT)
		{
		switch (q->op)
			{
		case 0:
		case DINDIR:
			q = addrof(q);
			break;
		case LGETS:
			q->e.v.ty = q->e.o.left->e.v.ty;
				qq = q->e.o.left;
			if ((q->e.o.right->op == DFNCALL || q->e.o.right->op == DFNONLY) &&
				qq->e.o.ty == YUSHORT &&
				!q->e.o.right->e.o.mid->op && 
				q->e.o.right->e.o.mid->e.v.idx == XVXR1)
				{
				q->e.o.right->e.o.mid = q->e.o.left;
				q = q->e.o.right;
				}
			else
				q->op = GGETSTR;
			break;
		case DFNCALL:
		case DFNONLY:
			fatsfn(q, (BYTES)stsize);
			q->e.v.ty = q->e.o.mid->e.v.ty;
			q->e.o.mid->e.v.ty = q->e.o.left->e.v.ty;
			break;
		case LQUERY:
			q->e.v.ty = YUSHORT;
			break;
		default:
			panic("STROP");
			}
		q->e.v.stsize = stsize;
		}
	else if (q->e.o.ty == YSTRUCT)
		switch (q->op)
			{
		case LGETS:
			q = fatgfn(q);
			break;
		case DFNCALL:
		case DFNONLY:
			fatsfn(q, bytype[q->e.o.ty]);
			}
dbprint("/gexpr %E", q);
	return (SUCC(q));
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

	p = (TEXT *)&l;
	*p++ = needch();
	*p = needch();
	return (l);
	}

/*	get a name
 */
TEXT *gname(s)
	TEXT *s;
	{
	IMPORT TEXT dblbuf[];
	BOOL loclab;
	FAST BYTES i, n;
	FAST TEXT *ss, *dbptr;
	IMPORT BOOL shortname;
	IMPORT COUNT namelen;
	INTERN TEXT nm[LENNAME + 8];

	nm[0] = '\0';
	n = needch();
	if (!s)
		s = nm;
	ss = s;
	if (i = glabel())
		{
		for (*ss++ = '>'; i; ++ss)
			{
			*ss = (i & 07) + '0';
			i >>= 3;
			}
		*ss = '\0';
		loclab = YES;
		}
	else
		loclab = NO;
	if (n)
		{
		n -= 1;
		i = needch();
		if (i & NMVAL)
			{
			if ((i & 07) == NMFRS)
				*ss++ = '\24';
			}
		*ss++ = '<';
		for (i = 0, dbptr = &dblbuf[0]; n;++i, --n)
			{
			*ss = needch();
			*dbptr++ = *ss;
			if (loclab || (!(i < namelen) && shortname))
				*ss = '\0';
			ss++;
			}
		*ss = '\0';
		*dbptr = '\0';
		}
	return (s);
	}

/*	make a space mapping entry
 */
VOID makspace(name)
	TEXT *name;
	{
	IMPORT SPM spmap[];
	FAST SPM *ps;
	INTERN SPACE spno = {TFFAR};

	for (ps = spmap; ps->nam; ++ps)
		if (cmpstr(ps->nam, name + 1))
			break;
	if (!ps->nam)
		perror("unknown space: ", name + 1);
	else if (ps->p1bit)
		perror("!redefined space: ", name + 1);
	else
		ps->p1bit = spno >>= 1;
	}

/*	map a p1 space to p2 segment code
 */
SPACE mapspace(space)
	BITS space;
	{
	IMPORT SPM spmap[];
	FAST SPM *ps;
	FAST BITS ret;

	if (!space)
		return (0);
	for (ret = 0, ps = spmap; ps->nam; ++ps)
		if (ps->p1bit && (space & ps->p1bit) == ps->p1bit)
			ret |= ps->p2bit;
	if (!ret)
		perror("!unknown space modifier", NULL);
	return (ret);
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
