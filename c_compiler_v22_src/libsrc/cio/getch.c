/*	GET A CHARACTER FROM STDIN
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT getch()
	{
	IMPORT FIO stdin;

	return (getc(&stdin));
	}
