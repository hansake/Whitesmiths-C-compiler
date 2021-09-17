/*	PUT BACK A CHARACTER (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>
#include <stdio.h>

METACH ungetc(c, fp)
	TEXT c;
	FILE *fp;
	{
	if (c < 0)
		return (c);
	if (fp->_pnext > &fp->_buf)
		{
		++fp->_nleft;
		*--fp->_pnext = c;
		return (c);
		}
	else
		return (EOF);
	}

#ifdef TRYMAIN
main()
	{
	TEXT c;

	c = getchar();
	ungetc('x', stdin);
	if (getchar() != 'x')
		printf("no good\n");
	exit(1);
	}
#endif
