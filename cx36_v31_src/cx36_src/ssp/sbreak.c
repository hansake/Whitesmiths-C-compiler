/*  SET SYSTEM BREAK UNDER IBM SYSTEM/36
 *  copyright (c) 1979, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"

VOID *_sbreak(size)
    BYTES size;
    {
    IMPORT TEXT *_stop;
    IMPORT TEXT *_memry;
	TEXT *obrk, yellow[40];
	INTERN TEXT *_brk = {_memry};

	obrk = _brk;
	_brk += size;
	if (yellow <= _brk || _brk < _memry)
		{
		_brk = obrk;
		_errno = ENOMEM;
		return (NULL);
		}
	else
		{
		if (_stop)
			_stop = _brk;
		return (obrk);
		}
	}
