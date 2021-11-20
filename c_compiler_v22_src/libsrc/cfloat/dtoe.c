/*	CONVERT DOUBLE TO STRING IN E-FORMAT
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include	<std.h>

#define MAXPREC	17

BYTES dtoe(is, d, p, g)
	TEXT *is;
	DOUBLE d;
	BYTES p, g;
	{
	IMPORT COUNT _ntens;
	FAST TEXT *s;
	FAST COUNT i, j;
	COUNT exp, rnd;
	TEXT buf[MAXPREC + 1];

	s = is;
	if (d < 0)
		{
		d = -d;
		*s++ = '-';
		}
	rnd = p + g;
	if (MAXPREC < rnd)
		rnd = MAXPREC;
	exp = _norm(buf, d, rnd + 1);
	exp =+ _round(buf, rnd + 1, rnd);
	for (i = 0; i < rnd && i < p; ++i, --exp)
		*s++ = buf[i];
	for (; i < p; ++i, --exp)
		*s++ = '0';
	if (0 < g)
		{
		*s++ = '.';
		for (j = 0; j < g && i < rnd; ++i, ++j)
			*s++ = buf[i];
		for (; j < g; ++j)
			*s++ = '0';
		}
	*s++ = 'e';
	if (!d)
		exp = 0;
	if (exp < 0)
		{
		*s++ = '-';
		exp = -exp;
		}
	else
		*s++ = '+';
	if (100 <= exp)
		{
		*s++ = exp / 100 + '0';
		exp =% 100;
		}
	*s++ = exp / 10 + '0';
	*s++ = exp % 10 + '0';
	return (s - is);
	}
