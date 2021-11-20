/*	STRING TYPES HEADER
 *	copyright (c) 1984 by Whitesmiths, Ltd.
 */

#ifndef __STRING__
#define __STRING__	1

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

/*	set up type not already set up
 */
#ifndef __STDEFS__
#ifndef __STDLIB__
#ifndef __STDIO__
typedef unsigned int size_t;
#endif
#endif
#endif

/*	function declarations
 */
char *strcat __((char *s1, const char *s2));
char *strchr __((const char *s, int c));
char *strcpy __((char *s1, const char *s2));
char *strncat __((char *s1, const char *s2, size_t n));
char *strncpy __((char *s1, const char *s2, size_t n));
char *strpbrk __((const char *s1, const char *s2));
char *strrchr __((const char *s, int c));
char *strtok __((char *s1, const char *s2));
int memcmp __((const void *s1, const void *s2, size_t n));
int strcmp __((const char *s1, const char *s2));
int strncmp __((const char *s1, const char *s2, size_t n));
size_t strcspn __((const char *s1, const char *s2));
size_t strlen __((const char *s));
size_t strspn __((const char *s1, const char *s2));
void *memchr __((const void *s, int c, size_t n));
void *memcpy __((void *s1, const void *s2, size_t n));
void *memset __((void *s, int c, size_t n));

#endif
