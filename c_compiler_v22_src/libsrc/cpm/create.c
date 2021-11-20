/*	CP/M CREATE A FILE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

COUNT create(name, mode, rsize)
	TEXT *name;
	COUNT mode, rsize;
	{
	return (_copen(name, MCREATE, rsize));
	}
