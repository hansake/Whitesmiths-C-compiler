/*	ENTER AND LEAVE CONTROL REGIONS
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

LOCAL TEXT *entcond {"bad leave call"};
LOCAL BYTES entval {0};

/*	enter a control region
 */
BYTES enter(pfn, arg)
	BYTES (*pfn)(), arg;
	{
	IMPORT BYTES entval;
	IMPORT TEXT *entcond;

	if (_when(NULL, &entcond, NULL) < 0)
		leave((*pfn)(arg));
	return (entval);
	}

/*	leave a control region
 */
VOID leave(val)
	BYTES val;
	{
	IMPORT BYTES entval;
	IMPORT TEXT *entcond;

	entval = val;
	_raise(NULL, &entcond);
	}
