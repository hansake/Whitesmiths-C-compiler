/*	BUY A BUFFER AND COPY INTO IT
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *buybuf(s, n)
	FAST TEXT *s;
	FAST BYTES n;
	{
	FAST TEXT *q, *start;

	start = alloc(n, NULL);
	for (q = start; 0 < n; --n)
		*q++ = *s++;
	return (start);
	}
