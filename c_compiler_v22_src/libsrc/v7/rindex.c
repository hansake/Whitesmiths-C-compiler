/*	RETURN POINTER TO LAST c IN STRING s (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *rindex(s, c)
	FAST TEXT *s, c;
	{
	TEXT *last;

	last = NULL;
	for ( ; *s != '\0'; ++s)
		if (*s == c)
			last = s;
	return (last);
	}

#ifdef TRYMAIN
main()
	{
	TEXT *rindex();

	if (*(rindex("abcbx", 'b') + 1) != 'x' ||
		 rindex("abc", 'x') != NULL)
		putfmt("No good\n");
	exit(1);
	}
#endif
