/*	UNPACK IBM/370, IBM S/36 FLOATING POINT
 *	copyright (c) 1983, 1986 by Whitesmiths, Ltd.
 */
#include <wslxa.h>

/*	_unpack returns the power of two exponent of the double at *pd as
 *	the value of the function, and writes the value at *pd.
 */
COUNT _unpack(pd)
	FAST TEXT *pd;
	{
	FAST COUNT i, t;
	/*	pd is assumed to be normalized on entry	*/
	
	if (!pd[0] && !pd[1])
		return (0);
	t = ((pd[0] & 0x7F) - 64) << 2;
	for (; !(pd[1] & 0x80); --t)
		{
		for (i = 1; i < 7; i++)
			pd[i] = (pd[i] << 1) | ((pd[i + 1] & BYTMASK) >> 7) ;
		pd[i] <<= 1;
		}
	pd[0] &= 0x80;
	pd[0] |= 0x40;
	return (t);
	}
