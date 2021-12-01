/*  SEND SIGNAL TO A PROCESS (DUMMY) UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden
 */
#include <wslxa.h>

INT _kill(pid, sig)
	INT pid, sig;
	{
	return (0);
	}
