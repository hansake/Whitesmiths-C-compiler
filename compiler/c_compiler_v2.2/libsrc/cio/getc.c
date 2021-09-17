/*	GET A CHARACTER FROM FILE I/O BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT getc(pf)
	FAST FIO *pf;
	{
	IMPORT TEXT *fioerr, *readerr;

	if (!pf)
		_raise(NULL, &fioerr);
	if (pf->_nleft == 0)
		{
		pf->_nleft = read(pf->_fd, pf->_pnext = pf->_buf, BUFSIZE);
		if (pf->_nleft < 0)
			_raise(NULL, &readerr);
		}
	if (--pf->_nleft < 0)
		{
		pf->_nleft = -1;
		return (EOF);
		}
	else
		return (*pf->_pnext++ & BYTMASK);
	}
