/*	INSPECT IDRIS OBJECT FILES
 * 	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

/*	symbol types
 */
#define UDEFSYMB	0
#define ABSSYMB		4
#define TXTSYMB		5
#define DATASYMB	6
#define BSSSYMB		7
#define	DEFD		004
#define EXTERN		010

/*	header sizes
 */
#define MAGSIZ		2
#define LNGLIB		26
#define SRTLIB		16
#define LNGHDR		26
#define SRTHDR		14

/*	stuff for new header
 */
#define	PSEGSZ	40
#define	LSEGNM	14

#define	DIRHDR	0177630
#define MODCODE	0200
#define LENCODE	0177

/*	flags:
	-a		sort alphabetically, by name
	-d		list defined symbols
	+dir	print details of library directories
	-g		list global symbols
	-h		output in hex (default is decimal)
	-i		print intervals in value field
	-in		input list of files from STDIN
	-o		output in octal (default is decimal)
	+seg	print segment header info for multi-segment files
	+size	print size in bytes of each module
	-s		tell sizes of text, data, bss and total
	-t		tell machine and library type data
	-u		list undefined symbols
	-v		sort by value
 */
BOOL aflag = {NO};
BOOL dflag = {NO};
BOOL dirflag = {NO};
BOOL gflag = {NO};
BOOL hflag = {NO};
BOOL iflag = {NO};
BOOL inflag = {NO};
BOOL manyfls = {NO};
BOOL newhdr = {NO};
BOOL oflag = {NO};
BOOL sflag = {NO};
BOOL segflag = {NO};
BOOL sizflag = {NO};
BOOL tflag = {NO};
BOOL uflag = {NO};
BOOL vflag = {NO};

TEXT *_pname = {"rel"};

/*	decoded header values
 */
BOOL evenbnd = {NO};
BOOL longint = {NO};
BOOL lsfmt  = {NO};
BOOL txtrev = {NO};
COUNT lenname = {0};
UCOUNT binhdr = {0};
UCOUNT sbytes = {0};
ULONG bsize = {0};
ULONG dbias = {0};
ULONG dsize = {0};
ULONG ssize = {0};
ULONG tbias = {0};
ULONG tsize = {0};

/*	decoded values from multi-segment header
 */
COUNT nsegs = {0};
ULONG seeksym = {0};
ULONG sizesym = {0};
ULONG *segend = {NULL};
ULONG totalsize = {0};

/*	running totals
 */
ULONG glbsize = {0};
ULONG gldsize = {0};
ULONG glssize = {0};
ULONG gltsize = {0};

/*	symbol table stats
 */
TEXT *norder= {NULL};		/* pointer to unordered symbol table */
TEXT **sorted = {NULL};	/* pointer to array of pointers to sorted entries */
COUNT numsym = {0};
COUNT entsize = {0};
COUNT presize = {0};
COUNT valsize = {0};
UTINY osnum = {0};

/*	function declarations for rel.c
 */
GLOBAL BOOL dofile(/* fd, fname, lsize, loff */);
GLOBAL BOOL main(/* ac, av */);
GLOBAL COUNT entorn(/* i, j, plist */);
GLOBAL COUNT entorv(/* i, j, plist */);
GLOBAL COUNT getext(/* list, start, total */);
GLOBAL COUNT nmbytes(/* i */);
GLOBAL TEXT *getsym(/* fd */);
GLOBAL UCOUNT getmagic(/* fd */);
GLOBAL UCOUNT xstos(/* s */);
GLOBAL ULONG getlsize(/* fd, loff */);
GLOBAL ULONG reldata(/* fd */);
GLOBAL ULONG xstol(/* s */);
GLOBAL VOID entswap(/* i, j, plist */);
GLOBAL VOID gethdr(/* fd */);
GLOBAL VOID getnhdr(/* fd */);
GLOBAL VOID intrpfil(/* fd, lsize, loff */);
GLOBAL VOID libdir(/* fd */);
GLOBAL VOID lstsmbls(/* fd */);
GLOBAL VOID ltoxs(/* b, val */);
GLOBAL VOID putsymb(/* pent */);
GLOBAL VOID stoxs(/* b, val */);
GLOBAL VOID warplst(/* list, numsyms, totwords */);


