/*	FIND CHAR IN STRING (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *index(s1, c)
	TEXT *s1;
	TEXT c;
	{
	while (*s1 && *s1 != c)
		s1++;
	if (*s1)
		return (s1);
	else
		return (NULL);
	}

#ifdef TRYMAIN
main()
	{
	TEXT *index();

	if (*index("abc", 'b') != 'b' || index("abc", 'x') != NULL)
		printf("no good\n");
	}
#endif
