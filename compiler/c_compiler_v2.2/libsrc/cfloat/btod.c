/*	BUFFER TO DOUBLE CONVERSION
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES btod(is, n, pdnum)
	TEXT *is;
	FAST BYTES n;
	DOUBLE *pdnum;
	{
	IMPORT DOUBLE _dtens[], _dzero, dtento();
	FAST COUNT exp;
	FAST TEXT *s;
	BOOL minus;
	COUNT x;
	DOUBLE d;

	s = is;
	minus = NO;
	while (0 < n && iswhite(*s))
		++s, --n;
	if (0 < n)
		if (*s == '-')
			{
			++s, --n;
			minus = YES;
			}
		else if (*s == '+')
			++s, --n;
	for (d = _dzero; 0 < n && isdigit(*s); ++s, --n)
		d = d * _dtens[1] + (*s - '0');
	exp = 0;
	if (0 < n && *s == '.')
		for (++s, --n; 0 < n && isdigit(*s); ++s, --n, --exp)
			d = d * _dtens[1] + (*s - '0');
	if (0 < n && tolower(*s) == 'e')
		{
		++s, --n;
		s =+ btos(s, n, &x, 10);
		exp =+ x;
		}
	d = dtento(d, exp);
	*pdnum = (minus ? -d : d);
	return (s - is);
	}
