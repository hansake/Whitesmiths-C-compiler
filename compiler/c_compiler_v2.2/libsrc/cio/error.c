/*	PRINT FATAL ERROR MESSAGE
 *	copyright (c) 1980 Whitesmiths, Ltd.
 */
#include <std.h>

/*	local putstr, to minimize references
 */
LOCAL VOID pstr(s)
	FAST TEXT *s;
	{
	FAST TEXT *t;

	if (s && *s)
		{
		for (t = s; *t; ++t)
			;
		write(STDERR, s, t - s);
		}
	}

/*	write error message and exit
 */
VOID error(s1,s2)
	TEXT *s1, *s2;
	{
	IMPORT TEXT *_pname;

	pstr(_pname);
	pstr(": ");
	pstr(s1);
	pstr(s2);
	pstr("\n");
	exit(NO);
	}
