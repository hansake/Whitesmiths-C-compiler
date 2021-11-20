/*	FORMATTED OUTPUT TO STDOUT
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID putfmt(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT COUNT putl();
	IMPORT FIO stdout;

	_putf(&putl, &stdout, fmt, &args);
	}
