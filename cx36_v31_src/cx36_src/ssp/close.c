/*  CLOSE A FILE UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"

COUNT _close(fd)
	FD fd;
	{
	FAST FDS *p;
	FAST STAT stat;
	IMPORT FDS *_fdsps[NFILES];

	if (!(p = CHECKFD(fd)))
		{
		_errno = EBADFD;
		return (NFAIL);
		}
	if (--p->nopen)
		{
		_fdsps[fd] = NULL;
		return (0);
		}
	if (p->flags & FDIRTY)
		{
		if (!p->daflag)
			_fill(p->bnext, p->bleft, p->oflag&O_BIN ? '\0' : 0x40);
		(*p->drvr->putrec)(p);
		}
	stat = (*p->drvr->close)(p);
	_free(p);
	_fdsps[fd] = NULL;
	return (stat ? NFAIL : 0);
	}
