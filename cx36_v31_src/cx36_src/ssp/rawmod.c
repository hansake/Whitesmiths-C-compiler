/*  QUERY OR SET TTY FILE TO A NEW STATE (DUMMY) UNDER IBM SYSTEM/36
 *  copyright 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>

COUNT _rawmode(fd, new)
	FD fd;
	COUNT new;
	{
	/*  We are, according to the new standard, supposed to return R_QUERY
	 *  if fd is not to a tty, and since we don't have a tty driver yet...
	 */
	return (R_QUERY);
	}
