/*	CREATE WITH FILE I/O
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

FIO *fcreate(pf, fname, mode)
	FIO *pf;
	TEXT *fname;
	COUNT mode;
	{
	FAST FILE fd;

	if (0 <= (fd = create(fname, (mode == BWRITE) ? WRITE : mode, 0)))
		return (finit(pf, fd, mode));
	else
		return (NULL);
	}
