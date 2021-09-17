/*	HEADER FOR STANDARD I/O UNDER UNIX V7
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */

#ifndef BUFSIZ
#define ASSERT(b, e) if (!(b))  e
#define BUFSIZ 512
#define EOF -1
#define NOINIT 0
#define NULL 0
#define STRUCTASST(a, b) cpybuf(&a, &b, sizeof (a))
#define atan arctan
#define creat(name, mode) create(name, WRITE, _recsize)
#define exit exit7
#define fclose fclos7
#define lseek lsek7
#define fileno(fp) (fp->_fd)
#define fopen fopen7
#define fread fread7
#define fwrite fwrit7
#define getc(pf)	(0 < (pf)->_nleft ? \
	 (--(pf)->_nleft, *(pf)->_pnext++ & 0377) : fgetc(pf))
#define getchar(x) getc(stdin)
#define log ln
#define malloc nalloc
#define putc(c, pf)	fputc(c, pf)
#define putchar(c) putc(c, stdout)
#define sbrk(n) (sbreak(n) ? 0 : -1)
#define stderr pstderr
#define stdin pstdin
#define stdout pstdout
#define strcat(a, b) cpystr(a, a, b, NULL)
#define strchr index
#define strcpy(a, b) cpystr(a, b, NULL)
#define strlen(a) lenstr(a)
#define strrchr rindex
#define unlink remove
#define METACH short
#define COUNT short
#define FILEDESC short
#define FILE struct file
struct file
	{
	FILEDESC _fd;
	COUNT _nleft;
	COUNT _fmode;
	TEXT *_pnext;
	TEXT _buf[BUFSIZ];
	};
extern FILE *pstdin, *pstdout, *pstderr;
#endif
