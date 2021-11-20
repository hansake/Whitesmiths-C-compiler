/*	CONVERT STRING TO DOUBLE
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

DOUBLE atof(s)
	TEXT *s;
	{
	DOUBLE dnum;

	btod(s, BUFSIZE, &dnum);
	return (dnum);
	}

#ifdef TRYMAIN
main()
	{
	DOUBLE atof();

	putfmt("0 = %.8d, 12345E6 = %.8d, 99999999E-9 = %.8d\n",
		atof("0"), atof("12345E6"), atof("   99999999E-9xx"));
	}
#endif
