/*	COPY MULTIPLE STRINGS
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *cpystr(dest, argl)
	TEXT *dest, *argl;
	{
	FAST TEXT **ap, *q, *s;

	for (q = dest, ap = &argl; s = *ap; ++ap)
		while (*s)
			*q++ = *s++;
	*q = '\0';
	return (q);
	}
