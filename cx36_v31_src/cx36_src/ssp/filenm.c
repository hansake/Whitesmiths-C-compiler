/*  CONVERT C FILE NAME TO FILE NAME UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <ctype.h>
#include "ssp.h"

/*	convert s to an 8 byte s/36 filename
 */
_filename(s, dest)
	TEXT *s, *dest;
	{
#ifndef _EBCDIC
	IMPORT RDONLY TEXT _atoe[];
#endif
	FAST COUNT len;

	for (len = 0; *s && len < _NAMSIZ; s++)
		if (*s == ':')
			len = 0;
		else
#ifdef _EBCDIC
			dest[len++] = _toupper(*s);
#else
			dest[len++] = _atoe[_toupper(*s)];
#endif
	while (len < _NAMSIZ)
#ifdef _EBCDIC
		dest[len++] = ' ';
#else
		dest[len++] = _atoe[' '];
#endif
	}
