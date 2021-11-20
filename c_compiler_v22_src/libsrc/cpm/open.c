/*	CP/M OPEN A FILE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

COUNT open(name, mode, rsize)
	TEXT *name;
	COUNT mode, rsize;
	{
	return (_copen(name, mode ? MOPEN|MWRITE : MOPEN, rsize));
	}
