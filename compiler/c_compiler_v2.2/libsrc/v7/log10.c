/*	RETURN LOG (BASE 10) OF x
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

DOUBLE log10(x)
	DOUBLE x;
	{
	DOUBLE ln();

	return (ln(x) / 2.3025850929940457);
	}

#ifdef TRYMAIN
main()
	{
	DOUBLE x;
	DOUBLE log10();

	while (1 == getfmt("%d", &x))
		putfmt("x=%26.20d, log10(x)=%26.20d\n", x, log10(x));
	}
#endif
