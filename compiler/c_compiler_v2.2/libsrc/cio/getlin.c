/*	GET A LINE FROM STDIN INTO BUFF
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES getlin(s, n)
	TEXT *s;
	BYTES n;
	{
	IMPORT FIO stdin;

	return (getl(&stdin, s, n));
	}
