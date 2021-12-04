/*	THIS IS THE MAIN PRINTING FUNCTION
 *
 */
#include <std.h>
#include "int012.h"
#include "int12.h"
#include "int2p.h"

/*
 * File control
 */
GLOBAL FILE errfd = {STDERR};
GLOBAL FILE outfd = {STDOUT};

TEXT *_pname = {"p1pr"};

LOCAL TEXT *ofile = {NULL};

/*	print output of p1
 */
BOOL main(ac, av)
	COUNT ac;
	TEXT **av;
	{
	BITS pspace;
	BYTES psize;
	LEX tok;
	LONG lo;
	TEXT funname[LENNAME + 2];
	UTINY dcbyte, magbyte, regbyte;

	getflags(&ac, &av, "o*:F <file>", &ofile);
	if (0 < ac)
		{
		close(STDIN);
		if (open(av[0],READ, 1) != STDIN)
			panic("bad input file");
		}
	if (ofile)
		if ((outfd = create(ofile, WRITE, 0)) < 0)
			panic("bad output file");
		else
			errfd = STDOUT;

	/* Here we start */

	magbyte = needch();
	putfmt("magic byte: 0x%h\n", magbyte);
	if (magbyte != P2MAGIC)
		error("intermediate file < release 3.0 ", av[0]);
	dcbyte = needch();
	putfmt("dcbyte: 0x%h\n", dcbyte);
	regbyte = needch();
	putfmt("regbyte: 0x%h\n", regbyte);
	for (tok = printgcode(); tok != EOF && tok ;)
		{
		if (tok != GFUNC)
			panic("NO GFUNC");
		gname(funname);
		pspace = glabel();
		gint(&lo);
		psize = lo;
		putfmt("GFUNC %p pspace: 0x%h, psize: %i\n", funname, pspace, psize);
		tok = printgtbody();
		}
	putch(-1);
	return(YES);
	}
