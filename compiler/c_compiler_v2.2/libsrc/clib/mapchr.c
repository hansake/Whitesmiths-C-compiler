/*	MAP A CHARACTER INTO 2 BYTES OF PRINTABLE TEXT
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID mapchar(c, buf)
	TEXT c, *buf;
	{
	if (' ' <= c && c < 0177)
		{
		*buf++ = ' ';
		*buf = c;
		}
	else
		{
		*buf++ = '\\';
		*buf = (0 <= c && c < 14) ? "01234567btnvfr"[c] : '?';
		}
	}
