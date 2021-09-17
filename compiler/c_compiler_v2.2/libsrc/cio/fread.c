/*	READ UNTIL BUFFER FULL
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT fread(fd, buf, n)
	FILE fd;
	TEXT *buf;
	BYTES n;
	{
	IMPORT TEXT *readerr;
	FAST BYTES j;
	FAST COUNT i;

	for (j = 0; j < n; buf =+ i, j =+ i)
		if ((i = read(fd, buf, n - j)) == 0)
			break;
		else if (i < 0)
			_raise(NULL, &readerr);
	return (j);
	}
