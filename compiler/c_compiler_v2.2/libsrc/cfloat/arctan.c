/*	ARC TANGENT
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */
#include <std.h>

#define PIOVER2	1.570796326794896619

/*	1/8 intervals of atan(x)
 */
LOCAL DOUBLE atantab[] {
	0.0,
	0.124354994546761435,
	0.244978663126864154,
	0.358770670270572220,
	0.463647609000806116,
	0.558599315343562436,
	0.643501108793284387,
	0.718829999621624505,
	0.785398163397448310
	};

/*	weighted atan coefficients
 */
LOCAL DOUBLE coeftab[] {
	-0.089932528409257986,
	 0.111106342739529896,
	-0.142857131681271964,
	 0.199999999987267074,
	-0.333333333333327116,
	 0.999999999999999999
	};

/*	interpolate and correct error
 */
DOUBLE arctan(x)
	DOUBLE x;
	{
	IMPORT DOUBLE atantab[], coeftab[], _poly();
	FAST UCOUNT i;
	DOUBLE z;

	if (x < 0)
		return (-arctan(-x));
	else if (x <= 1.0)
		{
		i = 16.0 * x;
		i = i + 1 >> 1;
		z = i * 0.125;
		x = (x - z) / (1.0 + x * z);
		return (atantab[i] + x * _poly(x * x, coeftab, 5));
		}
	else
		{
		x = arctan(1.0 / x);
		return (PIOVER2 - x);
		}
	}
