/*	PROCESS ESCAPE SEQUENCES
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT doesc(pp, magic)
	TEXT **pp, *magic;
	{
	FAST COUNT d, n;
	FAST TEXT *p;

	p = *pp + 1;
	if (*p == '\0')
		return (**pp);
	else if (isdigit(*p))
		{
		for (d = 0, n = 1; isdigit(*p) && n <= 3; ++p, ++n)
			d = (d << 3) + *p - '0';
		*pp = p - 1;
		return (d);
		}
	else if ((d = scnstr("bfnrtv", tolower(*p))) < 5)
		{
		*pp = p;
		return ("\b\f\n\r\t\v"[d]);
		}
	else if (magic && magic[d = scnstr(magic, *p)])
		{
		*pp = p;
		return (-1 - d);
		}
	else
		{
		*pp = p;
		return (*p);
		}
	}
