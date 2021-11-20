/*	FORMATTED INPUT FROM FILE I/O BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT getf(pf, fmt, args)
	FIO *pf;
	TEXT *fmt, *args;
	{
	IMPORT COUNT getl();

	return (_getf(&getl, pf, fmt, &args));
	}
