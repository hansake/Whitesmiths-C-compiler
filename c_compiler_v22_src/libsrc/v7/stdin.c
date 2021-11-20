/*	STDIN, STDERR, STDOUT (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */

#define BUFSIZ 512

typedef struct file
	{
	short _fd;
	short _nleft;
	short _fmode;
	char *_pnext;
	char _buf[BUFSIZ];
	} FILE;

FILE stdin = {0, 0, 0, &stdin._buf, 0}, *pstdin = &stdin;
FILE stdout = {1, 0, 1, &stdout._buf, 0}, *pstdout = &stdout;
FILE stderr = {2, 0, 1, &stderr._buf, 0}, *pstderr = &stderr;
