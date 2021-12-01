/*
 *  GET ENVIRONMENT VARIABLE (DUMMY) UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>

TEXT *_getenv(name)
	TEXT *name;
	{
	return (_cstreq(name, "PATH", 5) ? "" : NULL);
	}
