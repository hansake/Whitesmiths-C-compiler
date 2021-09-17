/*	CONVERT STRING TO int
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES atoi(s)
	TEXT *s;
	{
	BYTES n;

	btoi(s, BUFSIZE, &n, 10);
	return (n);
	}

#ifdef TRYMAIN
main()
	{
	putfmt("0 = %i, 32767 = %i, -32767 = %i\n",
		atoi("0"), atoi("32767"), atoi("-32767"));
	}
#endif
