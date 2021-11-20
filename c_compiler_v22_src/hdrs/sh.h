/*	THE SHELL HEADER
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */

/*	mask for quoted command arguments
 */
#define HI_BIT	0200

/*	mask for flag to xecsh
 */
#define NWAIT	01
#define NFORK	02
#define QUIT	04
#define SETREAL	08

/*	types of tokens
 */
#define TSTRING	1
#define TPIPE	2
#define TANDAND	3
#define TOROR	4
#define TSH		5
#define TLT		6
#define TLTLT	7
#define TGT		8
#define TGTGT	9
#define TEOL	10
#define TAND	11
#define TSEMI	12

/*	limits
 */
#define MAXTERMS	32
#define AVSIZE		2000
#define EXECSIZE	8192

/*	structure for table of builtin commands
 */
typedef struct x {
	TEXT *name;
	BOOL (*fn)();
	} BUILTIN;

/*	structure for command tree leaves or nodes
 */
typedef struct tlist {
	BITS type;
	union {
		struct {
			struct tlist *left, *right;
			} node;
		struct {
			TEXT *psin, *psout;
			} leaf;
		} ln;
	} TERM;

/*	masks for TERM types
 */
#define NODE	0100000
#define PIPE	0040000
#define ANDAND	0020000
#define OROR	0010000
#define SEMI	0004000
#define AND		0002000
#define LTLT	0040000
#define GTGT	0020000
