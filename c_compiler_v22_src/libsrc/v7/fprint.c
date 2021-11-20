/*	FORMATTED OUTPUT TO FILE fp (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

#define FMTSIZE 128

VOID fprintf(fp, fmt, args)
	FIO *fp;
	TEXT *fmt, *args;
	{
	IMPORT COUNT putl();
	TEXT wsfmt[FMTSIZE];

	if (fmtcvt(fmt, wsfmt))
		_putf(&putl, fp, wsfmt, &args);
	putch(-1);
	}

#ifdef TRYMAIN
main()
	{
	IMPORT FIO stdout;

	fprintf(&stdout, "hello, world\n");
	fprintf(&stdout, "%03o %-20.10s %8.4f\n", 0777, "left-adj!  or not?", 3.14159);
	}
#endif
