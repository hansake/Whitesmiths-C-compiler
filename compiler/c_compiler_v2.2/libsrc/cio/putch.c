/*	PUT A CHARACTER TO STDOUT
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT putch(c)
	COUNT c;
	{
	IMPORT FIO stdout;

	return (putc(&stdout, c));
	}
