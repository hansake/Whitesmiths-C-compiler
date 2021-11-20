/*	RAND, SRAND - GENERATE RANDOM NUMBERS
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <stdtyp.h>

LOCAL LONG rnum {0};

VOID srand(n)
	COUNT n;
	{
	rnum = (LONG)n;
	}

COUNT rand()
	{
	rnum = rnum * 0x41c64e6d + 0x3039;
	return ((COUNT)(rnum >> 16) & 0x7FFF);
	}
