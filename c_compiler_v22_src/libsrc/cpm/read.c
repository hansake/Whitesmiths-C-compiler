/*	CP/M READ
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

COUNT read(fd, buf, size)
	FILE fd;
	TEXT *buf;
	COUNT size;
	{
	IMPORT COUNT _intch;
	FAST FCB *pf;
	FAST TEXT *s;
	FAST WCB *p;

	if (!(p = _ckfd(fd)))
		return (FAIL);
	pf = p->pf;
	for (s = buf; 0 < size; )
		if (p->dev < 0)
			{
			switch (p->dev)
				{
			case CON:
				if (!pf)
					return (0);
				if (pf->bleft == 0)
					{
					pf->buf[0] = 125;
					_svc(CRDBUF, pf->buf);
					_svc(CWRCON, '\n');
					pf->bleft = pf->buf[1] + 1;
					pf->bnext = pf->buf + 2;
					pf->bnext[pf->buf[1]] = '\n';
					if (0 <= _intch && *pf->bnext == _intch)
						_intr();
					}
				*s = *pf->bnext++, --pf->bleft;
				break;
			case RDR:
				*s = _svc(CRDRDR, 0);
				break;
			default:
				return (0);
				}
			if (!(p->flags & WCR))
				++s, --size;
			else if (*s == '\r' || *s == '\0')
				;
			else if (*s == CTRLZ)
				break;
			else if (*s == '\n')
				return (s - buf + 1);
			else
				++s, --size;
			}
		else
			{
			switch (_rdwr(p, 0))
				{
			case FAIL:
				return (FAIL);
			case EOF:
				return (s - buf);
				}
			for (; 0 < size && pf->bleft;
				--pf->bleft, ++pf->bnext, ++pf->lseek)
				{
				*s = *pf->bnext;
				if (!(p->flags & WCR))
					++s, --size;
				else switch (*s)
					{
				case CTRLZ:
					return (s - buf);
				case '\0':
				case '\r':
					break;
				default:
					++s, --size;
					}
				}
			}
	return (s - buf);
	}
