/*	LINKER FOR ALL IDRIS BINARY
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "link.h"

VOID *(*nxtexit)() {NULL};

/*	do code relocation for text or data segment
 */
LONG docode(outbuf, rdbuf, ns, pp, pbhdr, bias, nin, ninit)
	UCOUNT outbuf, rdbuf, ns;
	SYMBOL **pp;
	BHDR *pbhdr;
	LONG bias, nin, ninit;
	{
	IMPORT BHDR obhdr;
	IMPORT BOOL longint;
	IMPORT BYTES xfl;
	IMPORT LONG xstol();
	IMPORT UCOUNT xstos();
	IMPORT ULONG bsiz, dsiz, tsiz;
	IMPORT BOOL lsfmt;
	FAST UCOUNT b;
	FAST COUNT co;
	FAST SYMBOL *p;
	COUNT i, wo;
	LONG n, nabs, val;
	TEXT buf[4];
	LONG zz;
	INTERN BOOL n16k = NO;

	for (nabs = 0; co = getby(HIBUF); nin =- n)
		if (co < 32)
			{
			n = co;
			nabs =+ n;
			}
		else if (co < 64)
			{
			n = ((co - 32) << 8) + getby(HIBUF) + 32;
			nabs =+ n;
			}
		else
			{
			for (n = 0; n < nabs; ++n)
				relby(outbuf, getby(LOBUF));
			nabs =+ ninit;
			ninit = 0;
			for (; 8223 < nabs; nabs =- 8223)
				{
				relby(rdbuf, 0x3f);
				relby(rdbuf, 0xff);
				}
			b = nabs;
			nabs = 0;
			if (32 <= b)
				{
				b =- 32;
				relby(rdbuf, b >> 8 | 32);
				relby(rdbuf, b);
				}
			else if (b)
				relby(rdbuf, b);
			b = (co >> 2) - 16;
			if (b == 47)
				{
				b =+ getby(HIBUF);
				if (175 <= b)
					b = ((b - 175) << 8) + getby(HIBUF) + 175;
				}
			switch (b)
				{
			case 0:
				val = 0;
				break;
			case 1:
				val = pbhdr->atbias;
				if (!(xfl & 2))
					b = 2;
				break;
			case 2:
				val = pbhdr->adbias;
				if (xfl & 1)
					b = 1;
				break;
			case 3:
				val = bsiz - (pbhdr->dsiz + pbhdr->dbias);
				break;
			case 0x8000:		/* 16032, switch byte order */
				lsfmt = !lsfmt;
				break;
			case 0x8001:		/* set link for 16032 30 bit arithmetic */
				n16k = YES;
				break;
			default:
				if (ns <= (b =- 4) || !(p = pp[b]))
					error("bad symbol number", NULL);
				if (p->flags & DEFD)
					{
					val = p->value;
					b = p->flags & 03;
					}
				else
					{
					val = 0;
					b = p->value + 4;
					}
				}
			if (b < 47)
				relby(rdbuf, b + 16 << 2 | co & 03);
			else if (b < 175)
				{
				relby(rdbuf, 0xfc | co & 03);
				relby(rdbuf, b - 47);
				}
			else
				{
				b =- 175;
				relby(rdbuf, 0xfc | co & 03);
				relby(rdbuf, b >> 8 | 0200);
				relby(rdbuf, b);
				}
			n = 0;
			if (b + 175 >= 0x8000)
				continue;
			if (co & 01)
				val =- bias;
			if (co & 02)
				{
				for (i = 0; i < 4; ++i)
					buf[i] = getby(LOBUF);
				zz = xstol(buf);
				val += zz;
				if (n16k)
					val = (val & 0x3fffffff) | (zz & 0xc0000000);
				relint(outbuf, val);
				n = 4;
				if (!longint)
					xremark("bad relocation item", "");
				}
			else
				{
				buf[0] = getby(LOBUF);
				buf[1] = getby(LOBUF);
				wo = xstos(buf);
				val =+ wo;
				relwd(outbuf, wo = val);
				n = 2;
				if (longint && wo != val)
					xremark("range error", "");
				}
			}
	nabs =+ nin;
	for (n = 0; n < nabs; ++n)
		relby(outbuf, getby(LOBUF));
	return (nabs + ninit);
	}

