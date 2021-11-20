/*	FOPEN (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

GLOBAL COUNT _recsize {0};

FIO *fopen7(name, mode)
	TEXT *name, *mode;
	{
	FIO *fp, *fp2;
	FILE fd;

	fp = alloc(1 + sizeof (FIO));
	if (*mode == 'r')
		{
		fd = open(name, READ, _recsize);
		if (0 <= fd)
			fp2 = finit(fp, fd, READ);
		else
			fp2 = NULL;
		}
	else if (*mode == 'a')
		{
		fd = open(name, WRITE, _recsize);
		if (0 <= fd)
			{
			lseek(fd, 0L, 2);
			fp2 = finit(fp, fd, BWRITE);
			}
		else if (0 <= (fd = create(name, WRITE, _recsize)))
			fp2 = finit(fp, fd, BWRITE);
		else
			fp2 = NULL;
		}
	else 
		{
		fd = create(name, WRITE, _recsize);
		if (0 <= fd)
			fp2 = finit(fp, fd, BWRITE);
		else
			fp2 = NULL;
		}
	if (fp2 == NULL)
		free(fp);
	return (fp2);
	}
