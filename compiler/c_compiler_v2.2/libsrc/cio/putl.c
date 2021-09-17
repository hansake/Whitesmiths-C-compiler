/*	BUFFERED OUTPUT TO I/O BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES putl(pf, s, n)
	FAST FIO *pf;
	FAST TEXT *s;
	BYTES n;
	{
	IMPORT TEXT *fioerr;
	FAST COUNT i;
	TEXT *q;

	if (!pf)
		_raise(NULL, &fioerr);
	else
		{
		for (i = n; 0 < i; )
			{
			for (q = &pf->_buf[pf->_nleft];
				0 < i && pf->_nleft < BUFSIZE; --i, ++pf->_nleft)
				*q++ = *s++;
			if (0 < i)
				{
				fwrite(pf->_fd, pf->_buf, BUFSIZE);
				pf->_nleft = 0;
				}
			}
		if (pf->_fmode != BWRITE && pf->_nleft
				&& pf->_buf[pf->_nleft - 1] == '\n')
			{
			fwrite(pf->_fd, pf->_buf, pf->_nleft);
			pf->_nleft = 0;
			}
		return (n);
		}
	}
