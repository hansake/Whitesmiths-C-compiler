/*	HEADER FOR CHARACTER TYPES
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */

#define _UC		001
#define _LC 	002
#define _D 		004
#define _S 		010
#define _P 		020
#define _C 		040
#define _X 		0100

GLOBAL TEXT _Ctype[]; 

#define _toupper(c) ((c) + 'A' - 'a'))
#define _tolower(c) ((c) + 'a' - 'A'))
#define isalnum(c) ((_Ctype+1)[c] & (_D | _UC | _LC))
#define isalpha(c) ((_Ctype+1)[c] & (_UC | _LC))
#define isascii(c) ((unsigned)(c) <= 0177)
#define iscntrl(c) ((_Ctype+1)[c] & _C)
#define isdigit(c) ((_Ctype+1)[c] & _D)
#define islower(c) ((_Ctype+1)[c] & _LC)
#define isprint(c) ((_Ctype+1)[c] & (_P | _D | _UC | _LC))
#define isgraph(c) (isprint(c) && (c) != ' ')
#define ispunct(c) ((_Ctype+1)[c] & _P)
#define isspace(c) ((_Ctype+1)[c] & _S)
#define isupper(c) ((_Ctype+1)[c] & _UC)
#define isxdigit(c) ((_Ctype+1)[c] & (_D | _X))
#define toascii(c) ((c) & 0177)
