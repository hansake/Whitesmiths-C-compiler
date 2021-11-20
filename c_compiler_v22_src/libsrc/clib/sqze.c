/*	SQUEEZE SPECIFIED CHARACTER OUT OF BUFFER
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES squeeze(s, n, c)
	TEXT c, *s;
	FAST BYTES n;
	{
	FAST TEXT *p, *q;
	TEXT ch = c;

	for (p = q = s; 0 < n; --n, ++p)
		if (*p != ch)
			*q++ = *p;
	return (q - s);
	}
