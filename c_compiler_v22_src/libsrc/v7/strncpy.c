/*	COPY n BYTES FROM s2 TO s1 (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <stdio.h>
#include <stdtyp.h>

TEXT *strncpy(s1, s2, n)
	FAST TEXT s1[], s2[];
	BYTES n;
	{
	FAST BYTES i;

	for (i = 0; i < n && s2[i] != '\0'; ++i)
		s1[i] = s2[i];
	for ( ; i < n; ++i)
		s1[i] = '\0';
	return (s1);
	}
