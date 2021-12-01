/*  CHANGE OLD FILE TO NEW FILE (DUMMY) UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */
#include <wslxa.h>
#include "ssp.h"

COUNT _rename(old, new)
	TEXT *old, *new;
	{
	_errno = ENOTSUP;
	return (NFAIL);
	}
