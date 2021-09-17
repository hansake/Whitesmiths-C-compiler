/*	CATENATE AT MOST n CHARS OF s2 ONTO s1
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *strncat(s1, s2, n)
	FAST TEXT *s1, *s2;
	BYTES n;
	{
	BYTES i;
	TEXT *prevs1 = s1;

	for ( ; *s1 != '\0'; ++s1)
		;
	for (i = 0; i < n && s2[i] != '\0'; ++i)
		*s1++ = s2[i];
	*s1 = '\0';
	return (prevs1);
	}

#ifdef TRYMAIN
main()
	{
	TEXT *strncat();
	LOCAL TEXT targ[8] = "abc";

	if (!cmpstr("abcxyz", strncat(targ, "xyzw", 3)))
		putfmt("No good\n");
	exit(1);
	}
#endif
