/*	COPY BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES cpybuf(s1, s2, an)
	FAST TEXT *s1, *s2;
	BYTES an;
	{
	FAST BYTES n;

	n = an;
	for (; 0 < n; --n)
		*s1++ = *s2++;
	return (an);
	}
