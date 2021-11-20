/*	ECHO ARGUMENTS TO STDOUT
 *	copyright (c) 1980, 1985 by Whitesmiths, Ltd.
 */
#include <wslxa.h>

/*	flags:
	-m		output newline between arguments
	-n		do not put newline at end of arguments
 */
BOOL mflag = {NO};
BOOL nflag = {NO};

TEXT *_pname = {"echo"};

/*	output args separated by space or newline
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL mflag, nflag;
	FAST COUNT n, ns, nw;
	TEXT *between;

	getflags(&ac, &av, "m,n:F <args>", &mflag, &nflag);
	if (!ac)
		return (YES);
	between = mflag ? "\n" : " ";
	for (nw = 0, ns = 0; ac; --ac, ++av)
		{
		if (nw)
			{
			nw += write(STDOUT, between, 1);
			++ns;
			}
		nw += write(STDOUT, *av, n = lenstr(*av));
		ns += n;
		}
	if (!nflag)
		{
		nw += write(STDOUT, "\n", 1);
		++ns;
		}
	return (nw == ns);
	}
