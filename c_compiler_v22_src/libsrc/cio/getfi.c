/*	GET FILES FROM COMMAND LIST
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

FILE getfiles(pac, pav, dfd, efd)
	FAST ARGINT *pac;
	FAST TEXT ***pav;
	FILE dfd, efd;
	{
	FAST FILE fd;

	if (*pac < 0)
		fd = -1;
	else if (*pac == 0 || cmpstr(**pav, "-"))
		fd = dfd;
	else if ((fd = open(**pav, READ, 0)) < 0)
		fd = efd;
	++*pav;
	if (--*pac <= 0)
		*pac = -1;
	return (fd);
	}
