/*	LIBRARY MAINTENANCE
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

#define MAGICW	0177565
#define MAGIC6	0177555
#define MAGIC7	0177545
#define MAGSIZE	2
#define MZERO -1L
#define MEMPTY 65535
#define NSIZE	14
#define N6SIZE	8
#define NOSIZE	0
#define FOUND 	1 /* module found in library */
#define STRPNM	2 /* strip path from module */

#define HDR union hdr
union hdr {
	struct {
		TEXT name[NSIZE];
		UTINY wsize[2];
		} w;
	struct {
		TEXT name6[N6SIZE];
		TINY pad6[6];
		UTINY v6size[2];
		} v6;
	struct {
		TEXT name[NSIZE];
		TINY pad8[8];
		UTINY v7size[4];
		} v7;
	};

/*	flags:
	-c		create (force virgin file)
	-d		delete named files
	-i		get files from STDIN
	-p		print (extract to STDOUT)
	-r		replace named files
	-t		list files in library
	-v3		use UNIX System III format with -cr
	-v6		use UNIX v6 format with -cr
	-v7		use UNIX v7 format with -cr
	-v		be verbose about it
	-x		extract named (or all) files
 */
BOOL cfl {NO};
BOOL dfl {NO};
BOOL ifl {NO};
BOOL pfl {NO};
BOOL rfl {NO};
BOOL tfl {NO};
BOOL vfl {NO};
BOOL v3fl {NO};
BOOL v6fl {NO};
BOOL v7fl {NO};
BOOL xfl {NO};

/*	configuration info
 */
BYTES vers {0};
COUNT hsize {16};	/* size of library header */
COUNT nsize {NSIZE};
COUNT round {0};	/* 1 if file sizes are rounded up */
UCOUNT magic {0};	/* magic header number */

/*	misc
 */
LONG msize {0};		/* size of module now under consideration */
TBOOL *pfound {NULL};
TEXT *_pname {"lib"};
VOID *(*nxtexit)() {NULL};

/*	copy [part of] a file
 */
LONG copy(ffd, tfd, size)
	FILE ffd, tfd;
	LONG size;
	{
	IMPORT COUNT round;
	FAST COUNT i, j;
	LONG n;
	TEXT buf[BUFSIZE];

	i = BUFSIZE;
	for (n = 0; !size || n < size; n =+ j)
		{
		if (size)
			i = min(size - n, BUFSIZE);
		if (!(j = fread(ffd, buf, i)) && size)
			error("unexpected EOF", NULL);
		else if (!j)
			break;
		else
			fwrite(tfd, buf, j);
		}
	if (!size && n & round)
		fwrite(tfd, "", 1);
	return (n);
	}

/*	delete files from library
 */
BOOL del(ac, av, file)
	BYTES ac;
	TEXT **av, *file;
	{
	IMPORT BOOL vfl;
	IMPORT COUNT round;
	IMPORT LONG msize;
	FAST COUNT j;
	FAST FILE lfd, tfd;
	HDR hdr;

	if (!ac)
		error("no delete files", NULL);
	lfd = lopen(file, YES);
	tfd = lcreate(NULL);
	while (gthdr(lfd, &hdr))
		if ((j = inpos(ac, av, hdr.w.name)) < 0)
			{
			if (vfl)
				report("c ", hdr.w.name);
			if (msize == 0)
				msize = MZERO;
			pthdr(tfd, &hdr);
			if (msize != MZERO)
				copy(lfd, tfd, msize + (msize & round));
			}
		else
			{
			if (vfl)
				report("d ", strpath(hdr.w.name, av[j]));
			found(j);
			if (msize != MZERO)
				skip(lfd, msize, round);
			}
	close(lfd);
	close(tfd);
	if (erprint(ac, av, "lib: can't delete "))
		{
		putback(file, NULL, 0);
		return (YES);
		}
	else
		return (NO);
	}

/*	remove temp on exit
 */