/*	copy data relocation bits
 */
VOID drcopy()
	{
	IMPORT FILE tfd;
	IMPORT IOBUF obuf[];
	FAST COUNT i, n;
	FAST TEXT *p;
	LONG nin = obuf[DREL].off;

	lseek(tfd, 0L, 0);
	for ( ; nin; nin =- n)
		{
		p = obuf[TDAT].buf;
		n = min(L_BUFSIZE, nin);
		if (fread(tfd, p, n) != n)
			error("can't read temp file", NULL);
		for (i = n; 0 <= --i; ++p)
			relby(TREL, *p);
		}
	}

/*	clean up on exit
 */
VOID *endeup()
	{
	IMPORT FILE tfd;
	IMPORT VOID *(*nxtexit)();

	close(tfd);
	remove(uname());
	return (nxtexit);
	}

/*	first module link, do symbol table
 */
BOOL link1(fd, mand)
	FILE fd;
	BOOL mand;
	{
	IMPORT BOOL longint;
	IMPORT BYTES xfl;
	IMPORT COUNT lenname;
	IMPORT LONG rebias(), xstol();
	IMPORT ULONG bsiz, dsiz, tsiz;
	FAST COUNT i;
	FAST SYMBOL *q;
	FAST TEXT *p;
	BHDR bhdr;
	COUNT nb, ns, nv;
	LONG val;
	TEXT *ps;

	ps = gtsyms(fd, &bhdr);
	nv = longint ? 4 : 2;
	nb = nv + 1 + lenname;
	ns = bhdr.sbytes / nb;
	if (!mand)
		for (p = ps + nv, i = 0; i < ns; p =+ nb, ++i)
			if (*p & EXTERN && *p & DEFD &&
				(q = lookup(p + 1)) && !q->flags)
				{
				mand = YES;
				break;
				}
	if (mand)
		{
		for (p = ps + nv, i = 0; i < ns; p =+ nb, ++i)
			{
			val = (longint) ? xstol(p - 4) : (LONG)xstos(p - 2);
			switch (*p & 07)
				{
			case 05:
				val =+ bhdr.atbias;
				if (!(xfl & 02))
					*p = *p & ~07 | 06;
				break;
			case 06:
				val =+ bhdr.adbias;
				if (xfl & 01)
					*p = *p & ~07 | 05;
				break;
			case 07:
				val =+ dsiz - bhdr.dbias + bhdr.dsiz;
				}
/*			val =+ rebias(*p, bhdr.tbias, bhdr.dbias, bhdr.dbias + bhdr.dsiz);*/
			if (!(*p & EXTERN))
				;
			else if (!(q = lookup(p + 1)))
				addsym(p + 1, (*p & DEFD) ? *p & (EXTERN|DEFD|03) : 0, val);
			else if (*p & DEFD && q->flags)
				xremark("multiply defined: ", p + 1);
			else if (*p & DEFD)
				{
				q->flags = *p & (EXTERN|DEFD|03);
				q->value = val;
				}
			else if (!q->flags)
				q->value = max(val, q->value);
			}
		bsiz =+ bhdr.bsiz;
		tsiz =+ bhdr.atsiz;
		dsiz =+ bhdr.adsiz;
		}
	free(ps, NULL);
	return (mand);
	}

/*	second module link, do code relocation
 */
