/*	FORMATTED READ (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <stdio.h>
#include <stdtyp.h>
#include <ctype.h>

#define BUFL	127		/* length of intermediate text */
#define MATCHL	127		/* maximum no. of match chars in [] */
#define GET1C(p) (isstr ? GET1CS(p) : getc((FILE *)p))
#define GET1CS(p) (*p ? *p++ : EOF)
#define GOODDIGIT(c) ('0' <= c && c <= 'f' && digit[c - '0'] < base)
#define UNGET1C(c, p) (isstr ? --p : ungetc(c, (FILE *)p))

/*	formatted read from stdin
 */
COUNT scanf(fmt, arg)
	TEXT *fmt, *arg;
	{
	return (_scan(stdin, 0, fmt, &arg));
	}

/*	formatted read from file
 */
COUNT fscanf(fp, fmt, arg)
	FILE *fp;
	TEXT *fmt, *arg;
	{
	return (_scan(fp, 0, fmt, &arg));
	}

/*	formatted read from string
 */
COUNT sscanf(s, fmt, arg)
	TEXT *s, *fmt, *arg;
	{
	return (_scan(s, 1, fmt, &arg));
	}

/*	internal scan routine
 */
LOCAL COUNT _scan(p, isstr, s, parg)
	TEXT *p;
	BOOL isstr;
	FAST TEXT *s;
	TEXT **parg;
	{
	BOOL end, toofar;
	COUNT matchlen, nc, w;
	TEXT c, buf[BUFL+2], match[MATCHL+2], matchtyp, mod, noassn, typ;
	TEXT **parg0, *s0;

	parg0 = parg;
	end = NO;
	while (*s && !end)
		{
		s0 = s;
		if (*s == ' ' || *s == '\t' || *s == '\n')
			++s;
		else if (*s != '%' || *s == '%' && *++s == '%')
			{
			c = GET1C(p);
			end = *s++ != c;
			}
		else
			{
			noassn = *s == '*' ? *s++ : ' ';
			w = 0;
			for ( ; isdigit(*s); ++s)
				w = w * 10 + *s - '0';
			if (*s == 'l' || *s == 'h')
				mod = *s++;
			else
				mod = '\0';
			typ = *s++;
			if (typ == '[')
				{
				if (*s == '^')
					matchtyp = *s++;
				else
					matchtyp = ' ';
				matchlen = (COUNT)index(s, ']') - (COUNT)s;
				if (MATCHL < matchlen)
					error("scanf [] set too big: ", s0);
				strncpy(match, s, matchlen);
				match[matchlen] = '\0';
				s =+ 1 + matchlen;
				}
			else
				matchlen = 0;
			if (BUFL < w)
				error("scanf width too big: ", s0);
			if (typ == 'c' && w == 0)
				w = 1;
			nc = 0;
			toofar = NO;
			FOREVER
				{
				if (BUFL < nc)
					error("scanf input field too long: ", buf);
				else if (0 < w && w <= nc)
					break;
				else if ((c = GET1C(p)) == EOF)
					break;
				else if (0 < matchlen 
					&& !index(match, c) == (matchtyp == ' '))
					{
					UNGET1C(c, p);
					break;
					}
				else if (typ != 'c' && (c==' ' || c=='\t' || c=='\n'))
					{
					if (nc != 0)
						{
						UNGET1C(c, p);
						break;
						}
					}
				else if (typ == 'c')
					buf[nc++] = c;
				else
					{
					switch (typ)
						{
					case 'd':
					case 'D':
						if (!(isdigit(c) || c == '-' || c == '+'))
							toofar = YES;
						break;
					case 'o':
					case 'O':
						if (!isdigit(c) || c == '8' || c == '9')
							toofar = YES;
						break;
					case 'x':
					case 'X':
						if (!isxdigit(c))
							toofar = YES;
						break;
					case 'e':
					case 'f':
					case 'E':
					case 'F':
						if (!isdigit(c) && index("eE.-+", c) == NULL)
							toofar = YES;
						break;
						}
					if (toofar)
						{
						UNGET1C(c, p);
						break;
						}
					else
						buf[nc++] = c;
					}
				}
			buf[nc] = '\0';
			if (noassn != '*' && nc != 0)
				{
				if (typ == 'c' || typ == 's' || typ == '[')
					outstr(*parg++, typ, buf);
				else if (index("doxDOX", typ) != NULL)
					outint(*parg++, mod, typ, buf);
				else if (index("efEF", typ) != NULL)
					outflt(*parg++, mod, typ, buf);
				else
					error("illegal scanf format: ", s0);
				}
			if (c == EOF || nc == 0)
				end = YES;
			}
		}
	if (parg == parg0 && c == EOF)
		return (EOF);
	else
		return (parg - parg0);
	}

LOCAL VOID outstr(arg, typ, buf)
	TEXT *arg, typ, *buf;
	{
	while (*buf)
		*arg++ = *buf++;
	if (typ != 'c')
		*arg = '\0';
	}

LOCAL VOID outint(arg, mod, typ, buf)
	TEXT *arg, mod, typ, *buf;
	{
	COUNT base;
	LONG val;
	TEXT sign;
	INTERN TEXT digit[] = 
		{
		0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 99, 99, 99, 99, 99, 99,
	   99, 10, 11, 12, 13, 14, 15, 99, 99, 99, 99, 99, 99, 99, 99, 99,
	   99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
	   99, 10, 11, 12, 13, 14, 15,
		};

	if (*buf == '-' || *buf == '+')
		sign = *buf++;
	else
		sign = '+';
	if (typ == 'x' || typ == 'X')
		base = 16;
	else if (typ == 'd' || typ == 'D')
		base = 10;
	else
		base = 8;
	val = 0;
	while (GOODDIGIT(*buf))
		val = val * base - digit[*buf++ - '0'];
	if (sign == '+')
		val = -val;
	if (mod == 'h')
		*(COUNT *)arg = val;
	else if (mod == 'l' || isupper(typ))
		*(LONG *)arg = val;
	else
		*(int *)arg = val;
	}

LOCAL VOID outflt(arg, mod, typ, buf)
	TEXT *arg, mod, typ, *buf;
	{
	DOUBLE atof(), val;

	val = atof(buf);
	if (mod == 'l' || isupper(typ))
		*(double *)arg = val;
	else
		*(float *)arg = val;
	}

#ifdef TRYMAIN
main()
	{
	BYTES ii;
	COUNT args, si;
	DOUBLE d;
	FLOAT f;
	LONG li;
	TEXT c, fmt[BUFSIZ], in[BUFSIZ], s[BUFSIZE];

	args = 0;
	while (args != EOF && fgets(in, BUFSIZ, stdin))
	{
		fgets(fmt, BUFSIZ, stdin);
		args = scanf(fmt, s, &si, &ii, &li, &f, &d);
		printf("args=%d, s=%s, si=%d, ii=%d, li=0x%lx, f=%10.4e, d=%10.4e\n",
			args, s, si, ii, li, f, d);
		getchar();
	}
}
#endif
