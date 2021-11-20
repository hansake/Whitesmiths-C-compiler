/*	UTILITIES FOR IDRIS LINKER
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "link.h"

/*	machine characteristics
 */
BOOL longint {0};
BOOL lsfmt {0};
BOOL ok {YES};
BOOL txtrev {NO};
COUNT lenname {15};
UCOUNT binhdr {0};

/*	the library seek vector
 */
COUNT nlibs {0};
LONG liboff[MAXLIBS] {0};

/*	the relocation information
 */
BHDR obhdr {0};
ULONG bsiz {0};
ULONG dsiz {0};
ULONG tsiz {0};

/*	the symbol table
 */
COUNT nsyms {0};
COUNT nund {0};
SYMBOL *stabs[NTABS] {0};

/*	flags:
	-a		make all code and symbols absolute
	-bb##	set bss bias to ## (not end of data)
	-b##	pad stack + heap area by ## bytes
	-c		don't output code sections (.text || .data) NOTE: implies -a
	-db##	set data bias to ## (not end of text)
	-dr#	round data bias up to 2**# byte bound (not end of text)
	-d		don't define BSS symbols or complain about undefined
	-eb*	define symbol as end of bss
	-ed*	define symbol as end of data
	-et*	define symbol as end of text
	-h		suppress header
	-i		get files from STDIN
	-l?^	scan /lib/lib?.a after all files
	-o*		output to file, not "xeq"
	-r		suppress relocation bits
	-sb*	define symbol as size of bss
	-sd*	define symbol as size of data
	-st*	define symbol as size of text
	-tb##	set text bias to ## (not 0)
	-tf#	fill text up to 2**# byte bound
	-t		suppress symbol table
	-u*^	enter symbol as undefined
	-x#		2=>text to text, 1=>data to text
 */
BOOL afl {NO};
BOOL cfl {NO};
BOOL dfl {NO};
BOOL hfl {NO};
BOOL ifl {NO};
BOOL rfl {NO};
BOOL tfl {NO};
BYTES dround {0};
BYTES tfill {0};
BYTES xfl {2};
LIBLIST llist {EXLIB};
LONG drmask {0};
TEXT *endbss {NULL};
TEXT *endtext {NULL};
TEXT *enddata {NULL};
TEXT *fname {""};
TEXT *ofile {"xeq"};
TEXT *sizbss {NULL};
TEXT *sizdat {NULL};
TEXT *siztxt {NULL};
ULIST ulist {NUND};
ULONG bbias {-1};
ULONG bpad {0};
ULONG tbias {0};
ULONG dbias {-1};

TEXT *_pname {"link"};

/*	input output stuff
 */
FILE ifd {-1};
FILE ofd {-1};
FILE tfd {-1};
IOBUF ibuf[2] {0};
IOBUF obuf[4] {0};
LONG iseek {0};

/*	add end symbol if not yet resolved
 */
SYMBOL *addend(name, flags, value)
	TEXT *name;
	UCOUNT flags;
	LONG value;
	{
	FAST COUNT i;
	FAST SYMBOL *p;
	FAST TEXT *n;
	TEXT buf[16];

	p = NULL;
	if (name)
		{
		for (i = 0, n = name; i < 16 && *n;)
			buf[i++] = *n++;
		while (i < 16)
			buf[i++] = '\0';
		if (!(p = lookup(buf)))
			;
		else if (p->flags & DEFD)
			xremark("redefined end symbol: ", name);
		else
			{
			p->flags = flags;
			p->value = value;
			}
		}
	return (p);
	}

/*	add offset to library vector
 */
VOID addlib(loff)
	LONG loff;
	{
	IMPORT COUNT nlibs;
	IMPORT LONG liboff[];

	if (MAXLIBS <= nlibs)
		error("library table full", NULL);
	liboff[nlibs++] = loff;
	}

/*	add symbol to tables
 */
