/*	RETURN x TO THE y POWER (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

DOUBLE pow(x, y)
	DOUBLE x, y;
	{
	DOUBLE exp(), ln();

	if (y == 0.)
		{
		if (x == 0.)
			error("pow: x and y both 0", "");
		else
			return (1.);
		}
	else if (x < 0.)
		{
		if ((LONG)y != y)
			error("pow: negative x with non-integer y", "");
		else
			return ((LONG)y % 2 ? -exp(ln(-x) * y) : exp(ln(-x) * y));
		}
	else
		return (exp(ln(x) * y));
	}

#ifdef TRYMAIN
main()
	{
	DOUBLE x, y, pow();

	while (2 == getfmt("%d%d", &x, &y))
		putfmt("x=%10.4d, y=%10.4d, x**y=%10.4d\n", x, y, pow(x, y));
	}
#endif
