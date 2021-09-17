/*	CP/M REMOVE
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

COUNT remove(name)
	TEXT *name;
	{
	return (_copen(name, MREMOVE, 0));
	}