SYMBOL *addsym(name, flags, value)
	TEXT *name;
	COUNT flags;
	LONG value;
	{
	IMPORT COUNT lenname, nsyms;
	IMPORT SYMBOL *stabs[];
	FAST COUNT i, j;
	FAST SYMBOL *p;

	if (MAXSYMS <= nsyms)
		error("symbol table full", NULL);
	i = nsyms >> SOFF;
	j = nsyms++ & SMASK;
	if (!j)
		stabs[i] = alloc((SMASK+1) * sizeof (*p), NULL);
	p = &stabs[i][j];
	cpybuf(p->name, name, lenname);
	p->flags = flags;
	p->value = value;
	return (p);
	}

/*	add -u symbol to table
 */
VOID addusym(name)
	TEXT *name;
	{
	FAST COUNT i;
	FAST TEXT *p;
	TEXT buf[16];
	
	if (name)
		{
		for (i = 0, p = name; i < 16 && *p;)
			buf[i++] = *p++;
		while (i < 16)
			buf[i++] = '\0';
		addsym(buf, 0, 0L);
		}
	}

/*	get a byte from load module
 */
UTINY getby(idx)
	COUNT idx;
	{
	IMPORT FILE ifd;
	IMPORT IOBUF ibuf[];
	IMPORT LONG iseek;
	FAST IOBUF *p = &ibuf[idx];
	LONG nseek;

	if (p->n <= p->start)
		{
		nseek = p->off & NOT_L_BUF_MASK;
		if (iseek != nseek)
			{
			lseek(ifd, nseek, 0);
			iseek = nseek;
			}
		if ((p->n = read(ifd, p->buf, L_BUFSIZE)) < 0)
			error("read error in pass2", NULL);
		iseek =+ p->n;
		p->off =+ p->n;
		p->start =- L_BUFSIZE;
		if (p->n <= p->start)
			error("early EOF in pass2", NULL);
		}
	return (p->buf[p->start++]);
	}

/*	open libs when getfiles runs out
 */
FILE gtlfiles(pac, pav)
	BYTES *pac;
	TEXT ***pav;
	{
	IMPORT LIBLIST llist;
	IMPORT TEXT *fname;
	FILE fd;
	INTERN BYTES index;
	INTERN TEXT libnam[64];
	INTERN TEXT libroot[] {"/lib/lib"};

	if (!pac)
		{
		index = EXLIB;
		return (0);
		}
	if (0 <= (fd = getbfiles(pac, pav, STDIN, STDERR, YES)))
		{
		fname = (*pav)[-1];
		if (fd != STDERR || !prefix(fname, "-l"))
			return (fd);
		else
			fname =+ 2;
		}
	else if (index <= llist.num)
		{
		fname = "";
		return (-1);
		}
	else
		fname = llist.nam[--index];
	cpystr(libnam, libroot, NULL);
	if (lenstr(fname) < 64 - (sizeof (libroot) - 1))
		cpystr(libnam + (sizeof (libroot) - 1), fname, NULL);
	fname = libnam;
	return ((0 <= (fd = open(libnam, READ, YES))) ? fd : STDERR);
	}

/*	get magic # at start of file
 */
UCOUNT gtmagic(fd)
	FILE fd;
	{
	IMPORT BOOL longint, lsfmt, txtrev;
	IMPORT COUNT lenname;
	IMPORT UCOUNT binhdr;
	UCOUNT hdr;
	UTINY header[2];

	if (fread(fd, &header, 2) != 2)
		error("can't read file header", NULL);
	hdr = lstoi(header);
	if (header[0] != 0x99)
		;
	else if (header[1] & 0200)
		error("no relocation bits", NULL);
	else if (!binhdr)
		{
		lenname = ((header[1] & 07) << 1) + 1;
		longint = header[1] & 010;
		lsfmt = header[1] & 020;
		txtrev = header[1] & 0100;
		if (txtrev)
			putstr(STDOUT, "text reversal specified\n", 0);
		binhdr = hdr;
		}
	return (hdr);
	}

