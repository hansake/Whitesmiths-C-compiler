/*	STANDARD I/O HEADER
 *	copyright (c) 1984 by Whitesmiths, Ltd.
 */

#ifndef __STDIO__
#define __STDIO__	1

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

/*	set up types not already set up
 */
#ifndef __STDARG__
typedef char *va_list[2];
#endif
#ifndef __STDEFS__
#ifndef __STDLIB__
#ifndef __STRING__
typedef unsigned int size_t;
#endif
#endif
#endif

/*	the FILE structure
 */
typedef struct _file
	{
	struct _file *flist;	/* chain used to flush buffers */
	short fd;				/* file descriptor */
	unsigned short flag;	/* flag bits */
	int nleft;				/* reading, # of chars undelivered in buffer */
							/* writing, # of chars put in buffer to output */
	int bufsize;			/* size of buffer (default BUFSIZ) */
	unsigned char *pnext;	/* ptr to next char to deliver */
	unsigned char *buf;		/* ptr to allocated buffer */
	} FILE;

/*	function declarations
 */
FILE *fopen __((const char *pathname, const char *type));
FILE *freopen __((const char *pathname, const char *type, FILE *stream));
FILE *tmpfile __((void));
char *fgets __((char *s, int n, FILE *stream));
char *gets __((char *s));
char *perror __((const char *s));
char *tmpnam __((char *s));
int fclose __((FILE *stream));
int feof __((FILE *stream));
int ferror __((FILE *stream));
int fflush __((FILE *stream));
int fgetc __((FILE *stream));
int fprintf __((FILE *stream, const char *format, ...));
int fputc __((int c, FILE *stream));
int fputs __((const char *s, FILE *stream));
int fread __((void *ptr, size_t size, int nelem, FILE *stream));
int fscanf __((FILE *stream, const char *format, ...));
int fseek __((FILE *stream, long offset, int ptrname));
int fwrite __((const void *ptr, size_t size, int nelem, FILE *stream));
int printf __((const char *format, ...));
int puts __((const char *s));
int remove __((const char *s));
int rename __((const char *old, const char *new));
int scanf __((const char *format, ...));
int setvbuf __((FILE *stream, char *buf, int type, int size));
int sprintf __((char *s, const char *format, ...));
int sscanf __((char *s, const char *format, ...));
int ungetc __((int c, FILE *stream));
int vfprintf __((FILE *stream, const char *format, va_list arg));
int vprintf __((const char *format, va_list arg));
int vsprintf __((char *s, const char *format, va_list arg));
long ftell __((FILE *stream));
void clearerr __((FILE *stream));
void rewind __((FILE *stream));
void setbuf __((FILE *stream, char *buf));

/*	global variable references
 */
extern FILE *stderr;
extern FILE *stdin;
extern FILE *stdout;

/*	include system dependent information
 */
#include <_OS>

/*	system parameters
 */
#define _IOFBF		1
#define _IOLBF		2
#define _IONBF		3
#define BUFSIZ 		512
#define EOF		 	-1
#define L_tmpnam	_TMPSIZ
#define SEEK_SET	0
#define	SEEK_CUR	1
#define SEEK_END	2
#define SYS_OPEN	_MAXFILE
#define TMP_MAX		_TMPMAX

/*	flag values used in getc and putc (020 and 040) should track values in
 *	READING and WRITING respectively in stioli.h
 */

/*	macros
 */
#define getc(pf)	(((pf)->flag & 020 && 0 < (pf)->nleft) ? \
					(--(pf)->nleft, *(pf)->pnext++) : fgetc(pf))
#define getchar()	(getc(stdin))
#define putc(c, pf)	(((pf)->flag & 040 && (pf)->nleft < (pf)->bufsize) ? \
					((pf)->buf[(pf)->nleft++] = (c)) : fputc(c, pf))
#define putchar(c)	(putc(c, stdout))

#endif
