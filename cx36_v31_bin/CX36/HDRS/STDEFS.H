/*	STANDARD DEFINES HEADER
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef __STDEFS__
#define __STDEFS__	1

/*	types
 */
typedef long ptrdiff_t;

/*	set up type not already set up
 */
#ifndef __STDIO__
#ifndef __STDLIB__
#ifndef __STRING__
typedef unsigned int size_t;
#endif
#endif
#endif

/*	global variable references
 */
extern int _errno;

/*	macros
 */
#ifndef NULL
#define NULL	(void *)0
#endif
#define errno	_errno

#endif
