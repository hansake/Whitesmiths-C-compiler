/*	CP/M INTERRUPT HANDLER
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

COUNT _intch {'\177'};		/* DEL key */
LOCAL VOID (*_onintr)() {onintr};

/*	interrupt the program
 */
VOID _intr()
	{
	IMPORT VOID (*_onintr)();

	if (_onintr)
		(*_onintr)(0);
	exit(NO);
	}

/*	register function to call on intr
 */
VOID *onintr(new)
	VOID (*new)();
	{
	IMPORT VOID _intr(), (*_onintr)();
	FAST VOID (*p)();

	if (new)
		{
		_setint(&_intr);
		_intch = '\177';
		}
	else
		{
		_setint(NULL);
		_intch = -1;
		}
	p = _onintr, _onintr = new;
	return (p);
	}