/*	do a file, object or library
 */
BOOL dofile(fd, fname, lsize, loff)
	FAST FILE fd;
	TEXT *fname;
	ULONG lsize, loff;
	{
	IMPORT BOOL manyfls, sflag, sizflag;
	IMPORT UCOUNT binhdr, getmagic();
	IMPORT UCOUNT lstou();
	FAST COUNT hsize;
	TEXT lhdr[LNGLIB];
	UCOUNT header;
	ULONG lngsize, maxloff;

	maxloff = loff + lsize;
	header = getmagic(fd);
	if (header == binhdr)
		{
		if (manyfls || loff)
			{
			putstr(STDOUT, fname, NULL);
			if (sflag || sizflag)
				putstr(STDOUT, ": ", NULL);
			else
				putstr(STDOUT, ":\n", NULL);
			}
		intrpfil(fd, lsize, loff);
		}
	else if (header == DIRHDR)
		libdir(fd);
	else if (header != 0177565 && header != 0177555 && header != 0177545)
		return (remark("bad file format: ", fname));
	else
		{
		putstr(STDOUT, "Library - ", fname, ":", NULL);
		if (tflag)
			switch (header)
				{
			case 0177565:
				putstr(STDOUT, "standard format.", NULL);
				break;
			case 0177555:
				putstr(STDOUT, "Unix V6 format.", NULL);
				break;
			case 0177545:
				putstr(STDOUT, "Unix System III format.", NULL);
				}
		putstr(STDOUT, "\n", NULL);
		hsize = (header == 0177545) ? LNGLIB : SRTLIB;
		for (loff += 2; (fread(fd, &lhdr, hsize) == hsize) && lhdr[0];
			lseek(fd, loff, 0))
			{
			lngsize = (header != 0177545) ? (ULONG)lstou(&lhdr[14])
				: ((ULONG)lstou(&lhdr[24]) << 16) + lstou(&lhdr[22]);
			lhdr[(header == 0177555) ? 8 : 14] = '\0';
			dofile(fd, lhdr, lngsize, loff + hsize);
			loff += hsize + lngsize;
			if (header != 0177565)
				loff += lngsize & 1;
			if (maxloff && maxloff <= loff)
				break;
			}
		}
	return (YES);
	}

/*	give the ordering of two pointer list entries by name
 */
COUNT entorn(i, j, dead_plist)
	COUNT i, j;
	TEXT **dead_plist;
	{
	IMPORT BOOL longint;
	IMPORT COUNT lenname, presize;
	IMPORT TEXT **sorted;
	FAST COUNT u;
	FAST UTINY *s;
	FAST UTINY *t;
	COUNT diff;

	s = (UTINY *)&sorted[i][presize];
	t = (UTINY *)&sorted[j][presize];
	u = lenname;
	if (lenname == 0)
		{
		u = min(*s, *t);
		diff = *s - *t;
		++s;
		++t;
		}
	for ( ; 0 < u; --u, ++s, ++t)
		if (*s != *t)
			return (*s - *t);
	return (lenname ? 0 : diff);
	}

/*	give the ordering of two pointer list entries
 */
COUNT entorv(i, j, plist)
	COUNT i, j;
	TEXT **plist;
	{
	IMPORT BOOL longint, newhdr;
	IMPORT TEXT **sorted;
	IMPORT UCOUNT xstos();
	IMPORT ULONG xstol();
	FAST ULONG s;
	FAST ULONG t;

	s = (sorted[i][(longint) ? 4 : 2]) & 03;
	t = (sorted[j][(longint) ? 4 : 2]) & 03;
	if (s != t)
		return ((COUNT)(s-t));
	if (newhdr)
		{
		s = sorted[i][(longint) ? 5 : 3];
		t = sorted[j][(longint) ? 5 : 3];
		if (s != t)
			return ((COUNT)(s - t));
		}
	s = (longint) ? xstol(sorted[i]) : xstos(sorted[i]);
	t = (longint) ? xstol(sorted[j]) : xstos(sorted[j]);
	if (s == t)
		return (entorn(i, j, plist));
	else if (s < t)
		return (-1);
	else 
		return (1);
	}

