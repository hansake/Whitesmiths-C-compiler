/*	FIRST OCCURRENCE IN STRING OF ANY CHAR FROM SET
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES instr(is, p)
	TEXT *is, *p;
	{
	FAST TEXT *s, *q;

	for (s = is; *s; s++)
		for (q = p; *q; q++)
			if (*s == *q)
				return (s - is);
	return (s - is);
	}
