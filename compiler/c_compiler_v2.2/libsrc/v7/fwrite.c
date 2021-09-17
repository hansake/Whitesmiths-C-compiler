/*	FWRITE (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES fwrit7(ptr, n, nitems, stream)
	TEXT *ptr;
	BYTES n, nitems;
	FIO *stream;
	{
	FAST BYTES i;
	FAST BYTES lim = n * nitems;

	for (i = 0; i < lim; ++i)
		ptc(stream, *ptr++);
	return (i);
	}
