/*	PROGRAM ASSERTION HEADER
 *	copyright (c) 1984 by Whitesmiths, Ltd.
 */

#ifndef __ASSERT__
#define __ASSERT__	1

/*	set up file and line defs if not defined by pp
 */
#ifndef __FILE__
#define __FILE__	"error"
#endif
#ifndef __LINE__
#define __LINE__	0
#endif

/*	macro
 */
#ifndef NDEBUG
extern struct _file *stderr;
#define assert(expr) \
		{ \
		if (!(expr)) \
			{ \
			fprintf(stderr, "Assertion failed: " #expr ", file %s, line %d\n", \
				__FILE__, __LINE__), \
			abort(); \
			} \
		}
#else
#define assert(expr)
#endif

#endif
