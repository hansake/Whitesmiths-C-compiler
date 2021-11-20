/*	FORMATTED PUT TO INTERNAL BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

#define SCRATCH struct x
SCRATCH
	{
	TEXT *text;
	BYTES size, actual;
	};

/*	decode put routine
 */
LOCAL COUNT _dcodx(sp, s, n)
	FAST SCRATCH *sp;
	FAST TEXT *s;
	BYTES n;
	{
	FAST COUNT i;

	for (i = n; 0 < i && sp->actual < sp->size; --i)
		{
		*sp->text++ = *s++;
		++sp->actual;
		}
	return (n);
	}

/*	put to internal buffer
 */
BYTES decode(s, n, f, args)
	TEXT *s;
	BYTES n;
	TEXT *f, *args;
	{
	SCRATCH pad;

	pad.text = s;
	pad.size = n;
	pad.actual = 0;
	_putf(&_dcodx, &pad, f, &args);
	return (pad.actual);
	}
