/*	FORMATTED PUT TO INTERNAL BUFFER (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

#define FMTSIZE 128
#define PADSIZE 32767

LOCAL BYTES actual {0};
LOCAL TEXT *where {0};

/*	string output routine
 */
LOCAL COUNT putx7(junk, s, n)
	TEXT *junk;
	FAST TEXT *s;
	BYTES n;
	{
	FAST COUNT i;

	for (i = n; 0 < i; --i)
		{
		*where++ = *s++;
		++actual;
		}
	return (n);
	}

/*	formatted output to buffer
 */
VOID sprintf(s, f, args)
	TEXT *s, *f, *args;
	{
	TEXT wsfmt[FMTSIZE];

	actual = 0;
	where = s;
	if (fmtcvt(f, wsfmt))
		_putf(&putx7, 0, wsfmt, &args);
	s[actual] = '\0';
	}

#ifdef TRYMAIN
main()
	{
	TEXT out[3][40];

	sprintf(out[0], "hello, world\n");
	sprintf(out[1], "%03o %-20.10s %8.4f\n", 0777, "left-adj!  or not?", 3.14159);
	printf("%s%s", out[0], out[1]);
	}
#endif
