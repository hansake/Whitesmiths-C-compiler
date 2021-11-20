/*	SORT PROGRAM HEADER
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */

#ifndef __SORT__
#define __SORT__	1

#define KEYLIM		10
#define MAXLINE		512
#define MAXPTR		500
#define MAXTEXT		10000
#define MAXFILES	7

typedef struct
	{
	BYTES n;
	TEXT *karray[KEYLIM + 1];
	} KARR ;

typedef struct
	{
	COUNT len;
	TEXT text[MAXLINE];
	} LINE;

typedef struct
	{
	BOOL rev;
	BOOL skipbl;
	TEXT cmptype;
	TEXT tabchar;
	COUNT begfsk;
	COUNT begaddch;
	COUNT endfsk;
	COUNT endaddch;
	} KEY;

#endif
