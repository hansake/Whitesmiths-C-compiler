/*	EXIT (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID exit7(cond)
	COUNT cond;
	{
	exit(cond == 0);
	}
