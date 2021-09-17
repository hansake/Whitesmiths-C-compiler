/*	FEOF, FILENO (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

#define EOFSTATE 1
#define MAXFILES 20

LOCAL TEXT feofvec[MAXFILES] {0};

BOOL feof(fp)
	FIO *fp;
	{
	return (feofvec[fp->_fd] & EOFSTATE);
	}

FILE fileno(fp)
	FIO *fp;
	{
	return (fp->_fd);
	}

VOID _seteof(fp, val)
	FIO *fp;
	BOOL val;
	{
	if (val == 0)
		feofvec[fp->_fd] &= ~EOFSTATE;
	else
		feofvec[fp->_fd] |= EOFSTATE;
	}