/*	exchange two entries
 */
VOID entswap(i, j, dead_plist)
	COUNT i, j;
	TEXT **dead_plist;
	{
	IMPORT TEXT **sorted;
	FAST TEXT *tmp;

	tmp = sorted[i];
	sorted[i] = sorted[j];
	sorted[j] = tmp;
	}

/*	get next extern from entry table
 */
COUNT getext(list, start, total)
	COUNT start, total;
	TEXT **list;
	{
	IMPORT BOOL longint;
	FAST COUNT i;
	FAST COUNT findx = (longint) ? 4 : 2;
	FAST TEXT *p;

	for (i = start; i < total; ++i)
		{
		p = list[i];
		if (p[findx] & EXTERN)
			return (i);
		}
	return (-1);
	}

/*	get header information from file
 */
VOID gethdr(fd)
	FILE fd;
	{
	IMPORT BOOL longint;
	IMPORT UCOUNT sbytes, xstos();
	IMPORT ULONG xstol();
	IMPORT ULONG bsize, dbias, dsize, ssize, tbias, tsize;
	FAST COUNT hsize = (longint) ? LNGHDR : SRTHDR;
	TEXT bhdr[LNGHDR];

	if (fread(fd, bhdr, hsize) != hsize)
		error("can't read binary header", NULL);
	sbytes = xstos(&bhdr[0]);
	if (longint)
		{
		tsize = xstol(&bhdr[2]);
		dsize = xstol(&bhdr[6]);
		bsize = xstol(&bhdr[10]);
		ssize = xstol(&bhdr[14]);
		tbias = xstol(&bhdr[18]);
		dbias = xstol(&bhdr[22]);
		}
	else
		{
		tsize = xstos(&bhdr[2]);
		dsize = xstos(&bhdr[4]);
		bsize = xstos(&bhdr[6]);
		ssize = xstos(&bhdr[8]);
		tbias = xstos(&bhdr[10]);
		dbias = xstos(&bhdr[12]);
		}
	}

/*	get size of module
 */
ULONG getlsize(fd, loff)
	FILE fd;
	ULONG loff;
	{
	IMPORT BOOL longint;
	IMPORT UCOUNT sbytes;
	IMPORT ULONG dsize, tsize;
	IMPORT ULONG reldata();
	ULONG hsize, size;

	hsize = (longint) ? LNGHDR + MAGSIZ : SRTHDR + MAGSIZ;
	size = hsize + tsize + dsize + sbytes;
	lseek(fd, loff + size, 0);
	size += reldata(fd) + reldata(fd);
	lseek(fd, loff + hsize, 0);
	return (size);
	}

/*	get magic number from header
 */
UCOUNT getmagic(fd)
	FILE fd;
	{
	IMPORT BOOL longint, lsfmt, evenbnd, newhdr, txtrev;
	IMPORT COUNT lenname;
	IMPORT UCOUNT binhdr;
	FAST UCOUNT hdr;
	UTINY header[MAGSIZ];

	if (fread(fd, &header, MAGSIZ) != MAGSIZ)
		error("can't read header", NULL);
	hdr = lstou(header);
	if (header[0] == 0x99 || header[0] == 0x9d)
		{
		newhdr = (header[0] == 0x9d) ? YES : NO;
		lenname = (header[1] & 07) ? ((header[1] & 07) << 1) + 1 : 0;
		longint = header[1] & 010;
		lsfmt = header[1] & 020;
		evenbnd = header[1] & 040;
		txtrev = header[1] & 0100;
		binhdr = hdr;
		}
	return (hdr);
	}

