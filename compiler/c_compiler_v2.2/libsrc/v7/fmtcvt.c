/*	CONVERT BTL printf FORMAT TO WSL putfmt FORMAT
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

#define CPY(t, t2) for (pt = t2; *pt; ) *t++ = *pt++

BOOL fmtcvt(s, t)
	FAST TEXT *s, *t;
	{
	COUNT i;
	TEXT adj, pad, w[4], p[4], mod, typ, *pt;
	TEXT *s0;

	while (*s)
		{
		s0 = s;
		if (*s != '%')
			*t++ = *s++;
		else if (*++s == '%')
			{
			++s;
			CPY(t, "%%");
			}
		else
			{
			*t++ = '%';
			adj = (*s == '-') ? *s++ : '+';
			if (*s == '0')
				pad = *s++;
			else
				pad = ' ';
			for (i = 0; isdigit(*s) || (*s == '*' && i == 0); ++i, ++s)
				w[i] = *s == '*' ? 'n' : *s;
			w[i] = '\0';
			p[0] = '\0';
			if (*s == '.')
				{
				for (++s, i = 0; isdigit(*s) || (*s == '*' && i == 0); ++i, ++s)
					p[i] = *s == '*' ? 'n' : *s;
				p[i] ='\0';
				}
			if (*s == 'l' || *s == 'h')
				mod = *s++;
			else
				mod = '\0';
			typ = *s++;
			/* now translate to WSL */
			if (w[0])
				{
				*t++ = adj;
				*t++ = pad;
				CPY(t, w);
				}
			if (p[0])
				{
				*t++ = '.';
				CPY(t, p);
				}
			if (typ == 's')
				*t++ = 'p';
			else if (typ == 'D' || typ == 'd' && mod == 'l')
				*t++ = 'l';
			else if (typ == 'd')
				*t++ = 'i';
			else if (typ == 'c')
				CPY(t, "ac");
			else if (typ == 'e' || typ == 'E' || typ == 'g' || typ == 'G')
				*t++ = 'd';
			else if (typ == 'f')
				*t++ = 'f';
			else if (typ == 'X' || typ == 'x' && mod == 'l')
				CPY(t, "hl");
			else if (typ == 'x')
				*t++ = 'h';
			else if (typ == 'O' || typ == 'o' && mod == 'l')
				CPY(t, "ol");
			else if (typ == 'o')
				*t++ = 'o';
			else if (typ == 'U' || typ == 'u' && mod == 'l')
				CPY(t, "ul");
			else if (typ == 'u')
				*t++ = 'u';
			else
				{
				errfmt("unknown printf format: %b\n", s0, s - s0);
				*t = '\0';
				return (NO);
				}
			}
		}
	*t = '\0';
	return (YES);
	}

#ifdef TRYMAIN
main()
	{
	TEXT s[BUFSIZE], t[BUFSIZE];
	COUNT m;

	while ((m = getlin(s, BUFSIZE)) > 0)
		{
		s[m - 1] = '\0';
		fmtcvt(s, t);
		putfmt("BTL=  %p\nWSL=  %p\n\n", s, t);
		}
	}
#endif
