/*	CONVERT BUF TO LOWERCASE IN PLACE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES lower(s, n)
	FAST TEXT *s;
	BYTES n;
	{
	FAST BYTES i;

	for (i = 0; i < n; ++i, ++s)
		*s = tolower(*s);
	return (n);
	}
