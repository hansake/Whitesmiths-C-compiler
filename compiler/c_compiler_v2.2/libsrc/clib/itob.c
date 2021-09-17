/*	CONVERT INTEGER TO BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES itob(is, n, base)
	FAST TEXT *is;
	COUNT n;
	COUNT base;
	{
	FAST BYTES u;
	FAST TEXT *s;

	s = is;
	if (n < 0 && base <= 0)
		{
		u = -n;
		*s++ = '-';
		}
	else
		u = n;
	if (base == 0)
		base = 10;
	else if (base < 0)
		base = -base;
	if (base <= u)
		s =+ itob(s, u / base, base);
	*s = (u % base) + '0';
	if ('9' < *s)
		*s =+ ('a' - ('9' + 1));
	return (s - is + 1);
	}
