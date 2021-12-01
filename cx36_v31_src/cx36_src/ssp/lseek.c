/*  SET FILE READ/WRITE POINTER UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"

LONG _lseek(fd, offset, sense)
	FD fd;
	LONG offset;
	COUNT sense;
	{
	IMPORT FDS *_fdsps[];
	FAST FDS *p;

	if (!(p = CHECKFD(fd)))
		{
		_errno = EBADFD;
		return (NFAIL);
		}
	if (!p->daflag)
		{
		_errno = ESEEK;
		return (NFAIL);
		}
	switch (sense)
		{
	case 0:
		p->lseek = offset;
		break;
	case 1:
		p->lseek += offset;
		break;
	case 2:
		p->lseek = p->fsize + offset;
		break;
	default:
		_errno = ESEEK;
		return (NFAIL);
		}
	return (p->lseek);
	}
