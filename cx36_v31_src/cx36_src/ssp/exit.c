/*  TERMINATE PROGRAM EXECUTION UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"

LOCAL FNPTR __onexit = (FNPTR)0;

VOID _exit(status)
	INT status;		/* dummy - meaningless under system/36 */
	{
	_svc(SSP_EOJ);
	}

/*  register function to call on exit
 */
FNPTR _onexit(pfn)
	FNPTR pfn;
	{
	IMPORT FNPTR __onexit;
	LOCAL BOOL called = {NO};
	FNPTR p;

	if (called == NO)
		{
		__onexit = (FNPTR)_onexit;
		called = YES;
		}
	p = __onexit, __onexit = pfn;
	return (p);
	}

/*  cleanup before exit
 */
VOID _terminate(status)
    INT status;
    {
	IMPORT FNPTR __onexit, _onexit();

	while (__onexit && __onexit != (FNPTR)_onexit)
		__onexit = (FNPTR) (*__onexit)();
	_exit(status);
	}
