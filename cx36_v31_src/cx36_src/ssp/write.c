/*  WRITE CHARACTERS TO A FILE UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"

ARGINT _write(fd, buf, size)
	FD fd;
	TEXT *buf;
	ARGINT size;	
	{
#ifndef _EBCDIC
	IMPORT RDONLY TEXT _atoe[];
#endif
	IMPORT FDS *_fdsps[];
	COUNT len;
	FAST FDS *p;
	FAST TEXT *b;
	LONG off, recno, rno;
	TEXT c;
	TEXT stack;

	if (!(p = CHECKFD(fd)))
		{
		_errno = EBADFD;
		return (NFAIL);
		}
	if (!(p->flags & FOPENW))
		{
		_errno = EBADMODE;
		return (NFAIL);
		}
	if (!(p->daflag))
		{
		if (!(p->oflag&O_BIN))
			{
			/* process text-file
			 */
			for (b = buf; 0 < size; ++b, --size)
				{
				if (*b < ' ')
					{
					_fill(p->bnext, p->bleft, 0x40); /* EBCDIC space */
					if (*b == '\n')
						{
						if ((*p->drvr->putrec)(p, YES))
							return (NFAIL);
						p->ctl = CTLNEWL;
						}
					else if (*b == '\f')
						{
						if ((*p->drvr->putrec)(p, YES))
							return (NFAIL);
						p->ctl = CTLNEWP;
						}
					else if (*b == '\r')
						{
						if ((*p->drvr->putrec)(p, YES))
							return (NFAIL);
						p->ctl = CTLSAMEL;
						}
					else
						goto my_first_C_label;
					}
				else
					{
					my_first_C_label:
					if (!p->bleft)
						{
						if ((*p->drvr->putrec)(p, NO))
							return (NFAIL);
						p->ctl = CTLNONL;
						}
#ifndef _EBCDIC
#ifndef _UPCASE
					*p->bnext++ = _atoe[*b];
#else
					*p->bnext++ = _atoe[_toupper(*b)];
#endif
#else
					*p->bnext++ = *b;
#endif
					p->flags |= FDIRTY;
					p->bleft--;
					}
				}
			}
		else
			{
			/* binary file
			 */
			for (b = buf; 0 < size; )
				{
				if (!p->bleft && (*p->drvr->putrec)(p, NO))
					return (NFAIL);
				len = min(size, p->bleft);
				_cpybuf(p->bnext, b, len);
				p->bleft -= len;
				p->bnext += len;
				b += len;
				size -= len;
				if (len)
					p->flags |= FDIRTY;
				}
			}
		if (p->drvr->drflags & DIMMED && p->flags & FDIRTY)
			if ((*p->drvr->putrec)(p, NO))
				return (NFAIL);
		}
	else
		{
		for (b = buf; 0 < size; )
			{
			recno = p->lseek / p->rlen;
			if (recno != p->rcurr)
				{
				if (p->flags & FDIRTY)	/* used to be (FDIRTY|FMEMFIL) */
					if ((*p->drvr->putrec)(p))
						return (NFAIL);
				if (p->rend <= recno)
					{
					p->rcurr = p->rend;
					for (rno = p->rcurr; rno < recno; ++rno)
						{
						_fill(p->pbuf, p->rlen, '\0');
						p->rcurr = rno;
						(*p->drvr->putrec)(p);
						}
					_fill(p->pbuf, p->rlen, '\0');
					if (p->rcurr != -1)
						p->rcurr = recno;
					}
				if (recno != p->rcurr)
					{
					if ((*p->drvr->getrec)(p, recno))
						return (NFAIL);
					}
				}
			off = p->lseek % p->rlen;
			p->bleft = p->rlen - off;
			p->bnext = p->pbuf + off;
			len = min(size, p->bleft);
			_cpybuf(p->bnext, b, len);
			p->lseek += len;
			b += len;
			size -= len;
			if (p->fsize < p->lseek)
				p->fsize = p->lseek;
			if (len)
				p->flags |= FDIRTY;
			}
		}
	return (b - buf);
	}
