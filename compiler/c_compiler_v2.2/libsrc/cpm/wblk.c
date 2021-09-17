/*	CP/M INTERNAL BLOCK WRITE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

/*	setup for read/write
 */
COUNT _rdwr(p, rdfl)
	FAST WCB *p;
	COUNT rdfl;		/* 0 => read, 1 => no preread, 2 => write */
	{
	FAST FCB *pf;
	FAST COUNT i;
	LONG lact;

	pf = p->pf;
	lact = pf->lseek >> 7;
	i = (COUNT) pf->lseek & 0177;
	pf->bleft = 128 - i;
	pf->bnext = pf->buf + i;
	if (lact == pf->lact)
		return (0);
	if (_wblk(p) < 0)
		return (FAIL);
	_ltor(&pf->rno, &lact);
	if (pf->lend <= lact)
		{
		if (rdfl == 0)
			return (EOF);
		pf->lact = lact;
		pf->lend = lact + 1;
		if (rdfl == 2 || i)
			_clrbuf(pf->buf, 128);
		return (0);
		}
	pf->lact = lact;
	_svc(CSETAD, pf->buf);
	switch (_svc(CRREAD, pf))
		{
	case 2:		/* for DOS.  CP/M should fail on 3 */
		return (FAIL);
	case 6:
		return (EOF);
	case 1:
	case 4:
		_clrbuf(pf->buf, 128);
	default:
		return (0);
		}
	}

/*	make sure block/extent is written out
 */
BOOL _wblk(p)
	FAST WCB *p;
	{
	FAST FCB *pf;
	FAST COUNT i;

	if (p->flags & WDIRT)
		{
		pf = p->pf;
		_ltor(&pf->rno, &pf->lact);
		_svc(CSETAD, pf->buf);
		i = _svc(CRWRITE, pf);
		if (i)
			return (FAIL);
		p->flags =& ~WDIRT;
		}
	return (0);
	}

/*	clear a buffer
 */
VOID _clrbuf(s, n)
	FAST TEXT *s;
	FAST COUNT n;
	{
	for ( ; 0 <= --n; ++s)
		*s = '\0';
	}
