/*	IBM S/36 SSP GET COMMAND LINE
 *	copyright (c) 1983, 1986 by Whitesmiths, Ltd.
 */
#include <wslxa.h>
#include <fcntl.h>
#include <ctype.h>
#include "ssp.h"

BOOL _main(s)
	FAST TEXT *s;
	{
	IMPORT TEXT *_pname;
	BOOL txtleft;
	BYTES ac;
	TEXT *av[64], *sn;

	_open("sysin:", O_RDONLY, 0, NULL);					/* STDIN */
	_open("syslog:", O_WRONLY, 0, NULL);				/* STDOUT */
	_open("syslog:", O_WRONLY|O_XTYPE, 0, "recl=1");	/* STDERR */
	av[0] = _pname;
	for (ac = 1, txtleft = YES; *s && txtleft && ac < 64;)
		{
		while (*s == ' ')
			++s;
		if (*s == '\0')
			break;
		for (sn = s; *sn != ' '; ++sn)
			{
			if (*sn == '\0')
				{
				txtleft = NO;
				break;
				}
			*sn = _tolower(*sn);
			}
		*sn = '\0';
		if (*s == '<')
			{
			_close(STDIN);
			if (_open(&s[1], O_RDONLY, 0, 0) != STDIN)
				_error("can't open ", &s[1]);
			}
		else if (*s == '>')
			{
			_close(STDOUT);
			if (_open(&s[1], O_CREAT|O_TRUNC|O_WRONLY, 0, 0) != STDOUT)
				_error("can't open ", &s[1]);
			}
		else
			av[ac++] = s;
		s = ++sn;
		}
	return (main(ac, av));
	}