/*	get header information from multi-segment file
 */
VOID getnhdr(fd)
	FILE fd;
	{
	IMPORT BOOL segflag;
	IMPORT COUNT nsegs;
	IMPORT ULONG seeksym, sizesym, *segend, totalsize;
	COUNT atf, hsize, i;
	ULONG off, sdat, srel, virt;
	TEXT nhdr[PSEGSZ];
	TEXT *msg = "can't read new binary header";

	if (fread(fd, nhdr, 14) != 14)
		error(msg, NULL);
	hsize = xstos(&nhdr[0]);
	nsegs = xstos(&nhdr[2]);
	seeksym = xstol(&nhdr[6]);
	sizesym = xstol(&nhdr[10]);
	totalsize = hsize + sizesym;
	segend = (ULONG *)alloc(sizeof (ULONG) * (nsegs + 1), NULL);
	for (i = 0; i < nsegs; ++i)
		{
		if (fread(fd, nhdr, PSEGSZ) != PSEGSZ)
			error(msg, NULL);
		atf = nhdr[1] + (nhdr[0] << 8);
		virt = xstol(&nhdr[16]);
		off = xstol(&nhdr[20]);
		sdat = xstol(&nhdr[28]);
		srel = xstol(&nhdr[36]);
		segend[i + 1] = off + sdat;
		if (segflag)
			{
			putfmt("Segment %i:  flags = 0x%+02hs   %.14p\n",
				i + 1, (atf & 0xff), &nhdr[2]);
			putfmt("\tbias = 0x%+08hl  offset = 0x%+08hl  size = 0x%+04hs\n\n",
				virt, off, (UCOUNT)sdat);
			}
		totalsize += sdat + srel;
		}
	}

/*	get symbol table and set table stats
*/
TEXT *getsym(fd)
	FILE fd;
	{
	IMPORT BOOL longint, newhdr;
	IMPORT COUNT entsize, lenname, numsym, presize, valsize;
	IMPORT TEXT **sorted;
	IMPORT UCOUNT sbytes;
	IMPORT ULONG dsize, seeksym, sizesym, tsize;
	FAST COUNT i;
	FAST TEXT *ps, *tps;
	TEXT *pe;

	valsize = (longint) ? 4 : 2;
	presize = valsize + 1 + (newhdr ? 1 : 0);
	entsize = presize + lenname;
	if (newhdr)
		{
		lseek(fd, seeksym, 0);
		sbytes = sizesym;
		}
	else
		lseek(fd, tsize + dsize, 1);
	ps = alloc(sbytes, NULL);
	pe = ps + sbytes;
	if (fread(fd, ps, sbytes) != sbytes)
		error("can't read symbol table", NULL);
	if (lenname)
		numsym = sbytes / entsize;
	else
		{
		numsym = 0;
		for (tps = ps + entsize; tps < pe;
			tps += entsize + 1 + (tps[0] & BYTMASK))
			++numsym;
		}
	sorted = (TEXT **)alloc(sizeof (TEXT *) * numsym, NULL);
	for (tps = ps, i = 0; i < numsym ; ++i)
		{
		sorted[i] = tps;
		tps += lenname ? entsize : entsize + 1 + (tps[presize] & BYTMASK);
		}
	return (ps);
	}

/*	play with header while reacting to flags
 */
