/*  CAPTURE SIGNALS UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */
#include <wslxa.h>
#include "ssp.h"

VOID (*_signal(sig, func))()
	INT sig;
	VOID (*func)();
	{
	return (0);
	}
