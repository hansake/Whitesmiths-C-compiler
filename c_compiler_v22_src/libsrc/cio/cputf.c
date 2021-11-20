/*	PUT FORMATTED
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

#define DBUF	24
#define DBL YES

LOCAL TEXT *_gnum(q, pnum, ppp)
	FAST TEXT *q, ***ppp;
	FAST COUNT *pnum;
	{
	if (*q == 'n' || *q == 'N')
		{
		++q;
		*pnum = ((COUNT)*(*ppp)++);
		}
	else
		for (*pnum = 0; isdigit(*q); ++q)
			*pnum = *pnum * 10 + *q - '0';
	return (q);
	}

VOID _putf(pfn, arg, f, pp)
	COUNT (*pfn)();
	TEXT *arg;
	FAST TEXT *f;
	BYTES *pp;
	{
	FAST COUNT base;
	FAST TEXT *q;
	BOOL rjust;
	COUNT n, prec, width;
	LONG lo;
	TEXT buf[DBUF], cfill, mod, *s;

	for (; ; f = q + 1)
		{
		for (q = f; *q && *q != '%'; ++q)
			;
		if (f < q)
			(*pfn)(arg, f, q - f);
		if (!*q++)
			return;
		rjust = YES;
		cfill = ' ';
		if (*q == '-')
			{
			rjust = NO;
			cfill = *++q;
			++q;
			}
		else if (*q == '+')
			{
			cfill = *++q;
			++q;
			}
		q = _gnum(q, &width, &pp);
		prec = 0;
		if (*q == '.')
			q = _gnum(q + 1, &prec, &pp);
		if (*q == 'a' || *q == 'h' || *q == 'o' || *q == 'u')
			mod = *q++;
		else
			mod = '\0';
		if (mod == 'h')
			base = 16;
		else if (mod == 'o')
			base = 8;
		else if (mod == 'u')
			base = 10;
		else
			base = -10;
		if (*q == 'x')
			n = 0;
		else if (*q == 'b')
			{
			s = *pp++;
			n = *pp++;
			if (prec)
				n = min(n, prec);
			}
		else if (*q == 'p')
			{
			s = *pp++;
			n = lenstr(s);
			if (prec)
				n = min(n, prec);
			}
#if DBL
		else if (*q == 'f')
			{
			s = buf;
			n = dtof(buf, *(DOUBLE *)pp, (DBUF-2) - prec, prec);
			pp = (DOUBLE *)pp + 1;
			}
		else if (*q == 'd')
			{
			s = buf;
			n = dtoe(buf, *(DOUBLE *)pp, 1, min(DBUF-7, prec));
			pp = (DOUBLE *)pp + 1;
			}
#endif
		else if (*q == 'c' || *q == 's' || *q == 'i' || *q == 'l' || mod &&
			*--q)	/* ! */
			if (*q == 'l' || *q != 'c' && *q != 's' && sizeof (int) == sizeof (LONG))
				if (mod == 'a')
					{
					for (lo = *(LONG *)pp, n = 4; 0 <= --n; lo =>> 8)
						buf[n] = lo;
					s = buf;
					n = 4;
					pp = (LONG *)pp + 1;
					}
				else
					{
					s = buf;
					n = ltob(buf, *(LONG *)pp, base);
					pp = (LONG *)pp + 1;
					}
			else
				{
				s = buf;
				n = *pp++;
				if (mod == 'a')
					{
					if (*q == 'c')
						{
						buf[0] = n;
						n = 1;
						}
					else
						{
						buf[0] = n >> 8;
						buf[1] = n;
						n = 2;
						}
					}
				else
					{
					if (base < 0)
						;
					else if (*q == 'c')
						n =& BYTMASK;
					else
						n =& 0177777;
					n = stob(buf, n, base);
					}
				}
		else
			{
			s = q;
			n = 1;
			}
		if (rjust)
			for (; n < width; --width)
				(*pfn)(arg, &cfill, 1);
		if (0 < n)
			(*pfn)(arg, s, n);
		if (!rjust)
			for (; n < width; --width)
				(*pfn)(arg, &cfill, 1);
		}
	}