VOID intrpfil(fd, lsize, loff)
	FILE fd;
	ULONG lsize, loff;
	{
	IMPORT BOOL dflag, hflag, evenbnd, gflag, longint;
	IMPORT BOOL lsfmt, newhdr, oflag;
	IMPORT BOOL sflag, sizflag, tflag, txtrev, uflag;
	IMPORT COUNT lenname;
	IMPORT ULONG bsize, dsize, ssize, tsize, totalsize;
	IMPORT ULONG glbsize, gldsize, glssize, gltsize;
	IMPORT ULONG getlsize();
	TEXT *szfmt = (oflag) ?
		"0%olT + 0%olD + 0%olB + 0%olS = 0%ol\n" : (hflag) ?
		"0x%hlT + 0x%hlD + 0x%hlB + 0x%hlS = 0x%hl\n" :
		"%ulT + %ulD + %ulB + %ulS = %ul\n";

	if (newhdr)
		{
		getnhdr(fd);
		sflag = NO;
		}
	else
		gethdr(fd);
	if (tflag)
		{
		if (newhdr)
			putfmt("multi-segment header.\n");
		putfmt("%s byte integers, data %p significant byte first.\n",
			(longint) ? 4 : 2, (lsfmt) ? "least" : "most");
		if (evenbnd)
			putfmt("even byte boundaries enforced by hardware.\n");
		if (txtrev)
			putfmt("text segment int byte order is reversed from data segment.\n");
		if (lenname)
			putfmt("%s character names max.\n", lenname);
		else
			putfmt("variable length names.\n");
		}
	if (sizflag)
		{
		if (lsize == 0)
			lsize = (newhdr) ? totalsize : getlsize(fd, loff);
		putfmt("%ul bytes %op", lsize, sflag ? ": " : "\n");
		}
	if (sflag)
		{
		putfmt(szfmt, tsize, dsize, bsize, ssize,
			tsize + dsize + bsize + ssize);
		gltsize += tsize;
		gldsize += dsize;
		glbsize += bsize;
		glssize += ssize;
		}
	if (dflag || uflag || gflag)
		lstsmbls(fd);
	}

/*	skip over or walk thru a library directory
 */
VOID libdir(fd)
	FILE fd;
	{
	IMPORT BOOL dflag, dirflag;
	IMPORT LONG lstol();
	FAST COUNT i, j, jj;
	COUNT ln, nmods, r;
	TEXT c, *bbuf, dbuf[6], *sep;
	ULONG dbytes, dsize;

	if (dflag)
		putfmt("Library Directory encountered.\n");
	if (!dflag || !dirflag)
		return;
	fread(fd, dbuf, 6);
	nmods = lstou(dbuf);
	dbytes = lstol(&dbuf[2]);
	r = 1 + nmods / 8;
	bbuf = alloc(max(LENCODE+3, r), NULL);

	for (i = 0; fread(fd, &c, 1) && c; )
		{
		if (c & MODCODE)
			{
			++i;
			putfmt("Module No.%+ 4ui:\n", i);
			}
		if (c & LENCODE)
			{
			ln = fread(fd, bbuf, (c & LENCODE));
			putfmt("	%b\n", bbuf, ln);
			}
		}
	putfmt("\nModule Dependencies\n\n");
	for (i = 1; i <= nmods; ++i)
		{
		putfmt("Module No.%+ 4ui: ", i);
		fread(fd, bbuf, r);
		sep = "";
		for (j = 0; j < r; ++j)
			if (bbuf[j])
				for (jj = 0; jj < 8; ++jj)
					if (bbuf[j] & "\200\100\40\20\10\4\2\1"[jj])
						putfmt("%p%ui", sep, 8*j + jj), sep = ", ";
		putfmt("\n");
		}
	putfmt("\n Module Sizes\n\n");
	for (i = 1; i <= nmods; ++i)
		{
		fread(fd, dbuf, 4);
		dsize = lstol(dbuf);
		putfmt("Module No.%+ 4ui: %ul\n", i, dsize);
		}
	}

/*	list symbols from the symbol table
 */
VOID lstsmbls(fd)
	FILE fd;
	{
	IMPORT BOOL aflag, iflag, vflag;
	IMPORT COUNT entorn();
	IMPORT COUNT entorv();
	IMPORT COUNT numsym;
	IMPORT TEXT *norder, **sorted;
	IMPORT VOID entswap();
	FAST UCOUNT i;

	if ((norder = getsym(fd)) == NULL)
		return;
	if (aflag || vflag)
		sort(numsym, (vflag ? &entorv : &entorn), &entswap, sorted);
	if (iflag)
		warplst(sorted, numsym, tsize + dsize + bsize);
	for (i = 0; i < numsym; ++i)
		putsymb(sorted[i]);
	free(norder, NULL);
	free(sorted, NULL);
	}

