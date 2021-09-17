/*	SORT PROGRAM HEADER FILE
 * copyright (c) 1981 by Whitesmiths, Ltd.
 */

#define KEYLIM	10
#define MAXLINE 512
#define MAXPTR 500
#define MAXTEXT 10000
#define MAXFILES 7

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
