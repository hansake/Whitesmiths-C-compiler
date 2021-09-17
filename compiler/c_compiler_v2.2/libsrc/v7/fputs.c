/*	PUT STRING TO FILE (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

VOID fputs(s, fp)
	TEXT *s;
	FIO *fp;
	{
	fprintf(fp, "%s", s);
	}

#ifdef TRYMAIN
main()
	{
	IMPORT FIO stdout;

	fputs("testing 123\n", &stdout);
	}
#endif
