/*	PRINT ERROR MESSAGE
 */
#include <std.h>

BOOL remark(s1, s2)
	TEXT *s1, *s2;
	{
	putstr(STDERR, s1, s2, NULL);
	write(STDERR, "\n", 1);
	return (NO);
	}
