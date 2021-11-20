/*	LSEEK (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

LONG lsek7(fd, offset, whence)
	FILE fd;
	LONG offset;
	COUNT whence;
	{
	if (lseek(fd, offset, whence) <0)
		return (-1);
	else
		return (offset);	/* BUG - not true "tell" unless whence == 0 */
	}
