/*	NATURAL LOG
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

LOCAL DOUBLE hafrad2 {0.707106781186547524};
LOCAL DOUBLE natlog2 {0.693147180559945309};

/*	weighted coefficients for log
 */
LOCAL DOUBLE logtab[] {
	0.244439303693324742,
	0.261296855188113503,
	0.320621846983999132,
	0.412198305248819107,
	0.577078018079458110,
	0.961796693921264866,
	2.885390081777930536
	};

/*	weights for ln(x) near 1.0
 */
LOCAL DOUBLE smalltab[] {
	 0.142857142857142857,
	-0.166688541666666667,
	 0.200000000000000000,
	-0.249999998906250000,
	 0.333333333333333333,
	-0.500000000000013672,
	 1.000000000000000000
	};

/*	evaluate log
 */
DOUBLE ln(x)
	DOUBLE x;
	{
	IMPORT DOUBLE _poly();
	COUNT n;

	if (x <= 0)
		_domain("ln domain error");
	else if (0.98 <= x && x <= 1.01)
		{
		x =- 1.0;
		return (x * _poly(x, smalltab, 6));
		}
	else
		{
		n = _unpack(&x);
		x = (x - hafrad2) / (x + hafrad2);
		return (natlog2 * (n - 0.5 + x * _poly(x * x, logtab, 6)));
		}
	}