VOID link2(loff)
	LONG loff;
	{
	IMPORT BHDR obhdr;
	IMPORT BYTES xfl;
	IMPORT BOOL lsfmt, txtrev;
	IMPORT COUNT lenname;
	IMPORT FILE ifd;
	IMPORT IOBUF ibuf[2];
	IMPORT LONG docode(), iseek, xstol();
	IMPORT ULONG bsiz, dsiz, tsiz;
	FAST COUNT i;
	FAST SYMBOL *q;
	FAST TEXT *p;
	BHDR bhdr;
	COUNT nb, ns, nv;
	SYMBOL **pp;
	TEXT *ps;
	INTERN LONG nabs[2];
	BOOL svlsfmt;

	ps = gtsyms(ifd, &bhdr);
	nv = longint ? 4 : 2;
	nb = nv + 1 + lenname;
	ns = bhdr.sbytes / nb;
	for (pp = ps, p = ps + nv, i = 0; i < ns; p =+ nb, ++i)
		{
		if ((*p & (EXTERN|DEFD)) != EXTERN)
			q = NULL;
		else if (!(q = lookup(p + 1)))
			error("phase error", NULL);
		*pp++ = q;
		}
	loff =+ longint ? 28 : 16;
	lseek(ifd, loff, 0);
	iseek = loff;
	ibuf[LOBUF].off = loff & NOT_L_BUF_MASK;
	ibuf[LOBUF].n = (COUNT)loff & L_BUF_MASK;
	ibuf[LOBUF].start = ibuf[LOBUF].n + L_BUFSIZE;
	loff =+ bhdr.tsiz + bhdr.dsiz + bhdr.sbytes;
	ibuf[HIBUF].off = loff & NOT_L_BUF_MASK;
	ibuf[HIBUF].n = loff & L_BUF_MASK;
	ibuf[HIBUF].start = ibuf[HIBUF].n + L_BUFSIZE;
	if (txtrev)
		lsfmt = !lsfmt;
	else
		svlsfmt = lsfmt;
	nabs[0] = docode((xfl & 2) ? TDAT : DDAT, (xfl & 2) ? TREL : DREL,
		ns, ps, &bhdr, bhdr.atbias, bhdr.tsiz, nabs[0]);
	if (txtrev)
		lsfmt = !lsfmt;
	else
		lsfmt = svlsfmt;
	nv = "\0\1\1\0"[xfl];
	nabs[nv] = docode((xfl & 1) ? TDAT : DDAT, (xfl & 1) ? TREL : DREL,
		ns, ps, &bhdr, bhdr.adbias, bhdr.dsiz, nabs[nv]);
	if (!txtrev)
		lsfmt = svlsfmt;
	free(ps, NULL);
	bsiz =+ bhdr.bsiz;
	tsiz =+ bhdr.atsiz;
	dsiz =+ bhdr.adsiz;
	}

/*	COMBINE RELOCATABLE MODULES
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL afl, cfl, dfl, hfl, ifl, ok, rfl, tfl;
	IMPORT BYTES dround, tfill, xfl;
	IMPORT ULONG bbias, bpad, dbias, tbias;
	IMPORT IOBUF ibuf[2], obuf[3];
	IMPORT LIBLIST llist;
	IMPORT LONG drmask;
	IMPORT TEXT *endbss, *enddata, *endtext, *ofile, *sizbss, *sizdat, *siztxt;
	IMPORT ULIST ulist;
	FAST COUNT i;
	FAST TEXT *s;
	TEXT buf[6][L_BUFSIZE];

	getflags(&ac, &av,
		"a,bb##,b##,c,db##,dr#,d,eb*,ed*,et*,h,i,l*>o*\
		,r,sb*,sd*,st*,tb##,tf#,t,u*>x#:F <files>",
		&afl, &bbias, &bpad, &cfl, &dbias, &dround, &dfl, &endbss, &enddata, &endtext,
		&hfl, &ifl, &llist, &ofile, &rfl, &sizbss, &sizdat, &siztxt, &tbias,
		&tfill, &tfl, &ulist, &xfl);
	xfl =& 03;
	if (ifl)
		getin(&ac, &av);
	drmask = (1L << (dround & 017)) - 1;
	for (i = ulist.nm; i < NUND; ++i)
		addusym(ulist.sym[i]);
	ibuf[LOBUF].buf = buf[0];
	ibuf[HIBUF].buf = buf[1];
	obuf[TDAT].buf = buf[2];
	obuf[DDAT].buf = buf[3];
	obuf[TREL].buf = buf[4];
	obuf[DREL].buf = buf[5];
	ok = YES;
	if (pass1(ac, av) && mid1() && mid2() && pass2(ac, av))
#ifdef IDRIS
		{
		chmod(ofile, 0777);
		return (YES);
		}
#else
		return (YES);
#endif
	else
		return (NO);
	}

/*	tidy up symbol table
 */
