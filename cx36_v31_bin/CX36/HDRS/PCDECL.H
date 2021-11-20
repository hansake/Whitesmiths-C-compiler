/*	PROTOTYPE DECLARATIONS FOR LIBC FUNCTIONS
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef	__PCDECL__
#define	__PCDECL__	1

ARGINT read __((FD fd, TEXT *buf, ARGINT size));
ARGINT round __((DOUBLE d));
ARGINT trunc __((DOUBLE d));
ARGINT write __((FD fd, TEXT *buf, ARGINT size));
BOOL cmpbuf __((TEXT *s1, TEXT *s2, BYTES n));
BOOL cmpstr __((TEXT *s1, TEXT *s2));
BOOL getin __((BYTES *pac, TEXT ***pav));
BOOL match __((TEXT *buf, BYTES n, TEXT *pat));
BOOL prefix __((TEXT *s1, TEXT *s2));
BOOL remark __((TEXT *s1, TEXT *s2));
BYTES amatch __((TEXT *buf, BYTES n, BYTES indx, TEXT *pat, \
	struct _msub *pmsub));
BYTES btod __((TEXT *is, BYTES n, DOUBLE *pdnum));
BYTES btoi __((TEXT *s, BYTES n, BYTES *i, COUNT base));
BYTES btol __((TEXT *s, BYTES n, ULONG *l, COUNT base));
BYTES btos __((TEXT *s, BYTES n, UCOUNT *i, COUNT base));
BYTES cpybuf __((TEXT *s1, TEXT *s2, BYTES an));
BYTES decode __((TEXT *s, BYTES n, TEXT *f, ...));
BYTES dtoe __((TEXT *is, DOUBLE d, COUNT p, COUNT g));
BYTES dtof __((TEXT *is, DOUBLE d, COUNT p, COUNT g));
BYTES enter __((BYTES (*pfn)(), BYTES arg));
BYTES fill __((TEXT *s, BYTES n, TEXT c));
BYTES getl __((struct _file *pf, TEXT *s, BYTES n));
BYTES getlin __((TEXT *s, BYTES n));
BYTES inbuf __((TEXT *is, BYTES n, TEXT *p));
BYTES instr __((TEXT *is, TEXT *p));
BYTES itob __((TEXT *is, ARGINT n, COUNT base));
BYTES lenstr __((TEXT *is));
BYTES lower __((TEXT *s, BYTES n));
BYTES ltob __((TEXT *is, LONG ln, COUNT base));
BYTES notbuf __((TEXT *is, BYTES n, TEXT *p));
BYTES notstr __((TEXT *is, TEXT *p));
BYTES putl __((struct _file *pf, TEXT *s, BYTES n));
BYTES putlin __((TEXT *s, BYTES n));
BYTES scnbuf __((TEXT *s, BYTES n, TEXT c));
BYTES scnstr __((TEXT *s, TEXT c));
BYTES squeeze __((TEXT *s, BYTES n, TEXT c));
BYTES stob __((TEXT *is, COUNT n, COUNT base));
BYTES subbuf __((TEXT *ps, BYTES ns, TEXT *pp, BYTES np));
BYTES substr __((TEXT *ps, TEXT *pp));
COUNT close __((FD fd));
COUNT doesc __((TEXT **pp, TEXT *magic));
COUNT encode __((TEXT *s, BYTES n, TEXT *f, ...));
COUNT getf __((struct _file *pf, TEXT *fmt, ...));
COUNT getfmt __((TEXT *fmt, ...));
COUNT lstoi __((TEXT *s));
COUNT ordbuf __((TEXT *p, TEXT *q, COUNT n));
COUNT usage __((TEXT *msg));
FD getbfiles __((BYTES *pac, TEXT ***pav, FD dfd, FD efd, BYTES rsize));
FD getfiles __((BYTES *pac, TEXT ***pav, FD dfd, FD efd));
FD open __((TEXT *fname, BITS oflag, BITS mode));
UCOUNT lstou __((TEXT *s));
UCOUNT xstos __((TEXT *s, BOOL lsfmt));
VOID errfmt __((TEXT *fmt, ...));
VOID error __((TEXT *s1, TEXT *s2));
VOID leave __((BYTES val));
VOID mapchar __((TEXT c, TEXT *buf));
VOID prtheap __((VOID));
VOID putf __((struct _file *pf, TEXT *fmt, ...));
VOID putfmt __((TEXT *fmt, ...));
VOID putstr __((FD fd, ...));
VOID sort __((ARGINT n, COUNT (*ordf)(), VOID (*excf)(), TEXT *base));

#endif
