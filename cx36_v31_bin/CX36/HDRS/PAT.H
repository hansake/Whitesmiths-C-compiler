/*	PATTERN MATCHING HEADER
 *	copyright (c) 1980 by Whitesmiths Ltd.
 */

#ifndef __PAT__
#define __PAT__	1

#define CCHAR	1	/* literal character */
#define ANY		2	/* any character */
#define SBOL	3	/* beginning of line */
#define SEOL	4	/* end of line */
#define CLOSE	5	/* reflexive closure */
#define CCL		6	/* begin character class */
#define NCCL	7	/* begin negated character class */
#define RANGE	8	/* range within character class */
#define CCLEND	9	/* end of character class */
#define PEND	10	/* pattern end */
#define RPAR	19	/* right parenthesis */
#define LPAR	20	/* left parenthesis */

/*	the substitution structure
 */
typedef struct _msub
	{
	TEXT *mtext;
	BYTES mlen;
	} MSUB;

#endif
