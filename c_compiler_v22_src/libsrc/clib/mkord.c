/*	MAKE ORDERING FUNCTION
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <sort.h>

typedef struct
	{
	BOOL rev;
	BOOL skipbl;
	TEXT cmptype;
	TEXT tabchar;
	COUNT begfsk;
	COUNT begaddch;
	COUNT endfsk;
	COUNT endaddch;
	} KEY;

LOCAL COUNT firstkey {KEYLIM};
LOCAL KEY key[KEYLIM + 1] {0};

/*	finds terminus of a key
 */
LOCAL TEXT *bound(p, tabchar, fsk, addch, end)
	FAST TEXT *p;
	TEXT tabchar;
	FAST COUNT fsk;
	COUNT addch;
	BOOL end;
	{
	FAST TEXT *q;
	TEXT *sp = p;

	for (; fsk && (*p != '\n'); ++p)
		if (tabchar)
			{
			if (*p == tabchar)
				--fsk;
			}
		else if (iswhite(*p))
			{
			if (--fsk || !end)
				for (q = p + 1; *q != '\n' && iswhite(*q); p = q++)
					;
			}
	for (; addch && *p != '\n'; ++p, --addch)
		;
	if (end && *p != '\n' && p != sp)
		--p;
	return (p);
	}

/*	compare field-dictionary order
 */ 
LOCAL COUNT dictcmp(p, q, lp, lq)
	FAST TEXT *p, *q;
	TEXT *lp, *lq;
	{
	INTERN TEXT ndict[128]
		{
		1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1
		};
	FAST COUNT ord;

	for (; ; ++p, ++q)
		{
		while ((p < lp) && ((*p & 0200) || ndict[*p]))
			++p;
		while ((q < lq) && ((*q & 0200) || ndict[*q]))
			++q;
		if (lp == p && lq == q)
			return (0);
		else if (lp == p && q < lq)
			return (-1);
		else if (p < lp && lq == q)
			return (1);
		else if (ord = *p - *q)
			return (ord);
		}
	}

/*	prepare sort keys for ordering
 *	function and return its address
 */
COUNT (*mkord(keyarray, lnordrule))()
	TEXT **keyarray, *lnordrule;
	{
	IMPORT COUNT firstkey, ordkey();
	IMPORT KEY key[];
	INTERN KEY defsort = {NO, NO, 'a', '\0', 0, 0, 0, -1};

	if (lnordrule)
		setkey(&defsort, lnordrule, &defsort);
	for (; *keyarray && firstkey; ++keyarray)
		if (!setkey(&key[--firstkey], *keyarray, &defsort))
			return (NULL);
	if (*keyarray)
		return (NULL);
	else if (firstkey == KEYLIM)
		cpybuf(&key[--firstkey], &defsort, sizeof (KEY));
	return(&ordkey);
	}

/*	compare fields-numeric order
 */
LOCAL VOID numcmp(p, q, lp, lq)
	FAST TEXT *p, *q;
	TEXT *lp, *lq;
	{
	COUNT ordp, ordq, diff, ord;
	TEXT *ip, *iq, *dpp, *dpq;

	while (iswhite(*p) && p < lp)
		++p;
	while (iswhite(*q) && q < lq)
		++q;
	ordp = ordq = 1;
	if (*p == '-')
		{
		++p;
		ordp = -ordp;
		}
	if (*q == '-')
		{
		++q;
		ordq = -ordq;
		}
	if (ordp != ordq)
		return (ordp);
	for (; *p == '0' && p < lp; ++p)
		;
	for (; *q == '0' && q < lq; ++q)
		;
	for (ip = p; p < lp && isdigit(*p); ++p)
		;
	for (iq = q; q < lq && isdigit(*q); ++q)
		;
	dpp = p;
	dpq = q;
	p = ip;
	q = iq;
	if (ord = (dpp - p) - (dpq - q))
		return ((ord < 0) ? -ordp : ordp);
	for (; p < dpp; ++p, ++q)
		if (diff = *p - *q)
			return ((diff < 0) ? -ordp : ordp);
	if (*(p = dpp) == '.')
		++p;
	if (*(q = dpq) == '.')
		++q;
	while (p < lp && isdigit(*p) && q < lq && isdigit(*q))
		if (diff = *p++ - *q++)
			return ((diff < 0) ? -ordp : ordp);
	while (p < lp && isdigit(*p))
		if (*p++ != '0')
			return (ordp);
	while (q < lq && isdigit(*q))
		if (*q++ != '0')
			return (-ordp);
	return (0);
	}

