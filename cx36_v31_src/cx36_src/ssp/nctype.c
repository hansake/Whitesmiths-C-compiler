/*	CHARACTER TYPES - ASCII/EBCDIC
 *	copyright (c) 1984 by Whitesmiths, Ltd.
 */
#include <wslxa.h>
#include <baseli.h>
#undef isalpha
#undef isdigit
#undef islower
#undef isupper
#undef tolower
#undef toupper
#include <ctype.h>

/*	character type mapping array
 */
#ifndef _EBCDIC

TEXT _ctype[257] = {
0, /* EOF */
0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,
0|_C,     0|_C|_S,  0|_C|_S,  0|_C|_S,  0|_C|_S,  0|_C|_S,  0|_C,     0|_C,
0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,
0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,     0|_C,
0|_SP,    0|_P,     0|_P,     0|_P,     0|_P,     0|_P,     0|_P,     0|_P,
0|_P,     0|_P,     0|_P,     0|_P,     0|_P,     0|_P,     0|_P,     0|_P,
0|_D,     0|_D,     0|_D,     0|_D,     0|_D,     0|_D,     0|_D,     0|_D,
0|_D,     0|_D,     0|_P,     0|_P,     0|_P,     0|_P,     0|_P,     0|_P,
0|_P,     0|_UC|_X, 0|_UC|_X, 0|_UC|_X, 0|_UC|_X, 0|_UC|_X, 0|_UC|_X, 0|_UC,
0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,
0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,
0|_UC,    0|_UC,    0|_UC,    0|_P,     0|_P,     0|_P,     0|_P,     0|_P,
0|_P,     0|_LC|_X, 0|_LC|_X, 0|_LC|_X, 0|_LC|_X, 0|_LC|_X, 0|_LC|_X, 0|_LC,
0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,
0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,
0|_LC,    0|_LC,    0|_LC,    0|_P,     0|_P,     0|_P,     0|_P,     0|_C,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
0,        0,        0,        0,        0,        0,        0,        0,
	};

#else

TEXT _ctype[257] = {
0, /* EOF */
0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_S|_C|_SP,0|_C|_SP,0|_C|_SP,
0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_S|_C|_SP,0|_S|_C|_SP,0|_C|_SP,0|_C|_SP,
0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP,
0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP,
0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_S|_C|_SP,0|_C|_SP,0|_C|_SP,
0|_C|_SP, 0|_SP,    0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP,
0| 0|_SP, 0| 0|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP,
0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_C|_SP, 0|_SP,    0|_C|_SP,
0|_S|_P|_SP,0,      0,        0,        0,        0,         0,       0,
0,        0,        0|_P,     0|_P,     0|_P,     0|_P,      0|_P,    0|_P,
0|_P,     0,        0,        0,        0,        0,         0,       0,
0,        0,        0|_P,     0|_P,     0|_P,     0|_P,      0|_P,    0|_P,
0|_P,     0|_P,     0,        0,        0,        0,         0,       0,
0,        0,        0|_P,     0|_P,     0|_P,     0|_P,      0|_P,    0|_P,
0,        0,        0,        0,        0,        0,         0,       0,
0,        0|_P,     0|_P,     0|_P,     0|_P,     0|_P,      0|_P,    0|_P,
0,        0|_LC|_X, 0|_LC|_X, 0|_LC|_X, 0|_LC|_X, 0|_LC|_X,  0|_LC|_X,0|_LC,
0|_LC,    0|_LC,    0,        0,        0,        0,         0,       0,
0,        0|_LC,    0|_LC,    0|_LC,    0|_LC,    0|_LC,     0|_LC,   0|_LC,
0|_LC,    0|_LC,    0,        0,        0,        0,         0,       0,
0,        0|_P,     0|_LC,    0|_LC,    0|_LC,    0|_LC,     0|_LC,   0|_LC,
0|_LC,    0|_LC,    0,        0,        0,        0|_P,      0,       0,
0,        0,        0,        0,        0,        0,         0,       0,
0,        0,        0,        0,        0,        0|_P,      0,       0,
0|_P,     0|_UC|_X, 0|_UC|_X, 0|_UC|_X, 0|_UC|_X, 0|_UC|_X,  0|_UC|_X,0|_UC,
0|_UC,    0|_UC,    0,        0,        0,        0,         0,       0,
0|_P,     0|_UC,    0|_UC,    0|_UC,    0|_UC,    0|_UC,     0|_UC,   0|_UC,
0|_UC,    0|_UC,    0,        0,        0,        0,         0,       0,
0|_P,     0,        0|_UC,    0|_UC,    0|_UC,    0|_UC,     0|_UC,   0|_UC,
0|_UC,    0|_UC,    0,        0,        0,        0,         0,       0,
0|_D,     0|_D,     0|_D,     0|_D,     0|_D,     0|_D,      0|_D,    0|_D,
0|_D,     0|_D,     0,        0,        0,        0,         0,       0|_SP
	};
#endif