/*	pack a long into a buffer in correct byte order
 */
VOID ltoxs(b, val)
	TEXT *b;
	ULONG val;
	{
	IMPORT BOOL lsfmt;
	FAST COUNT i;

	if (lsfmt)
		for (i = 0; i < 4; ++i)
			{
			b[i] = val;
			val >>= 8;
			}
	else
		for (i = 4; 0 <= --i; )
			{
			b[i] = val;
			val >>= 8;
			}
	}

/*	disassemble and tell unresolved references
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL manyfls;
	IMPORT ULONG gltsize, gldsize, glbsize, glssize;
	FAST BOOL success;
	FAST FILE fd;
	TEXT *tsfmt;
	INTERN TEXT *dfile = {"xeq"};

	getflags(&ac, &av, "a,+dir,d,g,h,+in,i,o,+seg,+size,s,t,u,v:F <files>",
		&aflag, &dirflag, &dflag, &gflag, &hflag, &inflag, &iflag,
		&oflag, &segflag, &sizflag, &sflag, &tflag, &uflag, &vflag);
	tsfmt = (oflag) ?
		"TOTAL: 0%olT + 0%olD + 0%olB + 0%olS + 0%ol\n" : (hflag) ?
		"TOTAL: 0x%hlT + 0x%hlD + 0x%hlB + 0x%hlS = 0x%hl\n" :
		"TOTAL: %ulT + %ulD + %ulB + %ulS = %ul\n";
	if ((aflag || vflag) && !gflag)
		dflag = YES;
	if (iflag)
		dflag = uflag = vflag = YES;
	if (!(dflag || gflag || uflag ||
		dirflag || segflag || sizflag || sflag || tflag))
		dflag = uflag = YES;
	if (inflag)
		getin(&ac, &av);
	manyfls = (1 < ac) ? YES : NO;
	success = YES;
	if (!ac)
		{
		ac = 1;
		av = &dfile;
		}
	while (0 <= (fd = getbfiles(&ac, &av, STDIN, STDERR, YES)))
		if (fd == STDERR) 
			success = remark("can't read ", av[-1]);
		else
			{
			success &= dofile(fd, av[-1], 0L, 0L);
			close(fd);
			}
	if (sflag && manyfls && !newhdr)
		putfmt(tsfmt, gltsize, gldsize, glbsize, glssize,
			gltsize + gldsize + glbsize + glssize);
	return (success);
	}

/*	number of bytes in a segment name
 */
COUNT nmbytes(i)
	COUNT i;
	{
	i = 2 + 4 * ((i + 2) / 4);
	i = max(6, i);
	return (i);
	}

/*	output a symbol entry
 */
