/*	GET AND PRINT EXPRESSIONS
 *
 */
#include <std.h>
#include "int012.h"
#include "int12.h"
#include "int2p.h"

/*	get the type and space info for expression and defs and refs
 */
BITS gettype(space, pspace, pl)
	BITS *space, *pspace;
	LONG *pl;
	{
	FAST BITS ty;

	*pl = 0L;
	*pspace = 0;
	*space = 0;
	ty = needch();
	if (ty & 0200)
		*space = glabel();
	ty &= 017;
	if (ty == XLPTRTO || ty == XPTRTO)
		*pspace = glabel();
	else if (ty == XSFIELD || ty == XLFIELD)
		{
		ty |= needch() << 4;
		ty |= needch() << 10;
		}
	else if (ty == XSTRUCT)
		gint(pl);
	return (ty);
	}

VOID printgexpr(ind)
	UCOUNT	ind;
	{
	FAST COUNT n;
	FAST TEXT *s;
	FAST METACH reg, refs;
	BITS ty;
	BITS pspace;
	BYTES space;
	DOUBLE d;
	LEX op;
	LONG bias, stsize;

	putfmt("%nx", ind);
	op = needch();
	ty = gettype(&space, &pspace, &stsize);
	if (op)
		n = 0;
	else
		{
		s = gname(NULL);
		n = lenstr(s);
		}
	printop(op); 
	printtype(ty & 017);
	if ((ty & 017) == XSFIELD || (ty & 017) == XLFIELD)
		{
		putfmt("off:%i, siz:%i ", (ty >> 4) & 0x3F, (ty >>10) & 0x3F);
		}
/*	else if ((ty & 017) == XPTRTO)
		{
		putfmt("object / %i ", 1 << (ty >> 4) );
		}
*/
	putfmt("obj / %i ", 1 << ((ty & 017) >> 4) );
	if (op == DADDR)
		{
		putch('\n');
		printgexpr(ind + 2);
		return;
		}
	else if (op)
		{
		putch('\n');
		printgexpr(ind + 2);
		if (op == LQUERY)
			printgexpr(ind + 2);
		if (op & BINOP)
			printgexpr(ind + 2);
		}
	else
		{
		gint(&bias);
		reg = needch();
		refs = needch();
		cpybuf(&d, s, 8);
		if ((ty & 017) == XDOUBLE && !refs && !reg)
			{
			putfmt("=D'%d';\n", d);
			}
		else
			{
			putfmt("'%p'+%l", s, bias);
			printregister(reg);
			putfmt(" refs=%o\n", refs);
			}
		}
	}