VOID *endup()
	{
	IMPORT VOID *(*nxtexit)();

	remove(uname());
	return (nxtexit);
	}

/*	print error msg for files not found in the av list,
 *	and free error map
 */
BOOL erprint(ac, av, s)
	BYTES ac;
	TEXT **av, *s;
	{
	IMPORT TBOOL *pfound;
	BOOL ok;
	TBOOL *p;

	ok = YES;
	for (p = pfound; ac--; ++av, ++p)
		if (!(*p & FOUND))
			ok = remark(s, *av);
	free(pfound, NULL);
	return (ok);
	}

/*	set up for error map
 */
VOID ersetup(ac)
	BYTES ac;
	{
	IMPORT TBOOL *pfound;
	TBOOL *p;

	if (!ac)
		ac = 1;
	pfound = alloc(ac * sizeof (TBOOL), NULL);
	for (p = pfound; ac--; ++p)
		*p = 0;
	}

/*	mark ith module in av list as found in the library
 */
VOID found(i)
	BYTES i;
	{
	IMPORT TBOOL *pfound;

	pfound[i] =| FOUND;
	}

/*	get entry header
 */
BOOL gthdr(fd, phdr)
	FILE fd;
	HDR *phdr;
	{
	IMPORT BYTES vers;
	IMPORT COUNT hsize, nsize;
	IMPORT LONG lstol(), msize;
	FAST COUNT i;
	UCOUNT wo;
	COUNT j;

	if (fd < 0 || (j = fread(fd, phdr, hsize)) != hsize || !phdr->w.name[0])
		return (NO);
	else
		{
		switch (vers)
			{
		case 0:
			wo = lstou(phdr->w.wsize);
			msize = (LONG)wo;
			break;
		case 3:
			for (i = 4, msize = 0; 0 <= --i; )
				msize = (msize << 8) + phdr->v7.v7size[i];
			break;
		case 6:
			wo = lstou(phdr->v6.v6size);
			msize = (LONG)wo;
			break;
		case 7:
			msize = lstol(phdr->v7.v7size);
			break;
			}
		phdr->w.name[nsize] = '\0';
		return (YES);
		}
	}

/*	test if in arg list
 */
TEXT *inlist(ac, av, name)
	BYTES ac;
	TEXT **av, *name;
	{
	COUNT j;

	if ((j = inpos(ac, av, name)) < 0)
		return (NULL);
	else
		return (av[j]);
	}

/*	return position of arg in av list
 */
COUNT inpos(ac, av, name)
	BYTES ac;
	TEXT **av, *name;
	{
	IMPORT TBOOL *pfound;
	INTERN TEXT buf[NSIZE];
	FAST COUNT i;
	BOOL strip;
	TEXT *pn;

	for (i = 0; i < ac; ++i, ++av)
		{
		if (strip = pfound[i] & STRPNM)
			pn = strpath(buf, *av);
		if (cmpstr(strip ? pn : *av, name))
			return (i);
		}
	return (-1);
	}

/*	create a library file
 */
FILE lcreate(file)
	TEXT *file;
	{
	IMPORT UCOUNT magic;
	IMPORT VOID *endup(), exit(), *(*nxtexit)();
	FAST FILE fd;
	UCOUNT tmagic;

	itols(&tmagic, magic);
	if (!file)
		{
		file = uname();
		if (!nxtexit)
			nxtexit = onexit(&endup);
		onintr(&exit);
		}
	if ((fd = create(file, WRITE, 1)) < 0)
		error("can't create ", file);
	else
		fwrite(fd, &tmagic, MAGSIZE);
	return (fd);
	}

/*	open a library file
 */
