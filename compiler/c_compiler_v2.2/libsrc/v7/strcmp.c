/*	STRING COMPARE (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT strcmp(s1, s2)
	TEXT *s1, *s2;
	{
	for ( ; *s1 && *s1 == *s2; ++s1, ++s2)
		;
	return (*s1 - *s2);
	}

#ifdef TRYMAIN
main()
	{
	if (strcmp("", "") != 0 || strcmp("ab", "a") <= 0)
		printf("no good\n");
	}
#endif
