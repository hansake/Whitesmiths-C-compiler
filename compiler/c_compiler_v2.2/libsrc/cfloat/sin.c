/*	SINE AND COSINE
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

#define TWODIVPI	0.636619772367581343

GLOBAL DOUBLE _pi {3.141592653589793239};

/*	weighted cos coefficients
 */
LOCAL DOUBLE costab[] {
	 0.000000465529978136,
	-0.000025200135509163,
	 0.000919259950106374,
	-0.020863480734954598,
	 0.253669507899865187,
	-1.233700550136151351,
	 0.999999999999999953
	};

/*	weighted sin coefficients
 */
LOCAL DOUBLE sintab[] {
	 0.000000056294725928,
	-0.000003598608108978,
	 0.000160441139884780,
	-0.004681754130726379,
	 0.079692626245925949,
	-0.645964097506240673,
	 1.570796326794896582
	};

/*	find folded taylor expansion
 */
DOUBLE _sin(x, quad)
	DOUBLE x;
	FAST COUNT quad;
	{
	IMPORT DOUBLE _poly();
	DOUBLE y;

	quad =+ _frac(&x, TWODIVPI);
	y = x * x;
	if (quad & 01)
		y = _poly(y, costab, 6);
	else
		y = x * _poly(y, sintab, 6);
	return ((quad & 02) ? -y : y);
	}

/*	cosine for radian argument
 */
DOUBLE cos(x)
	DOUBLE x;
	{
	return (_sin(x < 0 ? -x : x, 1));
	}

/*	sine for radian argument
 */
DOUBLE sin(x)
	DOUBLE x;
	{
	if (x < 0)
		return (_sin(-x, 2));
	else
		return (_sin(x, 0));
	}
