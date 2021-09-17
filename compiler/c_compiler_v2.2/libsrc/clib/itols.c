/*	CONVERT INT TO LEADING LOW STRING
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *itols(s, n)
	FAST TEXT *s;
	COUNT n;
	{
	s[0] = n;
	s[1] = n >> 8;
	return (s);
	}
