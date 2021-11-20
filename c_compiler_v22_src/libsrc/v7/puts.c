/*	PUT STRING (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <stdio.h>
#include <stdtyp.h>

VOID puts(s)
	FAST TEXT *s;
	{
	while (*s)
		putchar(*s++);
	putchar('\n');
	}
