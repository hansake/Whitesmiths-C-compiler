/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - CONFIGURATION MODULE
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

/* user modifiable portion of XA8
 * modify any of defaults + then recompile + link with other objects
 */
#include	<std.h>

/* define UCM  = 040 if require upper and lower case mnenomics */
#define	UCM	040			/* upper & lower case mnenomics */
TINY	ucbit	={UCM};

/* flags controlled by command line arguments
 * change these to alter default conditions
 */

TEXT	*hd_file	={""};	/*  header file */
BOOL	fp_flag		={NO};	/* do not use Whitesmiths fp format	*/
BOOL	ef_flag		={YES};	/* abort pass 2 if errors ? 	*/
BOOL	ep_flag		={NO};	/* print errors elsewhere ?	*/
BOOL	gu_flag		={NO};	/* make all unknowns == externals	*/
BOOL	hd_flag		={0};	/* -1 = read given file */
BOOL	l_flag		={0};	/* 0 = none, -1 to 'i/p'.ls, +1 to stnd op */
BOOL	ms_flag		={YES};	/* issue welcome message etc	*/
BOOL	nm_flag		={NO};	/* append name table to o/p (simulator)	*/
BOOL	op_flag		={-1};	/* 0 = none, -1 to 'i/p'.ou, +1 to stnd op	*/
BOOL	px_flag		={NO};	/* processor given in cmnd line	*/
BOOL	rl_flag		={1};	/* relocatable rqrd ? -1 = v2.0, +1 = v2.1 format */
BOOL	sc_flag		={YES};	/* single assembly	*/
BOOL	sym_flag	={NO};	/* print symbol table ?	*/
COUNT	siglength	={8};	/* significant length in names	*/