/*	sort buffers by key
 */
LOCAL COUNT ordkey(i, j, ppa)
	BYTES i, j;
	LINE ***ppa;
	{
	IMPORT COUNT firstkey;
	IMPORT KEY key[];
	FAST KEY *pk = &key[firstkey];
	FAST TEXT *p = (*ppa)[i]->text;
	FAST TEXT *q = (*ppa)[j]->text;
	BOOL revflag;
	COUNT ord;
	TEXT *sp, *sq, *lp, *lq;

	sp = p;
	sq = q;
	for (ord = 0; pk->cmptype && !ord; ++pk)
		{
		revflag = pk->rev;
		if(pk->begfsk || pk->begaddch)
			{
			p = bound(sp, pk->tabchar, pk->begfsk, pk->begaddch, NO);
			q = bound(sq, pk->tabchar, pk->begfsk, pk->begaddch, NO);
			}
		else
			{
			p = sp;
			q = sq;
			}
		if (pk->endfsk || 0 < pk->endaddch)
			{
			lp = bound(sp, pk->tabchar, pk->endfsk, pk->endaddch, YES);
			lq = bound(sq, pk->tabchar, pk->endfsk, pk->endaddch, YES);
			}
		else
			{
			lp = sp + (*ppa)[i]->len - 1;
			lq = sq + (*ppa)[j]->len - 1;
			}
		if (pk->skipbl)
			{
			while (iswhite(*p) && p < lp)
				++p;
			while (iswhite(*q) && q < lq)
				++q;
			}
		switch (pk->cmptype)
			{
		case 'n':
			ord = numcmp(p, q, lp, lq);
			break;
		case 'd':
			ord = dictcmp(p, q, lp, lq);
			break;
		case 'a':
		case 'l':
			for (; p < lp && q < lq; ++p, ++q)
				if (pk->cmptype == 'a')
					{
					if (*p != *q)
						break;
					}
				else if (tolower(*p) != tolower(*q))
					break;
			if (lp == p && lq == q)
				ord = 0;
			else if (lp == p && q < lq)
				ord = -1;
			else if (p < lp && lq == q)
				ord = 1;
			else if (pk->cmptype == 'a' && *p < *q)
				ord = -1;
			else if (pk->cmptype == 'l' && tolower(*p) < tolower(*q))
				ord = -1;
			else
				ord = 1;
			break;
		default:
			error("Panic: bad internal key format!", NULL);
			}
		}
	if (revflag)
		ord = -ord;
	return (ord);
	}

/*	convert external form of sort rule
 *	into internal key format
 */
LOCAL BOOL setkey(pk, p, proto)
	FAST KEY *pk;
	FAST TEXT *p;
	KEY *proto;
	{
	FAST TEXT *q;
	COUNT *pf, *pa;
	TEXT *sp = p;

	cpybuf(pk, proto, sizeof (KEY));
	if (*p == NULL)
		return (YES);
	for (; isalpha(*p); ++p)
		switch (*p)
			{
		case 'a':
			pk->cmptype = 'a';
			break;
		case 'b':
			pk->skipbl = YES;
			break;
		case 'd':
			pk->cmptype = 'd';
			break;
		case 'l':
			pk->cmptype = 'l';
			break;
		case 'n':
			pk->cmptype = 'n';
			break;
		case 'r':
			pk->rev = YES;
			break;
		case 't':
			pk->tabchar = *(++p);
			break;
		default:
			return (NO);
			}
	pf = &pk->begfsk;
	pa = &pk->begaddch;
	FOREVER 
		{
		if (isdigit(*p))
			{
			for (q = p++; isdigit(*p); ++p)
				;
			btos(q, p - q, pf, 10);
			}
		if (*p == '.')
			{
			for (q = ++p; isdigit(*p); ++p)
				;
			btos(q, p - q, pa, 10);
			}
		if (*p != '-')
			break;
		pk->endfsk = pk->endaddch = 0;
		pf = &pk->endfsk;
		pa = &pk->endaddch;
		++p;
		}
	return (YES);
	}
