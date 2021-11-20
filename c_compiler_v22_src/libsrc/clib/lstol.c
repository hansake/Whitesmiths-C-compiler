/*	PDP-11 BYTE ORDER TO LONG
 */
#include <std.h>

LONG lstol(s)
	FAST UTINY *s;
	{
	return ((ULONG)(s[1] << 8 | s[0]) << 16 | (s[3] << 8 | s[2]));
	}