VOID putsymb(pent)
	FAST UTINY *pent;
	{
	IMPORT BOOL aflag, dflag, gflag, hflag;
	IMPORT BOOL longint, newhdr, oflag, uflag, vflag;
	IMPORT UCOUNT xstos();
	IMPORT ULONG xstol();
	IMPORT UTINY osnum;
	FAST ULONG value;
	FAST UTINY flags = pent[(longint) ? 4 : 2];
	BOOL isdef, isext;
	BYTES len;
	TEXT *lfmt = (oflag) ? "0%+011ol%ac " : (hflag) ? "0x%+08hl%ac " :
		"%+ 10ul%ac ";
	TEXT outtype;
	TEXT *sfmt = (oflag) ? "0%+06os%ac " : (hflag) ? "0x%+04hs%ac " :
		"%+ 5us%ac ";
	UCOUNT wo;
	UTINY snum;

	if (longint)
		{
		value = xstol(pent);
		pent += 5;
		}
	else
		{
		value = xstos(pent);
		pent += 3;
		}
	isdef = flags & DEFD;
	isext = flags & EXTERN;
	if (newhdr)
		{
		snum = *pent++;
		if (vflag && snum != osnum)
			{
			putfmt("\nSEGMENT NUMBER %uc\n", snum);
			osnum = snum;
			}
		}
	if (lenname)
		len = scnbuf(pent, lenname, '\0');
	else
		len = *pent++;
	if (iflag && !isext)
		return;
	if (uflag && !dflag && !isdef && !gflag)
		putfmt("%b\n", pent, len);
	else if (dflag || gflag)
		{
		switch (flags & 07)
			{
		case ABSSYMB:
			outtype = 'a';
			break;
		case TXTSYMB:
			outtype = 't';
			break;
		case DATASYMB:
			outtype = 'd';
			break;
		case BSSSYMB:
			outtype = 'b';
			break;
		case UDEFSYMB:
			outtype = 'u';
			break;
		default:
			outtype = '?';
			break;
			}
		outtype = isext ? toupper(outtype) : outtype;
		if ((gflag && isext) || (dflag && isdef) || (uflag && !isdef))
			{
			if (longint)
				putfmt(lfmt, value, outtype);
			else
				putfmt(sfmt, wo = value, outtype);
			if (aflag && newhdr)
				putfmt("seg: %uc %b\n", snum, pent, len);
			else
				putfmt("%b\n", pent, len);
			}
		}
	}

/*	walk through a stream of relocation data and count bytes
 */
ULONG reldata(fd)
	FILE fd;
	{
	ULONG lsize;
	UTINY c;

	lsize = 0;
	FOREVER
		{
		fread(fd, &c, 1);
		++lsize;
		if (c == 0)
			break;
		else if (c < 32)
			;
		else if (c < 64)
			{
			fread(fd, &c, 1);
			++lsize;
			}
		else if (c < 252)
			;
		else if (fread(fd, &c, 1) && (c & 0200))
			{
			fread(fd, &c, 1);
			lsize += 2;
			}
		else
			++lsize;
		}
	return (lsize);
	}

/*	pack a short into a buffer in correct byte order
 */
VOID stoxs(b, val)
	TEXT *b;
	UCOUNT val;
	{
	IMPORT BOOL lsfmt;

	if (lsfmt)
		{
		b[0] = val;
		b[1] = val >> 8;
		}
	else
		{
		b[1] = val;
		b[0] = val >> 8;
		}
	}

/*	force symbols to contain intervals in value field
 */
VOID warplst(list, nsym, totwords)
	TEXT **list;
	UCOUNT nsym;
	ULONG totwords;
	{
	IMPORT BOOL longint;
	IMPORT UCOUNT xstos();
	IMPORT ULONG xstol();
	IMPORT ULONG *segend;
	FAST TEXT *p, *q;
	COUNT next, psnum, qsnum, top, sho;
	ULONG pvalue, qvalue;

	sho = (longint) ? 5 : 3;
	for (top = getext(list, 0, nsym); 0 <= top; top = next)
		{
		p = list[top];
		psnum = (newhdr) ? *(p + sho) : 0;
		pvalue = (longint) ? xstol(p) : xstos(p);
		next = getext(list, top + 1, nsym);
		q = (0 <= next) ? list[next] : NULL;
		if (q)
			qsnum = (newhdr) ? *(q + sho) : 0;
		if (!q || (qsnum != psnum))
			qvalue = (newhdr) ? segend[psnum] : totwords;
		else
			qvalue = (longint) ? xstol(q) : xstos(q);
		if (longint)
			ltoxs(p, qvalue - pvalue);
		else
			stoxs(p, qvalue - pvalue);
		}
	}

/*	get a long into proper byte order
 */
ULONG xstol(s)
	FAST UTINY *s;
	{
	IMPORT BOOL lsfmt;
	FAST COUNT i;
	FAST ULONG val;
	
	val = 0;
	if (lsfmt)
		for (i = 4; 0 <= --i; )
			val = val << 8 | s[i];
	else
		for (i = 0; i < 4; ++i)
			val = val << 8 | s[i];
	return (val);
	}


/*	get a short in proper byte order
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
