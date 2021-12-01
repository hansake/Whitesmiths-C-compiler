/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - DUMMY FLOATING POINT
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include	<std.h>
LOCAL	TEXT	v[]	={"@(#)nofp	1.0	1:May:84"};

struct	mantissa	{
	UTINY	man[8];
	};
typedef	struct	mantissa	XMAN;

UTINY	addl(refd,refa)
	XMAN	*refd,*refa;
{
	perror(1, "S E floating point not implemented");
}
	
VOID	strfpt(b,s,frmt)
	UTINY	*b;
	TEXT	*s;
	COUNT	frmt;
{
	perror(1, "S E floating point not implemented");
}
