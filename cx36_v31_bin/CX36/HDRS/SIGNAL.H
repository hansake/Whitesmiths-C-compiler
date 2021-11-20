/*	SIGNAL HANDLING
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef __SIGNAL__
#define __SIGNAL__	1

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

/*	signal values
 */
#define SIG_ERR		(void (*)())-1		/* error */
#define SIG_DFL		(void (*)())0		/* default */
#define SIG_IGN		(void (*)())1		/* no signal */
#define SIGINT		2		/* SIGINT */
#define SIGABRT		3		/* SIGQUIT */
#define SIGILL		4		/* SIGILIN */
#define SIGFPE		8		/* SIGFPT */
#define SIGSEGV		11		/* SIGSEG */
#define SIGTERM		15		/* SIGTERM */

/*	function declarations
 */
int kill __((int pid, int signo));
void (*signal __((int signo, void (*pfunc)())))();

#endif
