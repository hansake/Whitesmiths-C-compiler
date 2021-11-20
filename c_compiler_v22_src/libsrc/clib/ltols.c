/*	MAP LONG TO PDP-11 BYTE ORDER
 *	copyright (c) 1979 by Whitesmiths Ltd.
 */
#include <std.h>

TEXT *ltols(pl, lo)
	TEXT *pl;
	LONG lo;
	{
	FAST COUNT *pi = pl;

	itols(pi, (COUNT)(lo >> 16));
	itols(pi + 1, (COUNT)lo);
	return (pi);
	}
