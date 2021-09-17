/*	CP/M CHECK FOR VALID FD
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

WCB *_ckfd(fd)
	FAST FILE fd;
	{
	IMPORT BYTES _nfiles;
	IMPORT WCB _wcbs[];

	if (fd < 0 || _nfiles <= fd || !_wcbs[fd].flags)
		return (NULL);
	else
		return (&_wcbs[fd]);
	}
