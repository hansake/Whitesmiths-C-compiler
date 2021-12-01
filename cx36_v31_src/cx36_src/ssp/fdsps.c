/*  THE NFILES-ELEMENT ARRAY OF POINTERS TO FILE DESCRIPTOR STRUCTURES
 *						UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"

FDS *_fdsps[NFILES] = {0};
