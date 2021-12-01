/*  SYSIN DRIVER UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"
#include "sysin.h"

#define LINESIZE 120

/*	init buffer variables
 */
LOCAL VOID pfdsinit(p)
	FAST FDS *p;
	{
	p->bleft = LINESIZE;
	p->bnext = p->pbuf;
	p->flags &= ~FDIRTY;
	}

/*	open the sysin device
 */
STAT _sinopen(p, name, facc)
	FAST FDS *p;
	TEXT *name;
	BYTES facc;
	{
	IMPORT TEXT *_alloc();
	SYSIN *sin;

	if ((p->oflag & 3) != O_RDONLY)
		{
		_errno = EBADMODE;
		return (NFAIL);
		}
	p->user = _alloc(sizeof (SYSIN), 0);
	_fill(p->user, sizeof (SYSIN), '\0');
	sin = (SYSIN *) p->user;
	sin->siaddr = p->pbuf = _alloc(LINESIZE, NULL);
	pfdsinit(p);
	return (0);
	}

/*	get a record from sysin
 */
STAT _singet(p, recno)
	FAST FDS *p;
	LONG recno;
	{
	FAST SYSIN *sin = (SYSIN *) p->user;

	pfdsinit(p);
	sin->siopt1 = SINM120B;	/* 120 byte sysin buffer */
	sin->res1 = 0x80;
	_svc(SSP_SYSIN, sin);
	if ((sin->siopt1 == SINMEOF) ||
		((sin->siaddr[0] == '/') && (sin->siaddr[1] == '*')))
		return (1);	/* indicate end of file */
	return (0);
	}

/*	close the sysin device
 */
STAT _sinclose(p)
	FAST FDS *p;
	{
	FAST SYSIN *sin = (SYSIN *) p->user;
	UTINY completion;

	_free(p->pbuf);
	_free(p->user);

	return (0);
	}
