/*	FIND MATCHING PATTERN IN BUFFER
 *	copyright (c) 1980 by Whitesmiths Ltd.
 */
#include <std.h>
#include <pat.h>

/*	look for match anchored at index
 */
BYTES amatch(buf, n, indx, pat, pmsub)
	TEXT *buf;
	TEXT *pat;
	FAST BYTES n;
	BYTES indx;
	MSUB *pmsub;
	{
	FAST BYTES k;
	BYTES i;
	TEXT *rpat;

	while (*pat != PEND)
		if (*pat == CLOSE)
			{
			for (rpat = pat + 1, i = indx; i < n; rpat = pat + 1)
				if (!omatch(buf, n, &i, &rpat, NULL))
					break;
			for (; ; --i)
				if ((k = amatch(buf, n, i, rpat, pmsub)) != -1 || indx == i)
					return (k);
			}
		else if (!omatch(buf, n, &indx, &pat, pmsub))
			return (-1);
	return (indx);
	}

/*	look for unanchored match
 */
BOOL match(buf, n, pat)
	TEXT *buf;
	FAST BYTES n;
	FAST TEXT *pat;
	{
	FAST BYTES indx;

	if (*pat == SBOL)
		return (amatch(buf, n, 0, pat, NULL) != -1);
	for (indx = 0; indx < n; ++indx)
		if (buf[indx] != pat[3] && pat[2] == CCHAR)
			;
		else if (amatch(buf, n, indx, pat, NULL) != -1)
			return (YES);
	return (NO);
	}

/*	return length of subpat in offset
 */
LOCAL BOOL mtchccl(c, pat, offset)
	FAST TEXT c, *pat;
	BYTES *offset;
	{
	FAST TEXT rlc, ruc, *stpat = pat;
	BOOL matched;

	for (matched = NO; *pat != CCLEND; )
		switch (*pat)
			{
		case CCHAR:
			if (c == *++pat)
				matched = YES;
			++pat;
			break;
		case RANGE:
			for (rlc = *++pat, ruc = *++pat; rlc <= ruc; ++rlc)
				if (c == rlc)
					matched = YES;
			++pat;
			break;
			}
	*offset = pat - stpat + 1;
	return (matched);
	}

/* try to match one sub pattern
 */
LOCAL BOOL omatch(buf, n, pindx, pat, pmsub)
	TEXT *buf, **pat;
	FAST BYTES *pindx, n;
	MSUB *pmsub;
	{
	FAST TEXT *pp, *ppat;
	COUNT bump;
	BYTES offset;

	offset = 0;
	pp = &buf[*pindx];
	ppat = *pat;
	bump = -1;
	switch (*ppat)
		{
	case SBOL:
		if (*pindx == 0)
			{
			bump = 0;
			++ppat;
			}
		break;
	case ANY:
		if (*pindx < n && *pp != '\n')
			bump = 1;
		++ppat;
		break;
	case SEOL:
		if (*pp == '\n' || *pindx == n)
			{
			bump = 0;
			++ppat;
			}
		break;
	case CCL:
		if (*pindx < n && mtchccl(*pp, ++ppat, &offset))
			bump = 1;
		ppat =+ offset;
		break;
	case NCCL:
		if (*pindx < n && *pp != '\n' && !mtchccl(*pp, ++ppat, &offset))
			bump = 1;
		ppat =+ offset;
		break;
	case LPAR:
		if (pmsub)
			{
			pmsub[ppat[1]].mtext = pp;
			pmsub[ppat[1]].mlen = 0;
			}
		ppat =+ 2;
		bump = 0;
		break;
	case RPAR:
		if (pmsub)
			pmsub[ppat[1]].mlen = pp - pmsub[ppat[1]].mtext;
		ppat =+ 2;
		bump = 0;
		break;
	case CCHAR:
		*pat =+ 2;
		if (*pindx < n && *pp == ppat[1])
			{
			++*pindx;
			return (YES);
			}
		return (NO);
		}
	*pat = ppat;
	if (0 <= bump)
		{
		*pindx =+ bump;
		return (YES);
		}
	else
		return (NO);
	}
