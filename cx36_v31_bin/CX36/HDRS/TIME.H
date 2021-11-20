/*	TIME OF DAY HEADER
 *	copyright (c) 1984 by Whitesmiths, Ltd.
 */

#ifndef __TIME__
#define __TIME__	1

/*	set up default compiler version if none given
 */
#ifndef _CVERSION
#define _CVERSION	220
#endif

#if _CVERSION < 300
#define void char
#endif

/*	set up prototyping
 */
#ifndef __
#ifdef _PROTO
#define __(a)	a
#else
#define __(a)	()
#endif
#endif

/*	the asctime, ctime, gmtime and localtime structure
 */
typedef struct tm
	{
	int tm_sec;		/* seconds [0, 59] */
	int tm_min;		/* minutes [0, 59] */
	int tm_hour;	/* hours [0, 23] */
	int tm_mday;	/* day of month [1, 31] */  
	int tm_mon;		/* month of year [0, 11] */
	int tm_year;	/* years since 1900 A.D. */
	int tm_wday;	/* day of week, sunday = 0 [0, 6] */
	int tm_yday;	/* day of year [0, 365] */
	int tm_isdst;	/* non-zero if daylight savings time */
	} _tm;

/*	type declarations
 */
typedef unsigned long clock_t, time_t;

/*	function declarations
 */
char *asctime __((const struct tm *timeptr));
char *ctime __((const time_t *timer));
clock_t clock __((void));
double difftime __((time_t time2, time_t time1));
struct tm *gmtime __((const time_t *timer));
struct tm *localtime __((const time_t *timer));
time_t time __((time_t *timer));

/*	macro
 */
#define CLK_TCK		1	/* ticks = seconds */

#endif
