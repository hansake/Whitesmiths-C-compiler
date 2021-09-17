/*	CONVERT LEADING LOW STRING TO UNSIGNED SHORT
 *	copyright (c) 1982 by Whitesmiths, Ltd.
 */
#include <std.h>

UCOUNT lstou(s)
	FAST UTINY *s;
	{
	return (s[1] << 8 | s[0]);
	}