FILE lopen(file, mand)
	TEXT *file;
	BOOL mand;
	{
	IMPORT BYTES vers;
	IMPORT COUNT hsize, nsize, round;
	IMPORT UCOUNT magic;
	FAST FILE fd;

	if ((fd = open(file, READ, 1)) < 0)
		{
		if (mand)
			error("can't open library ", file);
		}
	else if (fread(fd, &magic, MAGSIZE) != MAGSIZE)
		error("can't read header for ", file);
	else
		magic = lstoi(&magic);
	setvers();
	return (fd);
	}

/*	do library function specified by flags
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL cfl, dfl, ifl, pfl, rfl, tfl, xfl, vfl, v3fl, v6fl, v7fl;
	IMPORT BOOL del(), repl(), tab(), xtract();
	IMPORT BYTES vers;
	FAST BOOL (*pfunc)();
	FAST BOOL special;
	FAST TEXT **plib, *s;

	if (ac <= 1)
		usage("expects flags <lfile> <files>\n");
	if (av[1][0] == '-' || av[1][0] == '+')
		{
		getflags(&ac, &av, "c,d,i,p,r,t,v3,v6,v7,v,x:F <lfile> <files>", &cfl,
			&dfl, &ifl, &pfl, &rfl, &tfl, &v3fl, &v6fl, &v7fl, &vfl, &xfl);
		if (ac)
			{
			plib = av++;
			--ac;
			}
		else
			usage("expects flags <lfile> <files>\n");
		}
	else
		{
		plib = ++av;
		--ac;
		getflags(&ac, &av, "c,d,i,p,r,t,v3,v6,v7,v,x:<lfile> F <files>", &cfl,
			&dfl, &ifl, &pfl, &rfl, &tfl, &v3fl, &v6fl, &v7fl, &vfl, &xfl);
		}
	special = NO;
	vers = (v3fl) ? 3 : (v6fl) ? 6 : (v7fl) ? 7 : 0;
	if (1 < cfl + dfl + pfl + rfl + tfl + xfl)
		usage("takes only one of -[c d p r t x]\n");
	else if (xfl)
		pfunc = &xtract;
	else if (rfl)
		pfunc = &repl;
	else if (pfl)
		{
		pfunc = &xtract;
		special = YES;
		}
	else if (dfl)
		pfunc = &del;
	else if (cfl)
		{
		pfunc = &repl;
		special = YES;
		}
	else
		pfunc = &tab;
	if (ifl)
		getin(&ac, &av);
	ersetup(ac);
	markpth(ac, av);
	return ((*pfunc)(ac, av, *plib, special));
	}

/*	mark ith module in av list as having a pathname to strip
 */
VOID markpth(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT TBOOL *pfound;
	FAST COUNT i, x;
	FAST TEXT *pn;
	TEXT *s;

	for (i = 0; i < ac; ++i)
		if (*(s = av[i]))
			{
			for (pn = s; pn[x = instr(pn, ":]/")]; pn =+ x + 1)
				;
			if (pn != s)
				pfound[i] =| STRPNM;
			}
	}

/*	put back library file
 */
VOID putback(to, sx, ns)
	TEXT *to;
	LONG *sx, ns;
	{
	IMPORT COUNT round;
	IMPORT LONG msize;
	FAST FILE lfd, tfd;
	HDR hdr;

	tfd = lopen(uname(), YES);
	lfd = lcreate(to);
	while (gthdr(tfd, &hdr))
		{
		if (msize == MEMPTY)
			msize = 0L;
		else if (0 < msize)
			;
		else if (0 <= --ns)
			msize = *sx++;
		else
			error("Panic putback!", NULL);
		pthdr(lfd, &hdr);
		if (msize)
			copy(tfd, lfd, msize + (msize & round));
		}
	wreof(lfd);
	close(lfd);
	close(tfd);
	}

/*	put header
 */
