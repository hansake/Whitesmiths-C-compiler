/*	CP/M EXIT
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

LOCAL VOID (*_onexit)() {onexit};

/*	exit to system
 */
VOID exit(stat)
	COUNT stat;
	{
	IMPORT VOID (*_onexit)(), *onexit();

	while (_onexit && _onexit != &onexit)
		_onexit = (*_onexit)();
	FOREVER
		_svc(CRESET, 0);
	}

/*	register function to call on exit
 */
VOID *onexit(new)
	VOID (*new)();
	{
	IMPORT VOID (*_onexit)();
	FAST VOID (*p)();

	p = _onexit, _onexit = new;
	return (p);
	}
