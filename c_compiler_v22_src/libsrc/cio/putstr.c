/*	WRITE STRINGS TO FD
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID putstr(fd, s)
	FILE fd;
	TEXT *s;
	{
	FAST TEXT **p;

	for (p = &s; *p; ++p)
		fwrite(fd, *p, lenstr(*p));
	}
