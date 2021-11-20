/*	CP/M OPEN/CREATE/DELETE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

#define NODEV	127

LOCAL VOID *(*_clink)() {NULL};

#define DEV struct devs
LOCAL DEV {
	TEXT *nm;
	TINY devno;
	} _dev[] {
	"LST", LST,
	"PUN", PUN,
	"RDR", RDR,
	"CON", CON,
	"A", 1,
	"B", 2,
	"C", 3,
	"D", 4,
	"E", 5,
	"F", 6,
	"G", 7,
	"H", 8,
	"I", 9,
	"J", 10,
	"K", 11,
	"L", 12,
	"M", 13,
	"N", 14,
	"O", 15,
	"P", 16,
	0};

/*	close files on exit
 */
LOCAL VOID *_cclose()
	{
	IMPORT BYTES _nfiles;
	IMPORT VOID *(*_clink)();
	FAST COUNT fd;

	for (fd = 0; fd < _nfiles; ++fd)
		close(fd);
	return (_clink);
	}

/*	get internal device name
 */
LOCAL TINY _getdev(s)
	FAST TEXT *s;
	{
	IMPORT DEV _dev[];
	FAST TEXT *q, *r;
	DEV *p;

	for (p = _dev; p->nm; ++p)
		{
		for (r = s, q = p->nm; *r == *q && *q; ++r, ++q)
			;
		if (*r == *q)
			return (p->devno);
		}
	return (NODEV);
	}

/*	get a CP/M filename
 */
LOCAL BOOL _getname(pf, s)
	FAST FCB *pf;
	FAST TEXT *s;
	{
	FAST COUNT i;

	for (i = 0; *s != '.' && *s; ++s)
		if (i < 8)
			pf->fn[i++] = toupper(*s);
	while (i < 8)
		pf->fn[i++] = ' ';
	if (*s == '.')
		++s;
	else if (*s)
		return (YES);
	for (i = 0; *s && i < 3; ++s)
		pf->ft[i++] = toupper(*s);
	while (i < 3)
		pf->ft[i++] = ' ';
	return (*s);
	}

/*	setup WCB/FCB for open, etc.
 */
FILE _copen(s, mode, rsize)
	FAST TEXT *s;
	COUNT mode;
	BYTES rsize;
	{
	IMPORT BYTES _nfiles;
	IMPORT WCB _wcbs[];
	IMPORT VOID *(*_clink)();
	FAST COUNT i;
	FAST FCB *pf;
	FAST WCB *p;
	COUNT fd;
	TEXT dbuf[5], *t;

	for (fd = 0; fd < _nfiles; ++fd)
		if (!_wcbs[fd].flags)
			break;
	if (_nfiles <= fd)
		return (FAIL);
	p = &_wcbs[fd];
	p->flags = WUSED;
	if (!rsize)
		p->flags =| WCR;
	p->pf = NULL;
	for (t = s, i = 0; *s != ':' && *s; ++s)
		if (i < 4)
			dbuf[i++] = toupper(*s);
	dbuf[i] = '\0';
	if (*s != ':')
		{
		s = t;
		p->dev = 0;
		}
	else if ((p->dev = _getdev(dbuf)) == NODEV)
		{
		close(fd);
		return (FAIL);
		}
	else
		++s;
	if (p->dev < 0 && (p->dev != CON || mode != MOPEN))
		return (fd);
	pf = alloc(sizeof (*pf), NULL);
	p->pf = pf;
	_clrbuf(pf, sizeof (*pf));
	pf->lact = -1;
	if (p->dev < 0)
		return (fd);
	if (_getname(pf, s))
		{
		close(fd);
		return (FAIL);
		}
	pf->et = p->dev;
	pf->curblk = 0;		/* for CP/M */
	if ((mode & ~MWRITE) == MOPEN)
		{
		_svc(CFSIZE, pf);
		if (_svc(COPEN, pf) < 0)
			{
			close(fd);
			return (FAIL);
			}
		_rtol(&pf->lend, &pf->rno);
		}
	else
		{
		i = _svc(CDEL, pf);
		if (mode == MREMOVE)
			{
			close(fd);
			return (i);
			}
		else if (_svc(CMAKE, pf) < 0)
			{
			close(fd);
			return (FAIL);
			}
		pf->lend = 0;
		}
	p->flags =| WOPEN;
	if (!_clink)
		_clink = onexit(&_cclose);
	return (fd);
	}
