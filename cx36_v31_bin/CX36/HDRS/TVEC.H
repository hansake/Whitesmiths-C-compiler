/*	TIME OF DAY HEADER
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */

#ifndef __TVEC__
#define __TVEC__ 1

#define BEGDST		119
#define ENDDST		303
#define SECDAY 		86400
#define SECHOUR		3600
#define THURSDAY	4

#define DATEFIL	"/adm/date"
#define ZONEFIL	"/adm/zone"

/*	the atime, ltime, and vtime structure
 */
typedef struct _tvec
	{
	BYTES secs;		/* seconds		[0, 60)	*/
	BYTES mins;		/* minutes		[0, 60)	*/
	BYTES hrs;		/* hours		[0, 24)	*/
	BYTES dmth;		/* day of month			[1, 31]	*/  
	BYTES mth;		/* month of year		[0, 12)	*/
	BYTES yr;		/* years since 1900		[70, 131)	*/
	BYTES dwk;		/* day of week, sunday = 0	[0, 7)	*/
	BYTES dyr;		/* day of year				[0, 365]	*/
	BOOL dstf;		/* non-zero if daylight savings time	*/
	} TVEC;

#endif