/*	get symbol table from binary file
 */
SYMBOL *gtsyms(fd, pbhdr)
	FILE fd;
	FAST BHDR *pbhdr;
	{
	IMPORT BOOL longint;
	IMPORT BYTES xfl;
	IMPORT LONG xstol();
	IMPORT UCOUNT xstos();
	IMPORT ULONG tsiz, dsiz;
	FAST SYMBOL *ps;
	COUNT hsize = longint ? 26 : 14;
	LONG off;
	TEXT bhdr[26];

	if (fread(fd, bhdr, hsize) != hsize)
		error("can't read binary header", NULL);
	pbhdr->sbytes = xstos(&bhdr[0]);
	if (longint)
		{
		pbhdr->tsiz = xstol(&bhdr[2]);
		pbhdr->dsiz = xstol(&bhdr[6]);
		pbhdr->bsiz = xstol(&bhdr[10]);
		pbhdr->tbias = xstol(&bhdr[18]);
		pbhdr->dbias = xstol(&bhdr[22]);
		}
	else
		{
		pbhdr->tsiz = xstos(&bhdr[2]);
		pbhdr->dsiz = xstos(&bhdr[4]);
		pbhdr->bsiz = xstos(&bhdr[6]);
		pbhdr->tbias = xstos(&bhdr[10]);
		pbhdr->dbias = xstos(&bhdr[12]);
		}
	switch (xfl)
		{
	case 0:
		pbhdr->atbias = dsiz;
		pbhdr->adbias = dsiz + pbhdr->tsiz;
		pbhdr->atsiz = 0;
		pbhdr->adsiz = pbhdr->tsiz + pbhdr->dsiz;
		break;
	case 1:
		pbhdr->atbias = dsiz;
		pbhdr->adbias = tsiz;
		pbhdr->atsiz = pbhdr->dsiz;
		pbhdr->adsiz = pbhdr->tsiz;
		break;
	case 2:
		pbhdr->atbias = tsiz;
		pbhdr->adbias = dsiz;
		pbhdr->atsiz = pbhdr->tsiz;
		pbhdr->adsiz = pbhdr->dsiz;
		break;
	case 3:
		pbhdr->atbias = tsiz;
		pbhdr->adbias = dsiz + pbhdr->tsiz;
		pbhdr->atsiz = pbhdr->tsiz + pbhdr->dsiz;
		pbhdr->adsiz = 0;
		break;
		}
	pbhdr->atbias =- pbhdr->tbias;
	pbhdr->adbias =- pbhdr->dbias;
	off = pbhdr->tsiz + pbhdr->dsiz;
	lseek(fd, off, 1);
	ps = alloc(pbhdr->sbytes, NULL);
	if (fread(fd, ps, pbhdr->sbytes) != pbhdr->sbytes)
		error("can't read symbol table", NULL);
	return (ps);
	}

/*	lookup name in symbol table
 */
SYMBOL *lookup(name)
	TEXT *name;
	{
	IMPORT COUNT lenname, nsyms;
	IMPORT SYMBOL *stabs[];
	FAST COUNT i;
	FAST SYMBOL *p;

	for (i = 0; i < nsyms; ++i, ++p)
		{
		if (!(i & SMASK))
			p = stabs[i >> SOFF];
		if (cmpbuf(p->name, name, lenname))
			return (p);
		}
	return (NULL);
	}

/*	relocate using flags and sizes
 */
LONG rebias(flags, tb, db, bb)
	COUNT flags;
	LONG tb, db, bb;
	{
	IMPORT ULONG bsiz, dsiz, tsiz;
	FAST LONG val;

	switch (flags & 03)
		{
	case 1:
		val = tsiz - tb;
		break;
	case 2:
		val = dsiz - db;
		break;
	case 3:
		val = bsiz - bb;
		break;
	default:
		val = 0;
		}
	return (val);
	}

