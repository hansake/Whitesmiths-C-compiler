/*	BUFFERED OUTPUT TO STDOUT BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES putlin(s, n)
	FAST TEXT *s;
	BYTES n;
	{
	IMPORT FIO stdout;

	return (putl(&stdout, s, n));
	}
