/*	ROUND OFF A FRACTION
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include	<std.h>

COUNT _round(s, n, p)
	FAST TEXT *s;
	COUNT n, p;
	{
	FAST COUNT i;

	if (p < 0 || n <= p || s[p] < '5')
		;
	else
		{
		for (i = p - 1; 0 <= i; --i)
			if (s[i] == '9')
				s[i] = '0';
			else
				{
				++s[i];
				break;
				}
		if (i < 0)
			{
			s[0] = '1';
			for (i = p; 0 < i; --i)
				s[i] = '0';
			return (1);
			}
		}
	return (0);
	}
