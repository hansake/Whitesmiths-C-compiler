/*	CP/M SEEK
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

COUNT lseek(fd, loff, sense)
	FILE fd;
	LONG loff;
	COUNT sense;
	{
	FAST COUNT bufseg;
	FAST FCB *pf;
	FAST WCB *p;
	COUNT rcount;

	if (!(p = _ckfd(fd)) || p->dev < 0)
		return (FAIL);
	pf = p->pf;
	if (sense == 0)
		pf->lseek = loff;
	else if (sense == 1)
		pf->lseek =+ loff;
	else
		pf->lseek = (pf->lend << 7) + loff;
	return (fd);
	}
