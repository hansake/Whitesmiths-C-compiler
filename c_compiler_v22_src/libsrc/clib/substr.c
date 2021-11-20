/*	FIND SUBSTRING pp IN ps
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES substr(ps, pp)
	TEXT *ps, *pp;
	{
	return (subbuf(ps, lenstr(ps), pp, lenstr(pp)));
	}
