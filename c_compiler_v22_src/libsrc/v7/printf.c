/*	FORMATTED OUTPUT TO STDOUT (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

#define FMTSIZE 128

VOID printf(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT COUNT putl();
	IMPORT FIO stdout;
	TEXT wsfmt[FMTSIZE];

	if (fmtcvt(fmt, wsfmt))
		_putf(&putl, &stdout, wsfmt, &args);
	putch(-1);
	}

#ifdef TRYMAIN
main()
	{
	printf("hello, world\n");
	printf("%03o %-20.10s %8.4f\n", 0777, "left-adj!  or not?", 3.14159);
	}
#endif