BOOL mid1()
	{
	IMPORT BHDR obhdr;
	IMPORT BOOL dfl, longint, ok, tfl;
	IMPORT BYTES tfill;
	IMPORT ULONG bbias, bpad, dbias, tbias;
	IMPORT COUNT lenname, nsyms, nund;
	IMPORT LONG drmask, rebias();
	IMPORT SYMBOL *stabs[];
	IMPORT TEXT *endbss, *enddata, *endtext, *sizbss, *sizdat, *siztxt;
	IMPORT ULONG bsiz, dsiz, tsiz;
	FAST COUNT i;
	FAST SYMBOL *p;
	LONG tfmask = (1L << tfill) - 1;
	SYMBOL *pebss, *psbss;

	tsiz = tsiz + tfmask & ~tfmask;
	addend(endtext, EXTERN|DEFD|1, tsiz);
	addend(enddata, EXTERN|DEFD|2, dsiz);
	pebss = addend(endbss, EXTERN|DEFD|3, bsiz);
	addend(siztxt, EXTERN|DEFD, tsiz);
	addend(sizdat, EXTERN|DEFD, dsiz);
	psbss = addend(sizbss, EXTERN|DEFD, bsiz);
	obhdr.tsiz = tsiz;
	obhdr.dsiz = dsiz;
	obhdr.bsiz = bsiz;
	obhdr.ssiz = bpad;
	obhdr.tbias = tbias;
	obhdr.dbias = (dbias == -1) ? 
		(obhdr.tbias + obhdr.tsiz + drmask) & ~drmask : dbias;
	obhdr.sbytes = tfl ? 0 : nsyms * ((longint ? 5 : 3) + lenname);
	bsiz = (bbias == -1) ? obhdr.dbias + dsiz : bbias;
	dsiz = obhdr.dbias;
	tsiz = obhdr.tbias;
	for (i = 0; i < nsyms; ++i, ++p)
		{
		if (!(i & SMASK))
			p = stabs[i >> SOFF];
		if (p->flags)
			{
			p->flags =| EXTERN;
			p->value =+ rebias(p->flags, 0L, 0L, 0L);
			}
		else if (p->value && !dfl)
			{
			p->flags = EXTERN|DEFD|3;
			obhdr.bsiz =+ p->value;
			pebss->value =+ p->value;
			psbss->value =+ p->value;
			p->value = obhdr.bsiz - p->value + bsiz;
			}
		else if (!dfl)
			xremark("undefined: ", p->name);
		else 
			++nund;
		}
	return (ok);
	}

/*	set up output file
 */
BOOL mid2()
	{
	IMPORT BHDR obhdr;
	IMPORT BOOL afl, cfl, hfl, longint, ok, rfl, tfl;
	IMPORT COUNT lenname, nsyms;
	IMPORT FILE ofd, tfd;
	IMPORT IOBUF obuf[2];
	IMPORT TEXT *uname();
	IMPORT UCOUNT binhdr;
	IMPORT SYMBOL *stabs[];
	IMPORT TEXT *ofile;
	IMPORT VOID *endeup(), exit(), *(*nxtexit)();
	FAST COUNT i, k;
	FAST SYMBOL *p;
	LONG loff;

	if ((ofd = create(ofile, WRITE, 1)) < 0)
		error("can't create ", ofile);
	if (!afl && !rfl)
		{
		if ((tfd = create(uname(), UPDATE, 1)) < 0)
			error("can't create tmp file", NULL);
		nxtexit = onexit(&endeup);
		onintr(&exit);
		}
	if (cfl)
		{
		obhdr.tsiz = 0;
		obhdr.dsiz = 0;
		obhdr.bsiz = 0;
		obhdr.ssiz = 0;
		obhdr.tbias = 0;
		obhdr.dbias = 0;
		}
	obuf[TREL].off = obhdr.tsiz + obhdr.dsiz + (hfl ? 0 : longint ? 28 : 16);
	obuf[TREL].start = (COUNT)obuf[TREL].off & L_BUF_MASK;
	obuf[TREL].n = obuf[TREL].start;
	obuf[DDAT].off = obhdr.tsiz + (hfl ? 0 : longint ? 28 : 16);
	obuf[DDAT].start = (COUNT)obuf[DDAT].off & L_BUF_MASK;
	obuf[DDAT].n = obuf[DDAT].start;
	if (!hfl)
		{
		relby(TDAT, 0x99);
		relby(TDAT, binhdr >> 8 & 0177 | (rfl ? 0200 : 0));
		relwd(TDAT, obhdr.sbytes);
		relint(TDAT, obhdr.tsiz);
		relint(TDAT, obhdr.dsiz);
		relint(TDAT, obhdr.bsiz);
		relint(TDAT, obhdr.ssiz);
		relint(TDAT, obhdr.tbias);
		relint(TDAT, obhdr.dbias);
		}
	if (!tfl)
		{
		for (i = 0, k = 0; k < nund && i < nsyms; ++i, ++p)
			{
			if (!(i & SMASK))
				p = stabs[i >> SOFF];
			if (!(p->flags & DEFD))
				{
				p->flags =| EXTERN;
				relsym(p);
				p->value = k;
				++k;
				}
			}
		for (i = 0; i < nsyms; ++i, ++p)
			{
			if (!(i & SMASK))
				p = stabs[i >> SOFF];
			if (p->flags & DEFD)
				relsym(p);
			}
		}
	return (ok);
	}