/*	put relocatable byte
 */
VOID relby(idx, by)
	FAST COUNT idx;
	TEXT by;
	{
	IMPORT BOOL afl, rfl;
	IMPORT FILE ofd, tfd;
	IMPORT IOBUF obuf[];
	FAST COUNT n;
	FAST IOBUF *p = &obuf[idx & 3];
	BOOL drain = idx & DRAIN;
	INTERN LONG off[2];

	if ((afl || rfl) && (idx & (WRMI|DREL&TREL)) == (DREL&TREL))
		return;
	idx =& 03;
	if (p->n < L_BUFSIZE && !drain)
		;
	else if (p->off != off[idx == DREL] &&
		lseek((idx == DREL) ? tfd : ofd, p->off, 0) < 0 ||
		(n = p->n - p->start) &&
		write((idx == DREL) ? tfd : ofd, p->buf + p->start, n) != n)
		error("can't write ", (idx == DREL) ? "temp file" : "output file");
	else
		{
		p->off =+ n;
		p->start = 0;
		p->n = 0;
		off[idx == DREL] = p->off;
		}
	if (!drain)
		p->buf[p->n++] = by;
	}

/*	put relocatable short or long
 */
VOID relint(idx, val)
	COUNT idx;
	LONG val;
	{
	IMPORT BOOL longint, lsfmt;
	FAST COUNT i, wo;
	TEXT buf[4];

	if (!longint)
		relwd(idx, wo = val);
	else if (lsfmt)
		for (i = 4; 0 <= --i; val =>> 8)
			relby(idx, wo = val);
	else
		{
		for (i = 4; 0 <= --i; val =>> 8)
			buf[i] = val;
		for (i = 0; i < 4; ++i)
			relby(idx, buf[i]);
		}
	}

/*	put a symbol
 */
VOID relsym(p)
	FAST SYMBOL *p;
	{
	IMPORT BOOL afl, cfl;
	FAST COUNT k;

	relint(WRMI|TREL, p->value);
	relby(WRMI|TREL, (afl || cfl) ? EXTERN|p->flags & DEFD : p->flags);
	for (k = 0; k < lenname; ++k)
		relby(WRMI|TREL, p->name[k]);
	}

/*	put relocatable word
 */
VOID relwd(idx, wo)
	COUNT idx, wo;
	{
	IMPORT BOOL lsfmt;

	if (lsfmt)
		{
		relby(idx, wo);
		relby(idx, wo >> 8);
		}
	else
		{
		relby(idx, wo >> 8);
		relby(idx, wo);
		}
	}

/*	complain and remember failure
 */
VOID xremark(s1, s2)
	TEXT *s1, *s2;
	{
	IMPORT BOOL ok;
	IMPORT COUNT lenname;
	IMPORT TEXT *fname;
	INTERN TEXT buf[16];

	buf[cpybuf(buf, s2, lenname)] = '\0';
	putstr(STDOUT, fname, (*fname != '\0' ? ": " : ""), s1, buf, "\n", NULL);
	ok = NO;
	}

/*	get long in proper byte order
 */
LONG xstol(s)
	FAST UTINY *s;
	{
	IMPORT BOOL lsfmt;
	FAST COUNT i;
	LONG val;

	val = 0;
	if (lsfmt)
		for (i = 4; 0 <= --i; )
			val = val << 8 | s[i];
	else
		for (i = 0; i < 4; ++i)
			val = val << 8 | s[i];
	return (val);
	}

/*	get short in proper byte order
 */
UCOUNT xstos(s)
	FAST UTINY *s;
	{
	IMPORT BOOL lsfmt;
	FAST UCOUNT val;

	if (lsfmt)
		val = s[0] | s[1] << 8;
	else
		val = s[0] << 8 | s[1];
	return (val);
	}
