/*  REMOVE A FILE UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"

COUNT _remove(fullname)
	TEXT *fullname;
	{
	IMPORT DRVR *_getdrv();
	DRVR *drvr;

	TEXT filename[_NAMSIZ+1];

	if (!filename)
		{
		_errno = EBADNM;
		return (NFAIL);
		}
	if (!(drvr = _getdrv(fullname)))
		return (NFAIL);
	_filename(fullname, filename);
	return ((*drvr->remove)(filename));
	}
