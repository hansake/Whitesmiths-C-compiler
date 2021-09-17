/*	DOUBLE ABSOLUTE VALUE
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>
#undef	abs

DOUBLE abs(d)
	DOUBLE d;
	{
	return ((d < 0) ? -d : d);
	}
