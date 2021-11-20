/*	AUXILIARY FILE I/O FOR A-NATURAL
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "a.h"

#define CCODE	1
#define CDATA	0
#define CEND	2

#define HDRSIZ	(2 + sizeof (BYTES))
typedef struct {
	BYTES cbase;
	TEXT ccode;
	TEXT count;
	TEXT cbuf[128];
	} CBUF;

LOCAL BOOL dseg {NO};
LOCAL CBUF cb[2] {NULL, CCODE};

/*	output file stuff
 */
#define HIBUF	1
#define LOBUF	0
typedef struct {
	LONG off;
	COUNT start, n;
	TEXT *buf;
	} OBUF;

LOCAL OBUF obuf[2] {0};

/*	drain a temp file buffer
 */
VOID drain(seg)
	COUNT seg;
	{
	IMPORT CBUF cb[];
	IMPORT FIO *outfio[];
	FAST CBUF *pcb = &cb[seg];

	if (pcb->count || pcb->ccode == CEND)
		{
		putl(outfio[seg], pcb, pcb->count + HDRSIZ);
		pcb->count = 0;
		pcb->cbase = NULL;
		}
	}

/*	put stuff to temp file
 */
VOID putcode(code, arg)
	COUNT code, arg;
	{
	IMPORT BOOL dseg;
	IMPORT CBUF cb[];
	IMPORT FIO *outfio[];
	FAST COUNT i;

	switch (code)
		{
	case DATA:
		if (cb[dseg].count == 127)
			drain(dseg);
		cb[dseg].cbuf[cb[dseg].count++] = arg;
		break;
	case END:
		for (i = 2; 0 <= --i; )
			{
			drain(i);
			cb[i].ccode = CEND;
			drain(i);
			putc(outfio[i], -1);
			}
		break;
	case RELOC:
		drain(dseg);
		cb[dseg].cbase = arg;
		break;
	case XSEG:
		dseg = !dseg;
		break;
	default:
		err("putcode!");
		}
	}

/*	put a relocation byte
 */
VOID relby(oseg, b)
	COUNT oseg;
	TINY b;
	{
	IMPORT FILE ofd;
	IMPORT OBUF obuf[];
	FAST COUNT n;
	FAST OBUF *p = &obuf[oseg & 01];
	INTERN LONG off;

	if (p->n < BUFSIZE && oseg <= 1)
		;
	else if (p->off != off && lseek(ofd, p->off, 0) < 0 ||
		(n = p->n - p->start) && write(ofd, p->buf + p->start, n) != n)
		{
		err("can't write object file");
		exit(NO);
		}
	else
		{
		p->off =+ n;
		p->start = 0;
		p->n = 0;
		off = p->off;
		}
	if (oseg <= 1)
		p->buf[p->n++] = b;
	}

/*	map temp file to relocatable output
 */
VOID relout(csize, dsize)
	BYTES csize, dsize;
	{
	IMPORT FILE tfd[];
	IMPORT FIO *outfio[];
	IMPORT OBUF obuf[];
	IMPORT TERM *cobase, *dabase, *psym[];
	FAST COUNT k, n;
	FAST TERM *p;
	COUNT u;

	cobase->val = 0;
	dabase->val = 0;
	for (n = 0, u = 3, k = NSYM; 0 <= --k; )
		for (p = psym[k]; p; p = p->next)
			{
			if (!p->base)
				;
			else if (p->base == cobase)
				p->base = 1;
			else if (p->base == dabase)
				{
				p->val =+ csize;
				p->base = 2;
				}
			else
				{
				p->ty =| PUBF;
				p->base = ++u;
				}
			if (p->ty & PUBF)
				++n;
			}
	if (020000/12 <= n)
		err("too many symbols");
	obuf[LOBUF].buf = outfio[0]->_buf;
	obuf[HIBUF].off = 020L + csize + dsize;
	obuf[HIBUF].start = (COUNT)obuf[1].off & 0777;
	obuf[HIBUF].n = obuf[1].start;
	obuf[HIBUF].buf = outfio[1]->_buf;
	relwd(LOBUF, 0x1499);
	relwd(LOBUF, n * 12);
	relwd(LOBUF, csize);
	relwd(LOBUF, dsize);
	relwd(LOBUF, 0);
	relwd(LOBUF, 0);
	relwd(LOBUF, 0);
	relwd(LOBUF, csize);
	for (k = NSYM; 0 <= --k; )
		for (p = psym[k]; p && 0 < n; p = p->next)
			if (4 <= p->base)
				{
				relsym(p);
				--n;
				}
	for (k = NSYM; 0 <= --k; )
		for (p = psym[k]; p && 0 < n; p = p->next)
			if (p->ty & PUBF && p->base < 4)
				{
				relsym(p);
				--n;
				}
	relseg(tfd[0]);
	relseg(tfd[1]);
	relwd(2, 0);
	relwd(3, 0);
	}

