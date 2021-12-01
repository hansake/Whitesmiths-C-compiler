/*  SYSTEM INTERFACE INTERNALS UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */
#include <wslxa.h>
#include "ssp.h"

/*	convert numeric string to integer
 */
INT _catoi(s)
	TEXT *s;
	{
	INT value = 0;

	while (*s >= '0' && *s <= '9')
		{
		value = (value * 10) + (*s - '0');
		s++;
		}
	return (value);
	}

/*	test if two strings are equal
 */
BOOL _cstreq(s1, s2, len)
	TEXT *s1, *s2;
	COUNT len;
	{
	while (*s1 && *s2 && *s1 == *s2 && len > 0)
		{
		s1++;
		s2++;
		len--;
		}
	return (*s1 == *s2 ? YES : NO);
	}

/*	flood buffer with fill character
 */
BYTES _fill(s, n, c)
	FAST TEXT *s;
	BYTES n;
	TEXT c;
	{
	FAST BYTES i;

	for (i = n; 0 < i; --i)
		*s++ = c;
	return (n);
	}
