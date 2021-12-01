/*  GET SYSTEM TIME (DUMMY) UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */
#include <wslxa.h>
#include "ssp.h"

ULONG _time()
	{
	return ((ULONG) 0);
	}

