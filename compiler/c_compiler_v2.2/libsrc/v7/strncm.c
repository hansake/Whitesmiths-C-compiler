/*	BOUNDED STRING COMPARE (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT strncmp(s1, s2, n)
	TEXT *s1, *s2;
	BYTES n;
	{
	for ( ; --n && *s1 == *s2; ++s1, ++s2)
		;
	return (*s1 - *s2);
	}

#ifdef TRYMAIN
main()
	{
	if (strncmp("", "", 1) != 0 || strncmp("ab", "a", 1) != 0)
		printf("no good\n");
	}
#endif
