/*	FORMATTED OUTPUT TO STDERR
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID errfmt(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT COUNT write();

	_putf(&write, STDERR, fmt, &args);
	}
