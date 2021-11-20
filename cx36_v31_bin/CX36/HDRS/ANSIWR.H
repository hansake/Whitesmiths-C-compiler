/*	WRAPPER DEFINITIONS FOR ANSI ENVIRONMENT
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */
#ifndef __ANSIWR__
#define __ANSIWR__	1

#define atan2	_atan2
#define clock	_time
#define exit	_terminate
#define getenv	_getenv
#define onexit	_onexit
#define realloc	_realloc
#define remove	_remove
#define rename	_rename
#define system	_system

#endif