VOID pthdr(fd, phdr)
	FILE fd;
	HDR *phdr;
	{
	IMPORT BYTES vers;
	IMPORT COUNT hsize;
	IMPORT LONG msize;
	IMPORT TEXT *ltols();
	FAST COUNT i;
	LONG lo;
	UCOUNT wo;

	for (i = 0; i < NSIZE && phdr->w.name[i]; ++i)
		;
	while (i < NSIZE+8)
		phdr->w.name[i++] = '\0';
	switch (vers)
		{
	case 0:
		itols(phdr->w.wsize, wo = (UCOUNT)msize);
		break;
	case 3:
		for (lo = msize, i = 0; i < 4; ++i)
			{
			phdr->v7.v7size[i] = lo;
			lo =>> 8;
			}
		break;
	case 6:
		itols(phdr->v6.v6size, wo = (UCOUNT)msize);
		break;
	case 7:
		ltols(phdr->v7.v7size, msize);
		break;
		}
	fwrite(fd, phdr, hsize);
	}

/*	replace files in library
 */
BOOL repl(ac, av, file, virgin)
	BYTES ac;
	TEXT **av, *file;
	BOOL virgin;
	{
	IMPORT BOOL vfl;
	IMPORT COUNT round;
	IMPORT LONG copy(), msize;
	IMPORT TBOOL *pfound;
	FAST COUNT i, j;
	FAST TEXT *s;
	FILE ifd, lfd, tfd;
	HDR hdr;
	LONG *svec, *sx;
	TEXT *pn;

	if (!ac)
		error("no replacement files", NULL);
	if (virgin)
		{
		setvers();
		lfd = -1;
		}
	else
		lfd = lopen(file, NO);
	tfd = lcreate(NULL);
	svec = alloc(ac * sizeof (*svec), NULL);
	for (sx = svec; gthdr(lfd, &hdr); )
		if ((j = inpos(ac, av, hdr.w.name)) < 0)
			{
			if (vfl)
				report("c ", hdr.w.name);
			if (msize == 0)
				msize = MZERO;
			pthdr(tfd, &hdr);
			if (msize != MZERO)
				copy(lfd, tfd, msize + (msize & round));
			}
		else
			{
			lseek(lfd, msize + (msize & round), 1);
			s = av[j];
			if (0 <= (ifd = open(s, READ, 1)))
				{
				if (vfl)
					report("r ", hdr.w.name);
				found(j);
				msize = NOSIZE;
				pthdr(tfd, &hdr);
				*sx++ = copy(ifd, tfd, 0L);
				close(ifd);
				*s = '\0';
				}
			}
	close(lfd);
	for (j = 0; j < ac; ++j)
		if (*(s = av[j]))
			{
			if (pfound[j] & STRPNM)
				{
				pn = strpath(hdr.w.name, av[j]);
				for (i = 0; i < NSIZE && (hdr.w.name[i] = *pn); ++i, ++pn)
					;
				}
			else
				for (i = 0; i < NSIZE && (hdr.w.name[i] = s[i]); ++i)
					;
			msize = NOSIZE;
			if (0 <= (ifd = open(s, READ, 1)))
				{
				if (vfl)
					report("a ", hdr.w.name);
				found(j);
				pthdr(tfd, &hdr);
				*sx++ = copy(ifd, tfd, 0L);
				close(ifd);
				}
			}
	close(tfd);
	if (erprint(ac, av, "lib: can't replace "))
		{
		putback(file, svec, (LONG)(sx - svec));
		return (YES);
		}
	else
		return (NO);
	}

/*	print non-fatal report
 */
VOID report(s1, s2)
	TEXT *s1, *s2;
	{
	putstr(STDOUT, s1, s2, NULL);
	putstr(STDOUT, "\n", NULL);
	}

/*	(re)set version and configuration info
 */
