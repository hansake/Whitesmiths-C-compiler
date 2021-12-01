/*	GET AND PRINT DATA
 *
 */
#include <std.h>
#include "int012.h"
#include "int12.h"
#include "int2p.h"

/*	process switch table
 */
LEX doswtab()
	{
	BOOL islong;
	BYTES num;
	COUNT c, n;
	LABEL tab, def, lbl;
	LONG lower, val;

	n = needch();
	tab = glabel();
	def = glabel();
	num = glabel();
	islong = (glabel() == 4);
	FOREVER
		switch (c = printgcode())
			{
		case GJMPTAB:
			putfmt("GJMPTAB\n");
			num = glabel();
			gint(&lower);
			for ( ; num; --num)
					glabel();
			break;
		case GSCNTAB:
			putfmt("GSCNTAB\n");
			num = glabel();
			for ( ; num; --num)
				{
				gint(&val);
				lbl = glabel();
				}
			c = printgcode();
		default:
			return (c);
			}
	}
		
/*	write data until a function
 */
LEX printgcode()
	{
	FAST LEX c, x;
	
	BITS pspace;
	BYTES psize;
	COUNT i, lineno, n;
	DOUBLE dflt;
	LONG lo;
	TEXT name[LENNAME+2], *s;
	TEXT txtbuf[80];
	
	FOREVER
		switch (c = getch())
			{
		case LLINENO:
			lineno = glabel();
			putfmt("LLINENO %i\n", lineno);
			break;
		case LIFILE:
			gname(txtbuf);
			putfmt("LIFILE %p\n", txtbuf);
			break;
		case LERROR:
			for (n = glabel(), s = txtbuf; n; --n, *s++ = needch())
				;
			*s = '\0';
			putfmt("LERROR %p\n", txtbuf);
			break;
		case GSPNAME:
			gname(name);
			putfmt("GSPNAME %p\n", name);
			break;
		case GFADDR:
		case GFADDR1:
		case GFADDR2:
		case GFADDR3:
		case GFADDR4:
			gname(name);
			gint(&lo);
			putfmt("GFADDR%i %p %l\n", c - GFADDR, name, lo);
			break;
		case GADDR:
		case GADDR1:
		case GADDR2:
		case GADDR3:
		case GADDR4:
			gname(name);
			gint(&lo);
			putfmt("GADDR%i %p %l\n", c - GADDR, name, lo);
			break;
		case GDEF:
			gname(name);
			pspace = glabel();
			gint(&lo);
			psize = lo;
			putfmt("GDEF %p pspace: 0x%h, psize: %i\n", name, pspace, psize);
			break;
		case GREF:
			gname(name);
			pspace = glabel();
			gint(&lo);
			psize = lo;
			putfmt("GREF %p pspace: 0x%h, psize: %i\n", name, pspace, psize);
			break;
		case GDATA:
			n = needch();
			gname(name);
			putfmt("GDATA bound: %i %p\n", n, name);
			break;
		case GFDATA:
			n = needch();
			pspace = glabel();
			gname(name);
			putfmt("GFDATA bound: %i %p pspace: 0x%h\n", n, name, pspace);
			break;
		case GLITS:
			n = needch();
			pspace = glabel();
			gname(name);
			putfmt("GLITS bound: %i %p pspace: 0x%h\n", n, name, pspace);
			break;
		case GSWTAB:
			putfmt("GSWTAB\n");
			return(doswtab());
			break;
		case GINT:
			n = needch();
			gint(&lo);
			putfmt("GINT %i %l\n", n, lo);
			break;
		case GFLOAT:
			n = needch();
			for (i = 0; i < 8; ++i)
				name[i] = needch();
			cpybuf(&dflt, name, 8);
			putfmt("GFLOAT %i %20.16d\n", n, dflt);
			break;
		case GSTRING:
			n = needch();
			putfmt("GSTRING %i \"", n);
			while (0 < n)
				{
				x = needch();
				switch (x)
					{
					case '\n':
						putfmt("\\n");
						break;
					case '\r':
						putfmt("\\r");
						break;
					default:
						if (x < '\040')
							putfmt("\\%oc", x);
						else
							putfmt("%ac", x);
					}
				n--;
				}
			putfmt("\"\n");
			break;
		case GDEND:
			putfmt("GDEND\n");
			break;
		case GSPACE:
			gint(&lo);
			putfmt("GSPACE %l\n", lo);
			break;
		default:
			return (c) ;
			}
	}
