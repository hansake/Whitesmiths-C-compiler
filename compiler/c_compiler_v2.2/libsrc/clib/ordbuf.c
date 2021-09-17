/*	COMPARE TWO NULL PADDED STRING BUFFERS FOR LEXICAL ORDER
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT ordbuf(p, q, n)
	FAST COUNT n;
	FAST TEXT *p, *q;
	{
	while (0 <= --n && *p++ == *q++)
		;
	if (n < 0)
		return (0);
	else if (p[-1] < q[-1])
		return (-1);
	else
		return (1);
	}
