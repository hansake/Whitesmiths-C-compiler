/*	CONVERT DOUBLE TO FRACTION AND RETURN EXPONENT
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT _norm(s, d, p)
	FAST TEXT *s;
	DOUBLE d;
	BYTES p;
	{
	IMPORT COUNT _ntens;
	IMPORT DOUBLE _dtens[];
	FAST COUNT exp, i;
	COUNT dig;
	DOUBLE nd;

	exp = 0;
	if (d < 0)
		d = -d;
	if (!d)
		;
	else if (_dtens[0] <= d)
		{
		for (i = _ntens - 1; 0 < i; --i)
			if (_dtens[i] <= d)
				{
				d = d / _dtens[i];
				exp =+ (1 << (i - 1));
				}
		if (_dtens[0] <= d)			/* 1.0 <= d && d < 10.0 */
			{
			d = d / _dtens[1];
			++exp;
			}
		}
	else if (d * _dtens[1] < _dtens[0])
		for (i = _ntens - 1; 0 < i; --i)	/* d < 0.1 */
			if ((nd = _dtens[i] * d) < _dtens[0])
				{
				d = nd;
				exp =- (1 << (i - 1));
				}
	for (i = 0; i < p && d; ++i)	/* d == 0 || 0.1 <= d && d < 1.0 */
		{
		nd = d * _dtens[1];
		dig = nd;
		d = nd - dig;
		*s++ = dig + '0';
		}
	for (; i < p; ++i)
		*s++ = '0';
	return (exp);
	}
