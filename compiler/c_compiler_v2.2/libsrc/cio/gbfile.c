/*	GET BINARY OR TEXT FILES FROM COMMAND LIST
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>

FILE getbfiles(pac, pav, dfd, efd, rsize)
	FAST ARGINT *pac;
	FAST TEXT ***pav;
	BYTES rsize;
	FILE dfd, efd;
	{
	FAST FILE fd;

	if (*pac < 0)
		fd = -1;
	else if (*pac == 0 || cmpstr(**pav, "-"))
		fd = dfd;
	else if ((fd = open(**pav, READ, rsize)) < 0)
		fd = efd;
	++*pav;
	if (--*pac <= 0)
		*pac = -1;
	return (fd);
	}
