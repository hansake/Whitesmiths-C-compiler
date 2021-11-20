/*	VARIABLE ARGUMENT LIST HEADER
 *	copyright (c) 1984 by Whitesmiths, Ltd.
 */

#ifndef __STDARG__
#define __STDARG__	1

/*	type and macro declarations
 */
#ifndef __STDIO__
typedef char *va_list[2];
#endif
#ifndef _bndof
#define _bndof(ty)	((unsigned)&(((struct{char c;ty x;}*)0)->x))
#endif
#ifndef _bndup
#define _bndup(ptr, ty)	((unsigned)(ptr) + (_bndof(ty) - 1) & \
							~(_bndof(ty) - 1))
#endif

/*	macros
 */
#define va_start(ap, arg)	((ap)[0] = (char *)&(arg), \
							(ap)[1] = (ap)[0] + sizeof (arg))
#define va_arg(ap, type)	((ap)[0] = (char *)_bndup((ap)[1], type), \
							(ap)[1] = (ap)[0] + sizeof(type), \
							*(type *)(ap)[0])
#define va_end(ap)

#endif
