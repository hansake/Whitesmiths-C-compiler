/*	FIRST OCCURRENCE IN BUFFER OF ANY CHAR FROM SET
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES inbuf(is, n, p)
	TEXT *is, *p;
	FAST BYTES n;
	{
	FAST TEXT *s, *q;

	for (s = is; 0 < n; --n, ++s)
		{
		q = p;
		FOREVER
			{
			if (*s == *q)
				return (s - is);
			if (!*++q)
				break;
			}
		}
	return (s - is);
	}
