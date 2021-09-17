/*	SCAN BUFFER S FOR CHAR C
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES scnbuf(s, n, c)
	TEXT *s;
	FAST BYTES n;
	COUNT c;
	{
	FAST TEXT *q;
	TEXT ch = c;

	for (q = s; 0 < n; --n, ++q)
		if (*q == ch)
			break;
	return (q - s);
	}
