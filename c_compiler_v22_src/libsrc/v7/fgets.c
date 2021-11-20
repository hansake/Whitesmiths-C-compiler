/*	GET STRING FROM FILE (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *fgets(s, n, fp)
	TEXT *s;
	BYTES n;
	FIO *fp;
	{
	COUNT i;

	i = getl(fp, s, n-1);
	if (i == 0)
		{
		_seteof(fp, YES);
		return (NULL);
		}
	else
		{
		s[i] = '\0';
		_seteof(fp, NO);
		return (s);
		}
	}

#ifdef TRYMAIN
main()
	{
	IMPORT FIO stdin;
	TEXT s[BUFSIZE];

	while (fgets(s, BUFSIZE, &stdin) != NULL)
		printf("%s", s);
	}
#endif
