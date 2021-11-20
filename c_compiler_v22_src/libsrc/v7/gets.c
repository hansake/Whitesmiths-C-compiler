/*	GET STRING (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <stdio.h>
#include <stdtyp.h>

TEXT *gets(s0)
	TEXT *s0;
	{
	FAST TEXT *s;
	METACH c;

	s = s0;
	while ((c = getchar()) != EOF && c != '\n')
		*s++ = c;
	*s = '\0';
	return (c == EOF ? NULL : s0);
	}

#ifdef TRYMAIN
main()
	{
	TEXT s[BUFSIZ];

	while (gets(s))
		puts(s);
	}
#endif
