/*	GET FULL LINE INTO I/O BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES getl(pf, s, n)
	FAST FIO *pf;
	FAST TEXT *s;
	BYTES n;
	{
	IMPORT TEXT *fioerr, *readerr;
	FAST BYTES i;

	if (!pf)
		_raise(NULL, &fioerr);
	for (i = 0; ; )
		{
		if (pf->_nleft == 0)
			{
			pf->_nleft = read(pf->_fd, pf->_pnext = pf->_buf, BUFSIZE);
			if (pf->_nleft < 0)
				_raise(NULL, &readerr);
			}
		if (pf->_nleft <= 0)
			{
			pf->_nleft = -1;
			return (i);
			}
		for (; i < n && 0 < pf->_nleft; ++s)
			{
			++i;
			--pf->_nleft;
			*s = *pf->_pnext++;
			if (*s == '\n')
				return (i);
			}
		if (n <= i)
			return (i);
		}
	}
