/*	FREAD (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES fread7(ptr, n, nitems, stream)
	TEXT *ptr;
	BYTES n, nitems;
	FIO *stream;
	{
	FAST BYTES i;
	FAST BYTES lim = n * nitems;
	FAST COUNT c;	/* should be METACH, if your std.h has it */

	for (i = 0; i < lim; ++i)
		{
		c = gtc(stream);
		if (c == EOF)
			return (i);
		*ptr++ = c;
		}
	return (i);
	}
