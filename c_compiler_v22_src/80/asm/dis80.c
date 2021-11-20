/*	DISASSEMBLER FOR 8080 OBJECT FILES
 * 	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

/*	header sizes
 */
#define MAGIC	2
#define HSIZE	16

/*	symbol table sizes
 */
#define VALSIZE	2
#define FLGSIZE	1
#define LENNAME	9

/*	symbol table definitions
 */
#define DATREL	006
#define DEFD	004
#define EXTRN	010
#define TXTREL	005
#define XDEF	007
#define XREF	017

/*	input buffer controls
 */
#define CODBUF	0
#define IBSIZE	64
#define RELBUF	1

/*	output controls
 */
#define BYTLIN	8

/*	symbol table structure
 */
typedef struct {
	UCOUNT val;
	TINY fl;
	TEXT nm[LENNAME];
	} SYMBOL;

/*	input buffer structure
 */
typedef struct {
	LONG off;
	COUNT n;
	UTINY *buf, *chr;
	} IOBUF;

/*	flags:
	-c		produce CP/M assembler - default
	-i		produce ISIS-II assembler
 */
BOOL cfl {NO};
BOOL ifl {NO};
BOOL manyfls {NO};
BOOL success {YES};

/*	decoded header values
 */
UCOUNT bsize {0};
UCOUNT dbias {0};
UCOUNT dsize {0};
UCOUNT sbytes {0};
UCOUNT ssize {0};
UCOUNT tbias {0};
UCOUNT tsize {0};

/*	symbol table definitions
 */
SYMBOL *symtab {NULL};	/* pointer to sorted entries */
COUNT numsym {0};

/*	input buffer
 */
IOBUF ibuf[2] {0};

/*	output definitions
 */
TEXT cseg[4] {"_c_"};
TEXT dseg[4] {"_d_"};

TEXT *_pname {"dis80"};

/*	find actual length of a name
 */
BYTES actlen(s)
	FAST TEXT *s;
	{
	FAST BYTES i;

	for (i = 0; i < LENNAME && *s++; ++i)
		;
	return (i);
	}

/*	check magic number from header
 */
BOOL chkmagic(fd)
	FILE fd;
	{
	UTINY header[MAGIC];

	fread(fd, header, MAGIC);
	if (header[0] != 0x99 && header[1] != 0x14)	/* 8080 specific */
		return (NO);
	else
		return (YES);
	}

/*	disassemble object file
 */
BOOL disasm(fd, fn)
	FILE fd;
	TEXT *fn;
	{
	IMPORT BOOL ifl;
	IMPORT SYMBOL *symtab;
	IMPORT TEXT *mapc();
	IMPORT UCOUNT dbias, dsize, tbias, tsize;

	if (!chkmagic(fd))
		return (remark("bad file format: ", fn));
	gethdr(fd);
	getsym(fd);
	mapsyms(mapc(), dbias);
	putfmt("\t%p\t%b%ac\n", ifl ? "name" : "title", fn, instr(fn, "."),  mapc()[1]);
	putsyms("\tpublic\t%.7b\n", (EXTRN | DEFD), (EXTRN | DEFD));
	putsyms("\textrn\t%.7b\n", XREF, EXTRN);
	putsyms("%.7b\tequ\t0%hsH\n", XDEF, DEFD);
	initbuf();
	if (!putsect(fd, tsize, tbias, "c", TXTREL) ||
		(!putsect(fd, dsize, dbias, "d", DATREL)))
		return (NO);
	putfmt("\tend\n");
	free(symtab, NULL);
	return (YES);
	}

/*	get a code or relocation byte
 */
UTINY getby(fd, idx)
	FILE fd;
	COUNT idx;
	{
	IMPORT IOBUF ibuf[];
	FAST IOBUF *p = &ibuf[idx];

	if (!p->n)
		{
		lseek(fd, p->off, 0);
		p->n = fread(fd, p->buf, IBSIZE);
		if (!p->n)
			error("unexpected EOF", NULL);
		p->off =+ p->n;
		p->chr = p->buf;
		}
	--p->n;
	return (*p->chr++);
	}

/*	get header information from file
 */
VOID gethdr(fd)
	FILE fd;
	{
	IMPORT UCOUNT bsize, dbias, dsize, sbytes;
	IMPORT UCOUNT ssize, tbias, tsize, xstos();
	TEXT bhdr[HSIZE - 2];

	fread(fd, bhdr, HSIZE - 2);
	sbytes = xstos(&bhdr[0]);
	tsize = xstos(&bhdr[2]);
	dsize = xstos(&bhdr[4]);
	bsize = xstos(&bhdr[6]);
	ssize = xstos(&bhdr[8]);
	tbias = xstos(&bhdr[10]);
	dbias = xstos(&bhdr[12]);
	}

/*	get symbol table and set table stats
 */
VOID getsym(fd)
	FILE fd;
	{
	IMPORT COUNT numsym;
	IMPORT SYMBOL *symtab;
	IMPORT UCOUNT dsize, sbytes, tsize, xstos();
	FAST COUNT i;
	FAST SYMBOL *ps;
	COUNT entsize = {VALSIZE + FLGSIZE + LENNAME};

	numsym = sbytes / entsize;
	lseek(fd, (LONG)(tsize + dsize), 1);
	symtab = alloc(sizeof (SYMBOL) * numsym, NULL);
	for (i = 0, ps = symtab; i < numsym; ++i, ++ps)
		{
		fread(fd, ps, entsize);
		ps->val = xstos(&ps->val);
		}
	}

/*	initialize input buffers
 */
