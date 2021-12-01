/*  OPEN A FILE UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"

LOCAL VOID *(*_clink)() {NULL};

/*  close all files on exit
 */
LOCAL VOID *_cclose()
	{
	IMPORT VOID *(*_clink)();
	FAST COUNT fd;

	for (fd = 0; fd < NFILES; ++fd)
		_close(fd);
	return (_clink);
	}

/*	open a file
 */
FD _open(fname, oflag, mode, xparm)
	TEXT *fname;	/* filename { <drv>: | [<drv>:]<file> } */
	BITS oflag;		/* open flag */
	BITS mode;		/* dummy - not meaningful under system/36 */
	TEXT *xparm;	/* extra parameter - usually passed by fopen */
	{
	IMPORT DRVR *_getdrv();
	IMPORT TEXT *_alloc();
	IMPORT VOID *_cclose();
	IMPORT VOID *(*_clink)();

	BYTES rsize;
	BYTES facc;
	IMPORT FDS *_fdsps[NFILES];
	DRVR *drvr;
	FAST FD fd;
	FAST FDS *p;
	STAT stat;

	if (!fname)
		{
		_errno = EBADNM;
		return (NFAIL);
		}
	if (!(oflag & O_BIN) && (oflag & O_RDWR))
		{
		_errno = EBADMODE;
		return (NFAIL);
		}
	if (oflag & O_EXCL)
		{
		_errno = EOEXCL;
		return (NFAIL);
		}
	facc = (oflag & O_TRUNC ? FACREATE : FAOPEN);
	if (oflag & O_XTYPE && xparm)
		rsize = _evalrecl (xparm);
	if (!(drvr = _getdrv(fname)))
		return (NFAIL);
	for (fd = 0; fd < NFILES; ++fd)
		{
		if (!(_fdsps[fd]))
			{
			p = (FDS *) _alloc(sizeof (FDS), NULL);
			p->drvr = drvr;
			p->daflag = drvr->drflags & DDA; /* Uh? What if da & seq is ok? */
			p->nopen = 1;
			p->flags = 0;
			p->bleft = 0;
			p->bnext = NULL;
			p->rcurr = -1;
			p->lseek = 0;
			p->rend = (facc == FAOPEN) ? 0x7fffffff : 0;
			p->fsize = (facc == FAOPEN) ? 0x7fffffff : 0;
			p->rlen = 0;
			p->nopen = 1;
			p->ctl = CTLNEWP;
			p->rsize = rsize;
			p->oflag = oflag;

			switch (oflag & (O_RDONLY|O_WRONLY|O_RDWR))
				{
			case O_RDONLY:
				p->flags |= (oflag & O_BIN ? FOPENR : (FOPENR|FMRD));
				break;
			case O_WRONLY:
				p->flags |= FOPENW;
				break;
			default:
				p->flags |= (FOPENR|FOPENW);
				break;
				}

			if (!_clink)
				_clink = (VOID *) _onexit(&_cclose);

			_fdsps[fd] = p;

			if ((*p->drvr->open)(p, fname, facc))
				{
				_free(p, NULL);
				_fdsps[fd] = NULL;
				return (NFAIL);
				}
			else
				return (fd);
			}
		}
	_errno = EMFILE;
	return (NFAIL);
	}
