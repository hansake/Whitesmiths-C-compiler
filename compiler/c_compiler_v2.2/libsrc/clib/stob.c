/*	CONVERT SHORT TO BUFFER
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES stob(is, n, base)
	FAST TEXT *is;
	COUNT n;
	COUNT base;
	{
	FAST BYTES u;
	FAST TEXT *s;

	s = is;
	if (n < 0 && base <= 0)
		{
		n = -n;
		*s++ = '-';
		}
	u = n & 0177777;
	if (base == 0)
		base = 10;
	else if (base < 0)
		base = -base;
	if (base <= u)
		s =+ stob(s, u / base, base);
	*s = (u % base) + '0';
	if ('9' < *s)
		*s =+ ('a' - ('9' + 1));
	return (s - is + 1);
	}
