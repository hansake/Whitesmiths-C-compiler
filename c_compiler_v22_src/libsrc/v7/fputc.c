/*	PUT A CHARACTER c TO FILE fp (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID fputc(c, fp)
	FIO *fp;
	COUNT c;
	{
	putc(fp, c);
	if (fp->_fmode != BWRITE)
		putc(fp, -1);
	}
