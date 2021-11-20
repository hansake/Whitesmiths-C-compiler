/*	CANONICALIZE PDP-11 FLOATING POINT
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT _fcan(d)
	FAST TEXT *d;
	{
	FAST COUNT i, t;

	if (!d[0] && !d[1])
		return (0);
	for (i = 0; i < 8; i =+ 2)
		t = d[i], d[i] = d[i + 1], d[i + 1] = t;
	t = (d[0] & 0177) << 1 | (d[1] & 0200) >> 7;
	d[0] =& 0200;
	d[1] =| 0200;
	return (t - 0200);
	}
