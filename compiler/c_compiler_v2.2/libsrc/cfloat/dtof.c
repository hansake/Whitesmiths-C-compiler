/*	CONVERT DOUBLE TO STRING IN F-FORMAT
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include	<std.h>

#define	MAXPREC	17

BYTES dtof(is, d, p, g)
	TEXT *is;
	DOUBLE d;
	COUNT p;
	FAST COUNT g;
	{
	FAST TEXT *q, *s;
	COUNT exp, keep;
	TEXT buf[MAXPREC + 1];

	s = is;
	if (d < 0)
		{
		*s++ = '-';
		d = -d;
		}
	q = buf;
	exp = _norm(q, d, MAXPREC + 1);
	keep = exp + g;
	if (MAXPREC < keep)
		keep = MAXPREC;
	exp =+ _round(buf, MAXPREC + 1, keep);
	if (p < exp)
		{
		q =+ exp - p;
		exp = p;
		}
	if (exp <= 0)
		*s++ = '0';
	else
		for (; 0 < exp; --exp)
			*s++ = (q - buf < MAXPREC) ? *q++ : '0';
	if (0 < g)
		*s++ = '.';
	for (; exp < 0 && 0 < g; ++exp, --g)
		*s++ = '0';
	for (; 0 < g; --g)
		*s++ = (q - buf < MAXPREC) ? *q++ : '0';
	return (s - is);
	}
