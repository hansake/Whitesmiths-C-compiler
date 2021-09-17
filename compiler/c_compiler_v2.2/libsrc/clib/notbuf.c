/*	FIRST OCCURRENCE IN BUFFER OF ANY CHAR NOT FROM SET
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES notbuf(is, n, p)
	TEXT *is, *p;
	FAST BYTES n;
	{
	FAST TEXT *s, *q;
	BOOL flag;

	for (s = is; 0 < n; --n, ++s)
		{
		for (flag = NO, q = p; ; )
			{
			if (*s == *q)
				{
				flag = YES;
				break;
				}
			else if (!*++q)
				break;
			}
		if (flag == NO)
			break;
		}
	return (s - is);
	}
