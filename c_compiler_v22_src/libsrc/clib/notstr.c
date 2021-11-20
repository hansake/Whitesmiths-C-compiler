/*	FIRST OCCURRENCE IN STRING OF ANY CHAR NOT FROM SET
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES notstr(is, p)
	TEXT *is, *p;
	{
	FAST TEXT *s, *q;
	BOOL flag;

	for (s = is; *s; ++s)
		{
		for (flag = NO, q = p; *q; ++q)
			if (*s == *q)
				{
				flag = YES;
				break;
				}
		if (flag == NO)
			break;
		}
	return (s - is);
	}
