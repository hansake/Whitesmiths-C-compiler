/*	FORMATTED INPUT FROM STDIN
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT getfmt(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT COUNT getl();
	IMPORT FIO stdin;

	return (_getf(&getl, &stdin, fmt, &args));
	}