VOID setvers()
	{
	IMPORT BYTES vers;
	IMPORT COUNT hsize, nsize, round;
	IMPORT UCOUNT magic;

	if (!magic)
		magic = (vers == 6) ? MAGIC6 : (vers == 3 || vers == 7) ?
			MAGIC7 : MAGICW;
	switch (magic)
		{
	case MAGICW:
		hsize = 16;
		nsize = NSIZE;
		round = 0;
		vers = 0;
		break;
	case MAGIC6:
		hsize = 16;
		nsize = N6SIZE;
		round = 1;
		vers = 6;
		break;
	case MAGIC7:
		hsize = 26;
		nsize = NSIZE;
		round = 1;
		if (vers != 3 && vers != 7)
			vers = 7;
		break;
	default:
		error("bad library header", NULL);
		}
	}

/*	skip bytes in a file
 */
VOID skip(fd, size, rnd)
	FILE fd;
	LONG size;
	BOOL rnd;
	{
	if (size)
		lseek(fd, size + (size & rnd), 1);
	}

/*	strip a pathname prefix
 */
TEXT *strpath(buf, name)
	TEXT *buf, *name;
	{
	FAST TEXT *pn = buf;

	pn = pathnm(buf, name, ":");
#ifdef IDRIS
	pn =+ 2;
#else
	++pn;
#endif
	return(pn);
	}

/*	tabulate library contents
 */
BOOL tab(ac, av, file)
	BYTES ac;
	TEXT **av, *file;
	{
	IMPORT BOOL vfl;
	IMPORT BYTES vers;
	IMPORT COUNT round;
	IMPORT LONG msize;
	FAST BOOL doall = (!ac);
	FAST FILE lfd;
	HDR hdr;
	TEXT buf[10];

	buf[0] = ':';
	buf[1] = '\t';
	lfd = lopen(file, YES);
	if (vfl)
		{
		putstr(STDOUT, "Library ", file, NULL);
		switch (vers)
			{
		case 0:
			putstr(STDOUT, " standard format.\n", NULL);
			break;
		case 3:
			putstr(STDOUT, " Unix III format.\n", NULL);
			break;
		case 6:
			putstr(STDOUT, " Unix V6 format.\n", NULL);
			break;
		case 7:
			putstr(STDOUT, " Unix V7 format.\n", NULL);
			}
		}
	while (gthdr(lfd, &hdr))
		{
		if (doall || inlist(ac, av, hdr.w.name))
			if (vfl)
				{
				buf[ltob(&buf[2], msize, 10) + 2] = '\0';
				report(hdr.w.name, buf);
				}
			else
				report(hdr.w.name, NULL);
		if (!doall)
			found(inpos(ac, av, hdr.w.name));
		skip(lfd, msize, round);
		}
	close(lfd);
	return (doall || erprint(ac, av, "lib: can't find "));
	}

/*	write 2 bytes of padding at EOF
 */
VOID wreof(fd)
	FILE fd;
	{
	fwrite(fd, "\0", 2);
	}

/*	extract files from library
 */
BOOL xtract(ac, av, file, print)
	BYTES ac;
	TEXT **av, *file;
	BOOL print;
	{
	IMPORT BOOL vfl;
	IMPORT COUNT round;
	IMPORT LONG msize;
	FAST BOOL doall = (!ac);
	FAST FILE lfd, ofd;
	BOOL ok;
	HDR hdr;

	ok = YES;
	lfd = lopen(file, YES);
	while (gthdr(lfd, &hdr))
		if (!doall && !inlist(ac, av, hdr.w.name))
			skip(lfd, msize, round);
		else
			{
			if (vfl)
				report("x ", hdr.w.name);
			if (!doall)
				found(inpos(ac, av, hdr.w.name));
			if (!print)
				{
				if ((ofd = create(hdr.w.name, WRITE, 1)) < 0)
					ok = remark("can't create ", hdr.w.name);
				}
			else
				ofd = STDOUT;
			if (ofd < 0)
				skip(lfd, msize, round);
			else if (msize)
				{
				copy(lfd, ofd, msize);
				skip(lfd, msize & round, NO);
				}
			if (!print)
				close(ofd);
			}
	close(lfd);
	return ((doall || erprint(ac, av, "lib: can't extract ")) && ok);
	}
