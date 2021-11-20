/*	CP/M CLOSE A FILE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

COUNT close(fd)
	FILE fd;
	{
	FAST WCB *p;

	if (!(p = _ckfd(fd)))
		return (FAIL);
	else if (p->pf && !(p->flags & WSTD))
		{
		if (p->flags & WOPEN)
			{
			_wblk(p);
			_svc(CCLOSE, p->pf);
			}
		p->pf = free(p->pf, NULL);
		}
	p->flags = 0;
	return (fd);
	}
