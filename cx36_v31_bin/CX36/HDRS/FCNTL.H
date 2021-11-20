/*	SHARED FILE CONTROL HEADER
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef __FCNTL__
#define __FCNTL__	1

/*	open flags
 */
#define O_RDONLY	00		/* read only */
#define O_WRONLY	01		/* write only */
#define O_RDWR		02		/* read/write */
#define O_NDELAY	04		/* non-blocking i/o */
#define O_APPEND	010		/* append */
#define O_CREAT		0400	/* create */
#define O_TRUNC		01000	/* truncate */
#define O_EXCL		02000	/* exclusive open */
#define O_XTYPE		010000	/* extended type field */
#define O_BUF		020000	/* buffered write, used only by WSL i/o for 3.0 */
#define O_REUSE		040000	/* reuse FILE structure (as in freopen) */
#define O_BIN		0100000	/* open file for binary i/o */

#endif
