/*	CP/M WRITE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

/*	internal write routine
 */
LOCAL COUNT _write(p, buf, size)
	FAST WCB *p;
	TEXT *buf;
	COUNT size;
	{
	IMPORT COUNT _intch;
	FAST FCB *pf;
	FAST TEXT *s;

	for (s = buf; 0 < size; )
		{
		if ( 0 <= _intch && _svc(CICRDY) & 1 && _svc(CRDCON) == _intch)
			_intr();
		if (p->dev < 0)
			{
			switch (p->dev)
				{
			case CON:
				_svc(CWRCON, *s);
				break;
			case PUN:
				_svc(CWRPUN, *s);
				break;
			case LST:
				_svc(CWRLST, *s);
				break;
			default:
				return (size);
				}
			--size, ++s;
			}
		else
			{
			pf = p->pf;
			if (_rdwr(p, (128 <= size) ? 1 : 2) == FAIL)
				return (FAIL);
			for (; 0 < size && pf->bleft; --size, --pf->bleft)
				{
				*pf->bnext++ = *s++;
				++pf->lseek;
				}
			if (p->flags & WCR && pf->bleft)
				*pf->bnext = CTRLZ;
			p->flags =| WDIRT;
			}
		}
	return (s - buf);
	}

/*	external write routine
 */
COUNT write(fd, buf, size)
	FILE fd;
	TEXT *buf;
	COUNT size;
	{
	FAST TEXT *s, *t;
	FAST WCB *p;

	if (!(p = _ckfd(fd)))
		return (FAIL);
	else if (!(p->flags & WCR))
		return (_write(p, buf, size));
	else
		{
		for (s = buf; 0 < size; )
			{
			for (t = s; 0 < size && *s != '\n'; ++s, --size)
				;
			if (_write(p, t, s - t) != s - t)
				return (FAIL);
			if (0 < size)
				if (_write(p, "\r\n", 2) != 2)
					return (FAIL);
				else
					++s, --size;
			}
		return (s - buf);
		}
	}
