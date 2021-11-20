/*	CP/M GET COMMAND LINE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

#define MAXCMD	128

BOOL _main(s)
	FAST TEXT *s;
	{
	IMPORT TEXT *_pname;
	FAST COUNT i;
	FAST TEXT *t;
	COUNT ac, n;
	FILE fd;
	TEXT *av[64], *q, qbuf[MAXCMD+1];

	for (q = qbuf, n = *s++; 0 < n; )
		{
		while (0 < n && iswhite(*s))
			--n, ++s;
		if (n <= 0)
			break;
		for (; 0 < n && !iswhite(*s); --n, ++s)
			*q++ = tolower(*s);
		*q++ = '\0';
		}
	ac = 1;
	av[0] = _pname;
	for (t = qbuf; t != q; ++t)
		{
		if (*t == '<')
			{
			close(STDIN);
			if (open(t + 1, READ, 0) != STDIN)
				error("can't read ", t + 1);
			}
		else if (*t == '>')
			{
			close(STDOUT);
			if (create(t + 1, WRITE, 0) != STDOUT)
				error("can't write ", t + 1);
			}
		else
			av[ac++] = t;
		while (*t)
			++t;
		}
	return (main(ac, av));
	}
