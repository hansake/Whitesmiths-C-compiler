/*	MAKE A PATTERN
 *	copyright (c) 1980 by Whitesmiths Ltd.
 */
#include <std.h>
#include <pat.h>

/*	form character class sub pattern
 */
LOCAL TEXT *getccl(pp, pat)
	FAST TEXT **pp, *pat;
	{
	BOOL ranok;
	COUNT n;
	TEXT *p;

	p = *pp;
	if (*++p == '!')
		{
		*pat++ = NCCL;
		++p;
		}
	else
		*pat++ = CCL;
	for (ranok = NO; *p != ']' && *p; ++p)
		switch (*p)
			{
		case '\\':
			*pat++ = CCHAR;
			*pat++ = doesc(&p, "");
			ranok = YES;
			break;
		case '-':
			if (ranok && p[1] != ']')
				{
				n = (*++p == '\\') ? doesc(&p, "") : *p;
				pat[-2] = RANGE;
				*pat++ = n;
				ranok = NO;
				break;
				}
		default:
			*pat++ = CCHAR;
			*pat++ = *p;
			ranok = YES;
			}
	*pat++ = CCLEND;
	*pp = (!p) ? --p: p;
	return (pat);
	}

TEXT *pattern(pat, delim, p)
	FAST TEXT *pat;
	TEXT *p, delim;
	{
	FAST COUNT c;
	FAST TEXT *t;
	COUNT nlp, parstack[10], *pstack;
	TEXT *last;

	nlp = 0;
	pstack = parstack;
	if (*p == '^')
		{
		++p;
		*pat++ = SBOL;
		}
	*pat++ = LPAR;
	*pat++ = 0;
	for (last = NULL; *p && *p != delim; ++p)
		{
		if (*p != '^')
			last = pat;
		switch (*p)
			{
		case '?':
			*pat++ = ANY;
			break;
		case '*':
			*pat++ = CLOSE;
			*pat++ = ANY;
			break;
		case '$':
			if (p[1] == delim || !p[1])
				*pat++ = SEOL;
			else
				{
				*pat++ = CCHAR;
				*pat++ = '$';
				}
			break;
		case '^':
			if (!last || *last == LPAR || *last == RPAR || *last == CLOSE)
				{
				*pat++ = CCHAR;
				*pat++ = *p;
				}
			else
				{
				for (t = ++pat; t != last; --t)
					t[0] = t[-1];
				*last = CLOSE;
				}
			break;
		case '[':
			pat = getccl(&p, pat);
			break;
		case '\\':
			if (0 <= (c = doesc(&p, "()")))
				{
				*pat++ = CCHAR;
				*pat++ = c;
				}
			else if (c == -1)
				{
				if (0 <= ++nlp && nlp < 10)
					{
					*pstack++ = nlp;
					*pat++ = LPAR;
					*pat++ = nlp;
					}
				}
			else if (c == -2)
				{
				if (10 <= nlp)
					--nlp;
				else if (0 <= nlp)
					{
					--pstack;
					*pat++ = RPAR;
					*pat++ = *pstack;
					}
				}
			break;
		default:
			*pat++ = CCHAR;
			*pat++ = *p;
			}
		}
	*pat++ = RPAR;
	*pat++ = 0;
	*pat++ = PEND;
	*pat = 0;
	return ((*p == delim && parstack == pstack) ? p : NULL);
	}
