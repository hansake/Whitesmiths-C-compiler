/*	FLUSH FILE BUFFER (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT fflush(fp)
	FIO *fp;
	{
	putc(fp, -1);
	return (fp->_fmode == WRITE || fp->_fmode == BWRITE ? 0 : EOF);
	}
