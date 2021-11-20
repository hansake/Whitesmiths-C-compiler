/*	CONVERT LONG TO BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES ltob(is, ln, base)
	FAST TEXT *is;
	LONG ln;
	COUNT base;
	{
	FAST TEXT *s;
	LONG lb;

	s = is;
	if (ln < 0 && base <= 0)
		{
		ln = -ln;
		*s++ = '-';
		}
	if (base == 0)
		base = 10;
	else if (base < 0)
		base = -base;
	lb = base;
	if (ln < 0 || lb <= ln)
		s =+ ltob(s, (ULONG)ln / lb, base);
	*s = (ULONG)ln % lb + '0';
	if ('9' < *s)
		*s =+ ('a' - ('9' + 1));
	return (s - is + 1);
	}
