/*	WRITE UNTIL BUFFER EMPTY
 *	copyright (c) 1982 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID fwrite(fd, buf, n)
	FILE fd;
	TEXT *buf;
	BYTES n;
	{
	IMPORT TEXT *writerr;
	FAST BYTES j;
	FAST COUNT i;

	if (write(fd, buf, n) != n)
		_raise(NULL, &writerr);
	}
