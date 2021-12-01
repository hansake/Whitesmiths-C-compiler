/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - DUMMY SDI
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include <std.h>
#include "xa.h"

VOID enter_sdi(t, p, o, r)
	TINY t, p;
	LVAL o;
	struct result *r;

	{
	perror(1, "S E sdis not implemented");
	}

VOID sdi_1zp(t, p, o, r)
	TINY t, p;
	LVAL o;
	struct result *r;

	{
	perror(1, "S E sdis not implemented");
	}

VOID do_sdi_list()
	{
	}

VOID set_sdinum(i, val)
	COUNT i, val;
	{
	}

