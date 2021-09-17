/*	FORMATTED GET FROM INTERNAL BUFFER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

#define SCRATCH struct x
SCRATCH
	{
	TEXT *text;
	BYTES size;
	};

/*	encode get routine
 */
LOCAL COUNT _ncodx(sp, s, n)
	FAST SCRATCH *sp;
	FAST TEXT *s;
	BYTES n;
	{
	FAST COUNT i;

	for (i = 0; 0 < sp->size && i < n; )
		{
		*s++ = *sp->text;
		--sp->size;
		++i;
		if (*sp->text++ == '\n')
			break;
		}
	return (i);
	}

/*	get from internal buffer
 */
COUNT encode(s, n, f, args)
	TEXT *s;
	BYTES n;
	TEXT *f, *args;
	{
	SCRATCH pad;

	pad.text = s;
	pad.size = n;
	return (_getf(&_ncodx, &pad, f, &args));
	}
