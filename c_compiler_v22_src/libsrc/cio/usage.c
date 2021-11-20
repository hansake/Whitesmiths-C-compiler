/*	WRITE STANDARD "USAGE" TEXT TO STDERR
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>
 
COUNT usage(msg)
	TEXT *msg;
	{
	IMPORT TEXT *_pname;
	FAST COUNT r;

	r = 0;
	putstr(STDERR, "usage: ", _pname, " ", msg, NULL);
	if (msg && msg[(r = lenstr(msg)) - 1] == '\n')
		exit(NO);
	return (r + lenstr(_pname) + 8);
	}
