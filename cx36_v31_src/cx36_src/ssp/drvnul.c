/*  NULL I/O DRIVER UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"

#define NULRECSIZE 512

/*  close NULL file
 */
STAT _nulclose(p)
	FAST FDS *p;
	{
	_free(p->user, NULL);
	return (0);
	}

/*  get record from NULL file with immediate end-of-file
 */
STAT _nulget(p, recno)
	FDS *p;
	LONG recno;
	{
	return (1);
	}

/*  open NULL file
 */
STAT _nulopen(p, name, facc)
	FAST FDS *p;
	TEXT *name;
	BYTES facc;
	{
	IMPORT TEXT *_alloc();

	p->user = _alloc(NULRECSIZE, NULL);
	p->bleft = NULRECSIZE;
	p->bnext = p->user;
	return (0);
	}

/*  put record to NULL file
 */
STAT _nulput(p, add)
	FDS *p;
	BOOL add;
	{
	p->bleft = NULRECSIZE;
	p->bnext = p->user;
	p->flags &= ~FDIRTY;
	return (0);
	}
