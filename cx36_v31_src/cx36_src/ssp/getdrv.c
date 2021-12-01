/*  DECODE DRIVER NAME IN FILENAME UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <ctype.h>
#include "ssp.h"

DRVR *_getdrv(s)
	TEXT *s;
	{
	IMPORT RDONLY DRVR _drvs[];

	FAST DRVR *d;
	FAST TEXT *dnamep;
	FAST COUNT len;
	TEXT name[_NAMSIZ+1];

	for (len = 0; *s && *s != ':'; s++)
		if (len < _NAMSIZ)
			name[len++] = _toupper(*s);
	if (*s == ':')
		{
		name[len] = '\0';
		dnamep = name;
		}
	else
		dnamep = _DEFNAM;
	for (d = _drvs; d->drname; ++d)
		if (_cstreq(d->drname, dnamep, _NAMSIZ))
			return (d);
	_errno = ENODRVR;
	return ((DRVR *)0);
	}
