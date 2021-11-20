/*	CP/M CONTROL BLOCKS
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "cpm.h"

#define NFILES	16

GLOBAL BYTES _nfiles {NFILES};

LOCAL FCB stdin {0};

GLOBAL WCB _wcbs[NFILES] {
	{WCR|WSTD|WOPEN, CON, &stdin},
	{WCR|WOPEN, CON, 0},
	{WCR|WOPEN, CON, 0},
	};
