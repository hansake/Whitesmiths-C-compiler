/*	FORMATTED OUTPUT TO FILE I/O BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID putf(pf, fmt, args)
	FIO *pf;
	TEXT *fmt, *args;
	{
	IMPORT COUNT putl();

	_putf(&putl, pf, fmt, &args);
	}
