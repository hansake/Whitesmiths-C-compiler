/*	CONVERT STRING TO LONG
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

LONG atol(s)
	TEXT *s;
	{
	LONG n;

	btol(s, BUFSIZE, &n, 10);
	return (n);
	}

#ifdef TRYMAIN
main()
	{
	putfmt("0 = %l, 32767 = %l, -32767 = %l\n",
		atol("0"), atol("32767"), atol("-32767"));
	}
#endif