/*	first pick the modules to link
 */
BOOL pass1(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL ok;
	IMPORT TEXT *fname;
	IMPORT UCOUNT binhdr, gtmagic();
	FAST COUNT hsize;
	UCOUNT header;
	FAST FILE fd;
	LONG loff;
	TEXT lhdr[26];
	TEXT nsize;

	for (gtlfiles(NULL); 0 <= (fd = gtlfiles(&ac, &av)); close(fd))
		if (fd == STDIN)
			error("can't link from STDIN", NULL);
		else if (fd == STDERR)
			error("can't open: ", fname);
		else if ((header = gtmagic(fd)) == binhdr)
			ok = link1(fd, YES);
		else if (header != 0177565 && header != 0177555 && header != 0177545)
			error("bad file format: ", fname);
		else
			{
			hsize = (header == 0177545) ? 26 : 16;
			nsize = (header == 0177555) ? 8 : 14;
			for (loff = 2; (fread(fd, &lhdr, hsize) == hsize) && lhdr[0];
				lseek(fd, loff, 0))
				{
				if (gtmagic(fd) != binhdr)
					{
					remark("bad library file in ", fname);
					write(STDERR, lhdr, nsize - 2);
					write(STDERR, "\n", 1);
					exit(NO);
					}
				else if (link1(fd, NO))
					addlib(loff + hsize);
				loff =+ hsize + ((header != 0177545) ? (LONG)lstoi(lhdr + 14)
					: ((LONG)lstoi(lhdr + 22) << 16) + lstoi(lhdr + 24));
				if (header != 0177565)
					loff =+ loff & 1;
				}
			addlib(0L);
			}
	if (!binhdr)
		error("no object files", NULL);
	return (ok);
	}

/*	finally put out code
 */
BOOL pass2(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL afl, cfl, ok, rfl;
	IMPORT FILE ifd, tfd;
	IMPORT LONG liboff[];
	IMPORT UCOUNT binhdr, gtmagic();
	UCOUNT header;
	LONG loff, *lptr;

	if (!cfl)
		for (gtlfiles(NULL), lptr = liboff;
			0 <= (ifd = gtlfiles(&ac, &av)); close(ifd))
			if ((header = gtmagic(ifd)) == binhdr)
				link2(0L);
			else
				while (loff = *lptr++)
					{
					lseek(ifd, loff + 2, 0);
					link2(loff);
					}
	relby(TDAT|DRAIN, 0);
	relby(DDAT|DRAIN, 0);
	afl = NO;
	relby(TREL, 0);
	relby(WRMI|DREL|DRAIN, 0);
	drcopy();
	relby(TREL, 0);
	relby(WRMI|TREL|DRAIN, 0);
	return (ok);
	}
