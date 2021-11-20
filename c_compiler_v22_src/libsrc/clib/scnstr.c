/*	SCAN STRING S FOR CHAR C
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES scnstr(s, c)
	FAST TEXT *s;
	TEXT c;
	{
	FAST TEXT *q;
	TEXT ch = c;

	for (q = s; *q; ++q)
		if (*q == ch)
			break;
	return (q - s);
	}
