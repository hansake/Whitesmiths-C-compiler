/*	FIND SUBSTRING pp IN ps
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES subbuf(ps, ns, pp, np)
	TEXT *ps, *pp;
	BYTES ns, np;
	{
	FAST TEXT *p, *q, *s;

	if (!np)
		return (0);
	for (s = ps; s - ps + np <= ns; ++s)
		if (*s == *pp)
			{
			for (q = s, p = pp; p - pp < np; ++p, ++q)
				if (*q != *p)
					break;
			if (np <= p - pp)
				return (s - ps);
			}
	return (ns);
	}
