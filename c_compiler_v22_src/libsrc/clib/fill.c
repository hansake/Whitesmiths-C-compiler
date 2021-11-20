/*	FLOOD BUFFER WITH FILL CHARACTER
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES fill(s, n, c)
	FAST TEXT *s;
	BYTES n;
	TEXT c;
	{
	FAST BYTES i;

	for (i = n; 0 < i; --i)
		*s++ = c;
	return (n);
	}
