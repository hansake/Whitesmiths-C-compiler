/*	FIND LENGTH OF STRING
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES lenstr(is)
	FAST TEXT *is;
	{
	FAST TEXT *s;

	for (s = is; *s; ++s)
		;
	return (s - is);
	}
