/*	P1PR SUBROUTINES
 *
 */
#include <std.h>
#include "int012.h"
#include "int12.h"
#include "int2p.h"

/* name byte table */

TEXT *nmbtab[] = {NULL, "NMDAT: ", "NMLIT: ", "NMBSS: ",
		"NMFUN: ", "NMFRD: ", "NMFRS: ", NULL};

/*	get an integer
 */
VOID gint(s)
	FAST TEXT *s;
	{
	FAST BYTES i;

	for (i = 0; i < 4; ++i)
		*s++ = needch();
	}

/*	get a label
 */
LABEL glabel()
	{
	FAST TEXT *p;
	LABEL l;

	p = &l;
	*p++ = needch();
	*p = needch();
	return (l);
	}

/*	get a name
 */
TEXT *gname(s)
	TEXT *s;
	{
	FAST BYTES i, n;
	FAST TEXT *ss, *dbptr;
	INTERN TEXT nm[LENNAME + 8];

	nm[0] = '\0';
	n = needch();
	if (!s)
		s = nm;
	ss = s;
	if (i = glabel())
		{
		for (*ss++ = '>'; i; ++ss)
			{
			*ss = (i & 07) + '0';
			i >>= 3;
			}
		*ss = '\0';
		}
	if (n)
		{
		n -= 1;
		i = needch();
		if (i & NMFAR)
			ss = cpystr(ss, "NMFAR: ", NULL);
		if (i & NMVAL)
			ss = cpystr(ss, nmbtab[i & 07], NULL);
		*ss++ = '<';
		for ( ; n; --n)
			{
			*ss++ = needch();
			}
		*ss++ = '\0';
		}
	return (s);
	}

/*	make a name from label
 */
TEXT *lbn(label)
	FAST LABEL label;
	{
	FAST TEXT *s;
	INTERN TEXT name[LENNAME+1];

	name[0] = '>';
	for (s = name + 1; label; ++s)
		{
		*s = (label & 07) + '0';
		label =>> 3;
		}
	*s = '\0';
	return (name);
	}

/*	need a character
 */
LEX needch()
	{
	FAST LEX c;

	if ((c = getch()) == EOF)
		{
		panic("EOF");
		exit(NO);
		}
	else
		return (c);
	}

/*	put a panic error message
 */
VOID panic(s)
	TEXT *s;
	{
	IMPORT FILE errfd;

	putch(-1);
	putstr(errfd, "PANIC ", s, "!\n", NULL);
	exit(NO);
	}
