/*	CONVERT BUFFER TO LONG
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

BYTES btol(s, n, l, base)
	FAST TEXT *s;
	FAST BYTES n;
	LONG *l;
	COUNT base;
	{
	FAST COUNT dig;
	BOOL minus;
	LONG val;
	TEXT *is;

	val = 0;
	is = s;
	minus = NO;
	for(; 0 < n && iswhite(*s); --n, ++s)
		;
	if (0 < n)
		if (*s == '-')
			{
			minus = YES;
			++s;
			--n;
			}
		else if (*s == '+')
			{
			++s;
			--n;
			}
	if (n <= 0)
			;
	else if (base == 1)
		{
		if (*s != '0')
			base = 10;
		else if (1 < n && tolower(s[1]) == 'x')
			{
			base = 16;
			s =+ 2;
			n =- 2;
			}
		else
			base = 8;
		}
	else if (base == 16 && 2 <= n)
		if (*s == '0' && tolower(s[1]) == 'x')
			{
			s =+ 2;
			n =- 2;
			}
	for (; 0 < n; --n, ++s)
		{
		if (isdigit(*s))
			dig = *s - '0';
		else if (isalpha(*s))
			{
			dig = tolower(*s) - ('a' - 10);
			if (base <= dig)
				break;
			}
		else
			break;
		val = val * base + dig;
		}
	*l = ((minus) ? -val : val);
	if (0 < n && tolower(*s) == 'l')
		++s;
	return (s - is);
	}
