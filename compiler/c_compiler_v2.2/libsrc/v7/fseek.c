/*	FSEEK (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID fseek(fp, offset, sense)
	FIO *fp;
	LONG offset;
	COUNT sense;
	{
	FILE fd;

	if (fp->_fmode == READ)
		{
		if (sense == 1)
			offset -= fp->_pnext - fp->_buf;
		lseek(fp->_fd, offset, sense);
		fp->_nleft = read(fp->_fd, fp->_buf, BUFSIZE);
		fp->_pnext = fp->_buf;
		}
	else
		{
		write(fp->_fd, fp->_buf, fp->_nleft);
		lseek(fp->_fd, offset, sense);
		fp->_nleft = 0;
		}
	}
