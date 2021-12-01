/*  EXECUTE A COMMAND (DUMMY) UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */
#include <wslxa.h>
#include "ssp.h"

COUNT _system(cmd)
	TEXT *cmd;
	{
	_errno = ENOTSUP;
	return (NFAIL);
	}
