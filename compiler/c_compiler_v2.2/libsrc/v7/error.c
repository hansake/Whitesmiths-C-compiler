/*	PRINT FATAL ERROR MESSAGE
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <stdio.h>
#include <stdtyp.h>

#define FAIL	1

VOID error(s1, s2)
	TEXT *s1, *s2;
	{
	write(STDERR, s1, strlen(s1));
	write(STDERR, " ", 1);
	write(STDERR, s2, strlen(s2));
	write(STDERR, "\n", 1);
	exit(FAIL);
	}
