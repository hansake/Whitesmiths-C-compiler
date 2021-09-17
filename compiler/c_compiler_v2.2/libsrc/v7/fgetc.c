/*	GET CHAR FROM fp (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT fgetc(fp)
	FIO *fp;
	{
	COUNT c;

	c = getc(fp);
	_seteof(fp, c == EOF);
	return (c);
	}
