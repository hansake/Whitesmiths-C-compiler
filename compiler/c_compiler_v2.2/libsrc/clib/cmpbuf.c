/*	COMPARE TWO BUFFERS
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BOOL cmpbuf(s1, s2, n)
	FAST TEXT *s1, *s2;
	FAST BYTES n;
	{
	for (; 0 < n; --n)
		if (*s1++ != *s2++)
			return (NO);
	return (YES);
	}
