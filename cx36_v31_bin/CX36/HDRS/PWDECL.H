/*	PROTOTYPE DECLARATIONS FOR LIBW FUNCTIONS
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef	__PWDECL__
#define	__PWDECL__	1

COUNT remove __((TEXT *fname));
COUNT usage __((TEXT *msg));
METACH getc __((struct _file *pf));
METACH getch __((VOID));
METACH putc __((struct _file *pf, COUNT c));
METACH putch __((COUNT c));
VOID exit __((BOOL status));
VOID onintr __((VOID (*pfn)()));

#endif
