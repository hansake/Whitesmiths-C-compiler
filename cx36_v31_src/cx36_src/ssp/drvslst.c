/*  SYSLIST DRIVER UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"
#include "syslist.h"

#define LINESIZE 132

/*	init buffer variables
 */
LOCAL VOID pfdsinit(p)
	FAST FDS *p;
	{
	p->bleft = LINESIZE;
	p->bnext = p->pbuf;
	p->flags &= ~FDIRTY;
	}

/*	open the syslist device
 */
STAT _sliopen(p, name, facc)
	FAST FDS *p;
	TEXT *name;
	BYTES facc;
	{
	IMPORT TEXT *_alloc();
	SYSLIST *sli;

	if (!(p->oflag & O_WRONLY))
		{
		_errno = EBADMODE;
		return (NFAIL);
		}
	p->user = _alloc(sizeof (SYSLIST), 0);
	_fill(p->user, sizeof (SYSLIST), '\0');
	sli = (SYSLIST *) p->user;
	sli->sladdr = p->pbuf = _alloc(LINESIZE, NULL);
	sli->slapge = 0x01;
	sli->sllen = LINESIZE;
	sli->res1 = 0x80;
	pfdsinit(p);
	return (0);
	}

/*	put a record to syslist
 */
STAT _sliput(p, add)
	FAST FDS *p;
	BOOL add;
	{
	FAST SYSLIST *sli = (SYSLIST *) p->user;

	_svc(SSP_SYSLST, sli);
	pfdsinit(p);
	return (0);
	}

/*	close the syslist device
 */
STAT _sliclose(p)
	FAST FDS *p;
	{
	FAST SYSLIST *sli = (SYSLIST *) p->user;
	UTINY completion;

	_free(p->pbuf);
	_free(p->user);

	return (0);
	}
