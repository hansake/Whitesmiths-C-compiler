/*	EXPONENTIAL
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

#define NATLOG2		0.693147180559945309
#define RECIPLN2	1.442695040888963407
#define	SILLY		16000.0

LOCAL TEXT *exprange {"exp out of range"};

/*	e^x
 */
DOUBLE exp(x)
	DOUBLE x;
	{
	IMPORT DOUBLE _addexp(), _range();
	IMPORT TEXT *exprange;
	COUNT n;
	DOUBLE s;

	if (x <= -SILLY)
		return (0.0);
	else if (SILLY <= x)
		return (_range(exprange));
	else
		{
		n = _frac(&x, RECIPLN2);
		x =* NATLOG2;
		s = x * x;
		s = (30240.0 + s * (3360.0 + 30.0 * s)) / (15120.0 + s * (420.0 + s));
		return (_addexp((s + x) / (s - x), n, exprange));
		}
	}
