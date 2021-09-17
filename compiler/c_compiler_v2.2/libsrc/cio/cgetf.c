/*	GET FORMATTED
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

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

COUNT _getf(pfn, arg, f, pp)
	COUNT (*pfn)();
	TEXT *f;
	TEXT *arg, **pp;
	{
	FAST TEXT *q, *r, *s;
	BOOL rjust;
	COUNT base, nargs, prec, size, width;
	COUNT inum, n, osize;
	DOUBLE dble;
	LONG lnum;
	TEXT *or, *t;
	TEXT cfill, mod, rbuf[BUFSIZE];

	if (!*f)
		return (0);
	else if ((size = (*pfn)(arg, r = rbuf, BUFSIZE)) <= 0)
		return (EOF);
	for (nargs = 0; ; f = q + 1)
		{
		for (q = f; *q && *q != '%' && *q != '\n'; ++q)
			if (*q != *r || size <= 0)
				return (nargs);
			else
				{
				++r;
				--size;
				}
		if (!*q)
			return (nargs);
		else if (*q == '\n')
			{
			size = 0;
			if (q[1])
				{
				if ((size = (*pfn)(arg, r = rbuf, BUFSIZE)) <= 0)
					return (nargs ? nargs : EOF);
				}
			}
		else if (q[1] == ' ')
			{
			++q;
			while (!size || iswhite(*r))
				{
				if (!size && (size = (*pfn)(arg, r = rbuf, BUFSIZE)) <= 0)
					return (nargs ? nargs : EOF);
				while (size && iswhite(*r))
					--size, ++r;
				}
			}
		else
			{
			++q;
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
			else
				base = 10;
			or = r;
			osize = size;
			if (0 < width)
				{
				s = r;
				n = min(width, size);
				if (0 < n && r[n - 1] == '\n')
					--n;
				r =+ n;
				size =- n;
				}
			else
				{
				for (; 0 < size && iswhite(*r); ++r)
					--size;
				for (s = r; 0 < size && !iswhite(*r); ++r)
					--size;
				if (!(n = r - s))
					return (nargs);
				}
			if (rjust)
				for (; 0 < n && *s == cfill; ++s)
					--n;
			else
				for (t = &s[n - 1]; 0 < n && *t == cfill; --t)
					--n;
			if (*q == 'x')
				;
			else if (*q == 'b' || *q == 'p')
				{
				if (prec)
					n = min(n, prec);
				cpybuf(*pp, s, n);
				if (*q == 'p')
					{
					(*pp)[n] = '\0';
					++pp;
					++nargs;
					}
				else
					{
					++pp;
					*(BYTES *)*pp++ = n;
					nargs =+ 2;
					}
				}
			else if (*q == 'l' || *q == 'i' && sizeof (int) == sizeof (LONG))
				{
				if (mod == 'a')
					for (lnum = 0; 0 < n; --n)
						lnum = (lnum << 8) | *s++;
				else if (btol(s, n, &lnum, base) != n)
					return (nargs);
				*(LONG *)*pp++ = lnum;
				++nargs;
				}
			else if (*q == 'd' || *q == 'f')
				{
				if (btod(s, n, &dble) != n)
					return (nargs);
				else if (*q == 'd')
					*(DOUBLE *)*pp++ = dble;
				else
					*(float *)*pp++ = dble;
				++nargs;
				}
			else if (*q == 'c' || *q == 'i' || *q == 's' || mod)
				{
				if (mod == 'a')
					for (inum = 0; 0 < n; --n)
						inum = (inum << 8) | *s++;
				else if (btos(s, n, &inum, base) != n)
					return (nargs);
				if (*q == 'c')
					**pp++ = inum;
				else
					*(COUNT *)*pp++ = inum;
				++nargs;
				if (*q != 'c' && *q != 'i' && *q != 's')
					--q;
				}
			else
				if (0 < osize && *or == *q)
					{
					r = or + 1;
					size = osize - 1;
					}
				else
					return (nargs);
			}
		}
	}
