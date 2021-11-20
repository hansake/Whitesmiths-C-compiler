/*	DOUBLE ROUNDED TO INTEGER
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>

ARGINT round(d)
	DOUBLE d;
	{
	return ((d < 0) ? d - 0.5 : d + 0.5);
	}
