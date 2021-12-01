/*  SYSLOG DRIVER UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"
#include "syslog.h"

#define LINESIZE 75

/*	init buffer variables
 */
LOCAL VOID pfdsinit(p)
	FAST FDS *p;
	{
	p->bleft = LINESIZE;
	p->bnext = p->pbuf;
	p->flags &= ~FDIRTY;
	}

/*	open the syslog device
 */
STAT _logopen(p, name, facc)
	FAST FDS *p;
	TEXT *name;
	BYTES facc;
	{
	IMPORT TEXT *_alloc();
	SYSLPT2 *log;

	if (!(p->oflag & O_WRONLY))
		{
		_errno = EBADMODE;
		return (NFAIL);
		}
	p->user = _alloc(sizeof (SYSLPT2), 0);
	_fill(p->user, sizeof (SYSLPT2), '\0');
	log = (SYSLPT2 *) p->user;
	log->msgadr = p->pbuf = _alloc(LINESIZE, NULL);
	log->funb_A	= LGMWSRT|LGMCRT;
	if (p->rsize)
		log->funb_A	|= LGMHIST;
	log->msglen	= LINESIZE;
	log->res2	= 0x80;
	pfdsinit(p);
	return (0);
	}

/*	put a record to syslog
 */
STAT _logput(p, add)
	FAST FDS *p;
	BOOL add;
	{
	FAST SYSLPT2 *log = (SYSLPT2 *) p->user;

	_svc(SSP_SYSLOG, log);
	pfdsinit(p);
	return (0);
	}

/*	close the syslog device
 */
STAT _logclose(p)
	FAST FDS *p;
	{
	FAST SYSLPT2 *log = (SYSLPT2 *) p->user;
	UTINY completion;

	_free(p->pbuf);
	_free(p->user);
	return (0);
	}
