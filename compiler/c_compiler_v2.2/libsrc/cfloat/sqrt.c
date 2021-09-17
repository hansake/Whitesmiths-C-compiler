/*	SQUARE ROOT
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

#define SQRT2	1.414213562373095049

/*	corrected newtonian approximation
 */
DOUBLE sqrt(x)
	DOUBLE x;
	{
	IMPORT DOUBLE _addexp();
	FAST COUNT n, i;
	DOUBLE y;

	if (x < 0)
		_domain("sqrt domain error");
	else if (x == 0)
		return (0.0);
	else
		{
		n = _unpack(&x);
		y = 0.32025 + x * (0.87204 - x * 0.19245);
		for (i = 3; 0 <= --i; )
			y = (x / y + y) * 0.5;
		if (n & 1)
			y =* SQRT2;
		return (_addexp(y, n >> 1, "sqrt!"));
		}
	}
