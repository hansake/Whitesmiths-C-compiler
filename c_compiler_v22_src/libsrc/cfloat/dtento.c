/* RAISE DOUBLE TO A POWER OF 10
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

DOUBLE dtento(d, exp)
	DOUBLE d;
	FAST COUNT exp;
	{
	IMPORT COUNT _ntens;
	IMPORT DOUBLE _dtens[];
	FAST BYTES u;
	FAST COUNT i;

	if (exp < 0)
		{
		for (u = -exp, i = 1; u && i < _ntens; u =>> 1, ++i)
			if (u & 1)
				d = d / _dtens[i];
		for (; 0 < u; --u)
			{
			d = d / _dtens[_ntens - 1];
			d = d / _dtens[_ntens - 1];
			}
		}
	else if (0 < exp)
		{
		for (u = exp, i = 1; u && i < _ntens; u =>> 1, ++i)
			if (u & 1)
				d = d * _dtens[i];
		for (; 0 < u; --u)
			{
			d = d * _dtens[_ntens - 1];
			d = d * _dtens[_ntens - 1];
			}
		}
	return (d);
	}
