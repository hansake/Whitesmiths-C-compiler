/*  EVALUATE RECORD LENGTH UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */
#include <wslxa.h>
#include "ssp.h"

BYTES _evalrecl(s)
	TEXT *s;
	{
	while (*s)
		{
		if (*s == 'r')
			{
			if (_cstreq(s, "recl=", 4))
				{
				return (_catoi(&s[5]));
				}
			}
		s++;
		}
	return (0);
	}
