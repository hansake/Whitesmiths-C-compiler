/*	IS S2 A PREFIX OF S1
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BOOL prefix(s1, s2)
	FAST TEXT *s1, *s2;
	{
	while (*s2)
		if (*s1++ != *s2++)
			return (NO);
	return (YES);
	}
