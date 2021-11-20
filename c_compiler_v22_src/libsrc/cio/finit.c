/*	INITIALIZE FILE I/O STRUCTURE
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

GLOBAL FIO *_pfio {NULL};
LOCAL VOID *(*_fnext)() {NULL};

/*	flush all pending writes
 */
LOCAL VOID *_flush()
	{
	IMPORT FIO *_pfio;
	IMPORT VOID *(*_fnext)();
	FAST FIO *q;

	for (q = _pfio; q; q = q->_pnext)
		if (0 < q->_nleft)
			write(q->_fd, q->_buf, q->_nleft);
	return (_fnext);
	}

/*	initialize file buffer
 */
FIO *finit(pf, fd, mode)
	FAST FIO *pf;
	FILE fd;
	FAST COUNT mode;
	{
	IMPORT FIO *_pfio;
	IMPORT TEXT *fioerr;
	IMPORT VOID *(*_fnext)();
	IMPORT VOID *_flush();

	if (!pf)
		_raise(NULL, &fioerr);
	pf->_fd = fd;
	pf->_nleft = 0;
	pf->_fmode = (mode == WRITE && 0 <= lseek(fd, 0L, 1)) ? BWRITE : mode;
	if (mode == WRITE || mode == BWRITE)
		{
		pf->_pnext = _pfio;
		_pfio = pf;
		if (!_fnext)
			_fnext = onexit(&_flush);
		}
	return (pf);
	}
