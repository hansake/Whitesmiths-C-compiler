/*	CONVERT LEADING LOW STRING TO INT
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT lstoi(s)
	FAST UTINY *s;
	{
	COUNT wo = s[1] << 8 | s[0];

	return (wo);
	}
