/*  TRANSLATE BUFFER IN PLACE UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>

UTINY *_xlate(t, oldb, n)
    FAST TEXT t[] ;
    UTINY *oldb; 
    FAST BYTES n; 
    {
    FAST UTINY *b = oldb; 

    for (; n--; ++b)
        *b = t[*b];
    return (oldb) ;
    }
