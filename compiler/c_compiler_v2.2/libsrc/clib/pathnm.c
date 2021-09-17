/*	ADD TAIL OF n1 TO n2
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *pathnm(buf, n1, n2)
	TEXT *buf;
	TEXT *n1, *n2;
	{
	FAST COUNT i;
	FAST TEXT *s, *t;

	s = n2 + lenstr(n2) - 1;
#ifdef IDRIS
	t = "/";
	for (s = n1; s[i = instr(s, "/")]; s =+ i + 1)
		;
#else
	t = (s[0] == ':' || s[0] == ']') ? "" : "/";
	for (s = n1; s[i = instr(s, "/:]")]; s =+ i + 1)
		;
#endif
	cpystr(buf, n2, t, s, NULL);
	return (buf);
	}
