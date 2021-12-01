/*	PRINT OPERATORS
 *
 */
#include <std.h>
#include "int012.h"
#include "int12.h"
#include "int2p.h"
#include "p2p.h"

/*	print op's
 */
VOID printop(op)
	LEX	op;
	{
	FAST COUNT i;
	IMPORT TABLE operators[];

	for (i = 0; operators[i].incode != -1; i++)
		if (op == operators[i].incode)
			{
			putfmt("%p ",operators[i].text);
			break;
			}
	if (operators[i].incode == -1 )
		putfmt("op=%o ",op);
	}

VOID printregisters(n)
	LEX	n;
	{
	FAST COUNT i;
	IMPORT TABLE regsters[];
	for (i = 0; regsters[i].incode !=NULL; i++)
		if (n == regsters[i].incode)
			{
			putfmt("[%p]",regsters[i].text);
			break;
			}
	if (regsters[i].incode == NULL )
		putfmt("[%o]",n);
	}

VOID printtype(ty)
	BITS ty;
	{
	IMPORT TABLE types[];
	FAST COUNT i;

	for (i = 0; types[i].incode != -1; i++)
		if (types[i].incode == ty)
			{
			putfmt("%p ",types[i].text);
			break;
			}
	if (types[i].incode == -1)
		putfmt("ty=%o ",ty);
	}
