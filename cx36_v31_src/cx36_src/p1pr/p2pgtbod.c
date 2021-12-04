/*	GET AND PRINT BODY OF FUNCTIONS
 *
 */
#include <std.h>
#include "int012.h"
#include "int12.h"
#include "int2p.h"

LEX printgtbody()
	{
	LEX	tok;
	COUNT index = 0 ;
	LONG lo;
	LEX lin, lb;
	INTERN TEXT *gops[] =
				{"GNOTEQ", "GISEQ", "GGEQ", "GGREAT", "GLEQ", "GLESS"};

	while ((tok = printgcode()) != GFUNC && tok != GFEND && tok != EOF && tok)
		switch (tok)
			{
		case GDBFUNC:
			putfmt("GDBFUNC %i\n", glabel());
			break;
		case GDBSTAT:
			putfmt("GDBSTAT %p\n", lbn(glabel()));
			break;
		case GAUTOFF:
			gint(&lo);
			putfmt("GAUTOFF %l\n", lo);
			break;
		case GREGS:
			putfmt("GREGS %o\n", needch());
			break;
		case GJUMP:
			putfmt("GJUMP %p\n", lbn(glabel()) );
			break;
		case GSWITCH:
			putfmt("GSWITCH %p\n", lbn(glabel()) );
			break;
		case GLSWITCH:
			putfmt("GLSWITCH %p\n", lbn(glabel()) );
			break;
		case	GSWEND:
			putfmt("GSWEND\n");
			break;
		case	GCASE:
		case	GLABEL:
			lb = glabel();
			lin= glabel();
			putfmt("%p %p\nlineno=%s\n",
				(tok == GCASE) ? "GCASE" : "GLABEL", lbn(lb), lin);
			break;
		case	GRET:
			putfmt("GRET\n");
			break;
		case	GVOID:
			putfmt("GVOID\n");
			printgexpr(0);
			break;
		case	GLESS:
			index++;
		case	GLEQ:
			index++;
		case	GGREAT:
			index++;
		case	GGEQ:
			index++;
		case	GISEQ:
			index++;
		case	GNOTEQ:
			putfmt("%p %p\n", gops[index], lbn(glabel()) );
			printgexpr(2);
			printgexpr(2);
			index = 0 ;
			break;
		case GSDBDATA:
			putfmt("GSDBDATA\n");
			break;
		case GEDBDATA:
			putfmt("GEDBDATA\n");
			break;
		default:
			panic("BAD INPUT TO p2pgtbody");
			}
	return(tok);
	}