/*	put a segment with relocation
 */
VOID relseg(fd)
	FILE fd;
	{
	IMPORT CBUF cb[];
	IMPORT FIO *infio;
	FAST COUNT b, i, n;
	UCOUNT nabs;

	lseek(fd, 0L, 0);
	finit(infio, fd, READ);
	nabs = 0;
	FOREVER
		{
		tread(&cb[0], HDRSIZ);
		if (cb[0].ccode == CEND)
			break;
		n = cb[0].count;
		tread(&cb[0].cbuf, n);
		if (!cb[0].cbase)
			{
			nabs =+ n;
			for (i = 0; i < n; ++i)
				relby(LOBUF, cb[0].cbuf[i]);
			}
		else
			{
			i = cb[0].cbase->val + (cb[0].cbuf[0] & BYTMASK)
				+ (cb[0].cbuf[1] << 8);
			b = (COUNT)cb[0].cbase->base;
			cb[0].cbuf[0] = i;
			cb[0].cbuf[1] = i >> 8;
			for (i = 0; i < n; ++i)
				relby(LOBUF, cb[0].cbuf[i]);
			if (!b)
				nabs =+ n;
			else
				{
				for (; 8223 <= nabs; nabs =- 8223)
					{
					relby(HIBUF, 0x3f);
					relby(HIBUF, 0xff);
					}
				if (32 <= nabs)
					{
					nabs =- 32;
					relby(HIBUF, nabs >> 8 | 32);
					relby(HIBUF, nabs);
					}
				else if (nabs)
					relby(HIBUF, nabs);
				nabs = n - 2;
				if (b < 47)
					relby(HIBUF, b + 16 << 2);
				else if (b < 175)
					{
					relby(HIBUF, 0xfc);
					relby(HIBUF, b - 47);
					}
				else
					{
					b =- 175;
					relby(HIBUF, 0xfc);
					relby(HIBUF, 0x80 | b >> 8);
					relby(HIBUF, b);
					}
				}
			}
		}
	relby(HIBUF, 0);
	}

/*	put a symbol to object file
 */
VOID relsym(p)
	FAST TERM *p;
	{
	FAST COUNT m;

	relwd(HIBUF, p->val);
	m = ((COUNT)p->base < 4) ? (COUNT)p->base | 004 : 0;
	if (p->ty & PUBF)
		m =| 010;
	relby(HIBUF, m);
	for (m = 0; m < LENNAME; ++m)
		relby(HIBUF, p->nm[m]);
	for ( ; m < 9; ++m)
		relby(HIBUF, 0);
	}

/*	put a relocation word
 */
VOID relwd(oseg, w)
	COUNT oseg, w;
	{
	relby(oseg, w);
	relby(oseg, w >> 8);
	}

/*	read with test
 */
VOID tread(buf, count)
	FAST TEXT *buf;
	FAST COUNT count;
	{
	IMPORT FIO *infio;

	while (0 <= --count)
		*buf++ = gtc(infio);
	}
