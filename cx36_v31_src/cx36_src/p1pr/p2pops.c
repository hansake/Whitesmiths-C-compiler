/*	PRINT OPERATORS
 *
 */
#include <std.h>
#include "int012.h"
#include "int12.h"
#include "int2p.h"

#define TABLE struct table

struct table {
	short incode;
	TEXT *text;
	} ;

TABLE regsters[] = {
	ARGIDX,"ARGIDX",
	AUTIDX,"AUTIDX",
	INTRET,"INTRET",
	LONGRET,"LONGRET",
	FLTRET,"FLTRET",
	R0,"R0",
	R1,"R1",
	R2,"FST1",
	R3,"FST2",
	R4,"FST3",
	-1, NULL } ;

TABLE operators[] = {
	LAND,"LAND",
	LANDAND,"LANDAND",
	LCOMP,"LCOMP",
	LDECR,"LDECR",
	LDIVIDE,"LDIVIDE",
	LGAND,"LGAND",
	LGDIV,"LGDIV",
	LGETS,"LGETS",
	LGLSH,"LGLSH",
	LGRSH,"LGRSH",
	LGMIN,"LGMIN",
	LGMOD,"LGMOD",
	LGOR,"LGOR",
	LGPLU,"LGPLU",
	LGREAT,"LGREAT",
	LGEQ,"LGEQ",
	LGTIM,"LGTIM",
	LGXOR,"LGXOR",
	LINCR,"LINCR",
	LISEQ,"LISEQ",
	LLESS,"LLESS",
	LLEQ,"LLEQ",
	LLSHIFT,"LLSHIFT",
	LMINUS,"MINUS",
	LMODULO,"LMODULO",
	LNOT,"LNOT",
	LNOTEQ,"LNOTEQ",
	LOR,"LOR",
	LOROR,"LOROR",
	LPLUS,"LPLUS",
	LQUERY,"LQUERY",
	LRSHIFT,"LRSHIFT",
	LTIMES,"LTIMES",
	LXOR,"LXOR",
	DADDR,"DADDR",
	DALIST,"DALIST",
	DCAST,"DCAST",
	DGMIN,"DGMIN",
	DELIST,"DELIST",
	DFNCALL,"DFNCALL",
	DFNONLY,"DFNONLY",
	DGPLU,"DGPLU",
	DINDIR,"DINDIR",
	DMINUS,"DMINUS",
	DPLUS,"DPLUS",
	DCMP,"DCMP",
	DGETS,"DGETS",
	DLESS,"DLESS",
	0,"TERM",
	-1,NULL } ;

TABLE types[] = {
	XSTRUCT,"XSTRUCT",
	XLPTRTO,"XLPTRTO",
	XPTRTO,"XPTRTO",
	XDOUBLE,"XDOUBLE",
	XFLOAT,"XFLOAT",
	XULONG,"XULONG",
	XLFIELD,"XFIELD",
	XLONG,"XLONG",
	XUSHORT,"XUSHORT",
	XSFIELD,"XSFIELD",
	XSHORT,"XSHORT",
	XUCHAR,"XUCHAR",
	XCHAR,"XCHAR",
	XTEST,"XTEST",
	-1,NULL } ;

/*	print op's
 */
VOID printop(op)
	LEX	op;
	{
	FAST COUNT i;
	/*IMPORT TABLE operators[];*/

	for (i = 0; operators[i].incode != -1; i++)
		if (op == operators[i].incode)
			{
			putfmt("%p ",operators[i].text);
			break;
			}
	if (operators[i].incode == -1 )
		putfmt("op=%o ",op);
	}

VOID printregister(n)
	LEX	n;
	{
	FAST COUNT i;
	/*IMPORT TABLE regsters[];*/
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
	/*IMPORT TABLE types[];*/
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
