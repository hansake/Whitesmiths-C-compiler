/*	OUTPUT CHAR TO FILE I/O BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

COUNT putc(pf, c)
	FAST FIO *pf;
	FAST COUNT c;
	{
	IMPORT TEXT *fioerr;

	if (!pf)
		_raise(NULL, &fioerr);
	if (0 <= c)
		{
		if (pf->_nleft < BUFSIZE)
			pf->_buf[pf->_nleft++] = c;
		else
			{
			fwrite(pf->_fd, pf->_buf, pf->_nleft);
			pf->_nleft = 1;
			pf->_buf[0] = c;
			}
		if (c != '\n' || pf->_fmode == BWRITE)
			return (c);
		}
	if (pf->_nleft <= 0)
		return (c);
	else
		{
		fwrite(pf->_fd, pf->_buf, pf->_nleft);
		pf->_nleft = 0;
		return (c);
		}
	}