VOID initbuf()
	{
	IMPORT IOBUF ibuf[];
	IMPORT UCOUNT dsize, sbytes, tsize;
	FAST IOBUF *p, *q;
	INTERN UTINY bufs[2][IBSIZE];

	p = &ibuf[CODBUF];
	q = &ibuf[RELBUF];
	p->off = (LONG)HSIZE;
	q->off = (LONG)(HSIZE + tsize + dsize + sbytes);
	p->n = 0;
	q->n = 0;
	p->buf = bufs[0];
	q->buf = bufs[1];
	p->chr = p->buf;
	q->chr = q->buf;
	}

/*	process flags and files
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL cfl, ifl, manyfls, success;
	FAST FILE fd;

	getflags(&ac, &av, "c,i:F <files>", &cfl, &ifl);
	if (cfl && ifl)
		usage("only one of -[c i] may be specified\n");
	else if (!ifl)
		cfl = YES;
	manyfls = (1 < ac);
	while (0 <= (fd = getbfiles(&ac, &av, STDIN, STDERR, YES)))
		if (fd == STDERR) 
			success = remark("can't read ", av[-1]);
		else
			{
			if (manyfls)
				{
				putstr(STDOUT, av[-1], NULL);
				putstr(STDOUT, ":\n", NULL);
				}
			success =& disasm(fd, av[-1]);
			close(fd);
			}
	return (success);
	}

/*	map characters for proper output
 */
TEXT *mapc()
	{
	IMPORT BOOL ifl;

	return(ifl ? "?@" : ".$");
	}

/*	map symbols for proper output
 */
VOID mapsyms(s, bias)
	TEXT *s;
	COUNT bias;
	{
	IMPORT COUNT numsym;
	IMPORT SYMBOL *symtab;
	IMPORT TEXT cseg[], dseg[];
	FAST COUNT j;
	FAST TEXT *q;
	FAST SYMBOL *ps;
	COUNT i;

	for (i = 0, ps = symtab; i < numsym; ++i, ++ps)
		{
		for (q = ps->nm, j = LENNAME; 0 <= --j; ++q)
			if (*q == '_')
				*q = s[0];
			else if (*q == '.')
				*q = s[1];
		if ((ps->fl & XDEF) == DATREL)
			ps->val =- bias;
		}
	cseg[0] = s[0];
	cseg[2] = s[0];
	dseg[0] = s[0];
	dseg[2] = s[0];
	}

/*	put out code bytes
 */
VOID putby(c)
	COUNT c;
	{
	INTERN COUNT nb {0};

	if (0 <= c)
		{
		putfmt(nb ? ",0%+02hsH" : "\tdb\t0%+02hsH", c);
		++nb;
		}
	if (BYTLIN <= nb || (c < 0 && nb))
		{
		putfmt("\n");
		nb = 0;
		}
	}

/*	put out a section of code and do code relocation
 */
BOOL putsect(fd, nin, bias, hdr, flags)
	FILE fd;
	UCOUNT nin, bias;
	TEXT *hdr;
	BITS flags;
	{
	IMPORT COUNT numsym;
	IMPORT SYMBOL *symtab;
	IMPORT TEXT cseg[], dseg[];
	IMPORT UCOUNT bsize, dbias, dsize, tbias, tsize;
	FAST BITS b, co;
	FAST UCOUNT nabs;
	SYMBOL *p;
	TEXT *s;
	UCOUNT i, n, val;

	putfmt("\t%pseg\n%p:\n", hdr, *hdr == 'c' ? cseg : dseg);
	putsyms("%.7b\tequ\t$+0%hsH\n", XDEF, flags);
	for (nabs = 0; co = getby(fd, RELBUF); nin =- n)
		{
		if (co < 32)
			{
			n = co;
			nabs =+ n;
			}
		else if (co < 64)
			{
			n = ((co - 32) << 8) + getby(fd, RELBUF) + 32;
			nabs =+ n;
			}
		else
			{
			for ( ; nabs; --nabs)
				putby(getby(fd, CODBUF));
			b = (co >> 2) - 16;
			if (b == 47)
				{
				b =+ getby(fd, RELBUF);
				if (175 <= b)
					b = ((b - 175) << 8) + getby(fd, RELBUF) + 175;
				}
			switch (b)
				{
			case 0:
				val = 0;
				s = "";
				break;
			case 1:
				val = -tbias;
				s = cseg;
				break;
			case 2:
				val = -dbias;
				s = dseg;
				break;
			case 3:
				return (remark("can't handle .bss"));
				break;
			default:
				if (numsym <= (b =- 4) || !(p = symtab + b))
					return (remark("bad symbol number"));
				if (p->fl & DEFD)
					val = p->val;
				else
					val = 0;
				s = p->nm;
				break;
				}
			if (co & 01)
				val =- bias;
			i = getby(fd, CODBUF);
			i =| getby(fd, CODBUF) << 8;
			val =+ i;
			n = 2;
			putby(-1);
			putfmt("\tdw\t%.7p", s);
			if (val)
				putfmt("+0%hsH", val);
			putfmt("\n");
			}
		}
	nabs =+ nin;
	for ( ; nabs; --nabs)
		putby(getby(fd, CODBUF));
	putby(-1);
	return (YES);
	}

/*	output symbols
 */
VOID putsyms(fmt, mask, val)
	TEXT *fmt;
	BITS mask, val;
	{
	IMPORT COUNT numsym;
	IMPORT SYMBOL *symtab;
	FAST COUNT i;
	FAST SYMBOL *ps;

	for (i = 0, ps = symtab; i < numsym; ++i, ++ps)
		if ((ps->fl & mask) == val)
			putfmt(fmt, ps->nm, actlen(ps->nm), ps->val);
	}

/*	get a short in proper byte order
 */
UCOUNT xstos(s)
	FAST UTINY *s;
	{
	FAST UCOUNT val;

	val = s[0] | s[1] << 8;
	return (val);
	}
