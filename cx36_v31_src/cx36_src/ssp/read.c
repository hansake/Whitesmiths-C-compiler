/*  READ CHARACTERS FROM A FILE UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"

ARGINT _read(fd, buf, size)
	FD fd;
	TEXT *buf;
	ARGINT size;
	{
#ifndef _EBCDIC
	IMPORT RDONLY TEXT _etoa[];
#endif
	IMPORT FDS *_fdsps[];

	BYTES off;
	COUNT len;
	COUNT stat;
	FAST FDS *p;
	FAST TEXT *b;
	LONG recno;
	LONG rleft;

	if (!(p = CHECKFD(fd)))
		{
		_errno = EBADFD;
		return (NFAIL);
		}
	if (!(p->flags & FOPENR))
		{
		_errno = EBADMODE;
		return (NFAIL);
		}
	if (p->flags & FEOF)
		return (0);

	if (!(p->daflag))
		{
		for (b = buf; 0 < size; )
			{
			if (p->flags & FMRD)
				{
				stat = (*p->drvr->getrec)(p, 0L);
				if (stat < 0)
					return (NFAIL);
				else if (stat == 1)
					{
					if (p->drvr->drflags & DPERMEOF)
						p->flags |= FEOF;
					break;
					}
#ifndef _EBCDIC
				_xlate(_etoa, p->bnext, p->bleft);
#endif
				if (!(p->drvr->drflags & DBLANKS))
					for (; p->bleft && p->bnext[p->bleft-1] == ' ';
						--p->bleft)
						;
				p->flags &= ~FMRD;
				}
			if (!p->bleft)
				{
				if (!(p->oflag&O_BIN))
					{
					*b++ = '\n';
					p->flags |= FMRD;
					break;
					}
				else
					{
					if ((stat = (*p->drvr->getrec)(p, 0L)) < 0)
						return (NFAIL);
					if (stat == 1)
						{
						p->flags |= FEOF;
						break;
						}
					}
				}
			len = min(size, p->bleft);
			_cpybuf(b, p->bnext, len);
			p->bnext += len;
			p->bleft -= len;
			b += len;
			size -= len;
			if (p->drvr->drflags & DIMMED)
				size = 0;
			}
		}
	else
		{
		for (b = buf; 0 < size; )
			{
			recno = p->lseek / p->rlen;
			if (recno != p->rcurr)
				{
				if (p->flags & FDIRTY)
					if ((*p->drvr->putrec)(p))
						return (NFAIL);
				if (p->rend <= recno)
					break;
				else if ((stat = (*p->drvr->getrec)(p, recno)) < 0)
					return (NFAIL);
				else if (stat == 1)
					{
					p->rend = p->rcurr + 1;
					break;
					}
				}
			off = p->lseek % p->rlen;
			p->bleft = p->rlen - off;
			p->bnext = p->pbuf + off;
			len = min(size, p->bleft);
			rleft = p->fsize - p->lseek;
			if (!rleft)
				break;
			len = min(len, rleft);
			_cpybuf(b, p->bnext, len);
			b += len;
			size -= len;
			p->lseek += len;
			}
		}
	return (b - buf);
	}
