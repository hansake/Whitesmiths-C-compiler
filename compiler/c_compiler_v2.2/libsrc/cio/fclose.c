/*	CLOSE WITH FILE I/O STRUCTURE
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

FIO *fclose(pf)
	FAST FIO *pf;
	{
	IMPORT FIO *_pfio;
	IMPORT TEXT *fioerr, *writerr;
	FAST FIO *q, **qb;

	if (!pf)
		_raise(NULL, &fioerr);
	for (qb = &_pfio; q = *qb; qb = &q->_pnext)
		if (q == pf)
			{
			*qb = q->_pnext;
			break;
			}
	if ((pf->_fmode == WRITE || pf->_fmode == BWRITE) && pf->_nleft
		&& (write(pf->_fd, pf->_buf, pf->_nleft) != pf->_nleft))
		_raise(NULL, &writerr);
	if (0 <= close(pf->_fd))
		return (pf);
	else
		return (NULL);
	}
