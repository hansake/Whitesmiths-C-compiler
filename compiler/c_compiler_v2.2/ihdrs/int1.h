/*	STRUCTURES FOR PASS 1
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */

#ifdef LONGNAME
#define LENNAME	32
#else
#define LENNAME	8
#endif

/*	the quasi types
 */
#define LABEL	short
#define LEX		short

/*	the type bit patterns
 */
#define TFNRET	01
#define TARRAY	02
#define TPTRTO	03
#define TCHAR	004
#define TUCHAR	010
#define TSHORT	014
#define TSFIELD	020
#define TUSHORT	024
#define TLONG	030
#define TLFIELD	034
#define TULONG	040
#define TFLOAT	044
#define TDOUBLE	050
#define TSTRUCT	054
#define TUNION	060

/*	token values
 */
#define VAL		union val

union val {
	DOUBLE dn;
	TEXT an[LENNAME];
	LONG ln;
	struct {
		TEXT *sptr;
		COUNT slen;
		} x;
	};

/*	the attribute list for types
 */
#define ATTR	struct attr

struct attr {
	TEXT *next;		/* SHDB ATTR * */
	union {
		LONG m;
		TEXT *stab;	/* SHDB SYMBOL * */
		struct {
			TINY boff, bsize;
			} b;
		} a;
	};

/*	the case structure
 */
#define CASE	struct kase

struct kase {
	TEXT *next;
	LABEL clabel;
	LONG cvalue;
	};

/*	the symbol table entry
 */
#define SYMBOL struct symbol

struct symbol {
	TEXT *next;
	ATTR *at;
	VAL n;
	BITS ty;
	LEX sc;
	union {
		TEXT *tagtab;	/* SHDB SYMBOL * */
		TINY reg;
		LABEL label;
		LONG offset;
		} s;
	};

/*	expression terms
 */
#define LITERAL	struct term
#define TERM	struct term

struct term {
	TEXT *next;
	ATTR *at;
	VAL n;
	BITS ty;
	LEX op;
	union {
		ATTR l;
		struct {
			TEXT *left, *right, *mid;	/* SHDB TERM * */
			} o;
		struct {
			LONG bias;
			TINY idx, refs;
			} v;
		} e;
	};

/*	the token structure
 */
#define TOKEN	struct token

struct token {
	LEX type;
	VAL t;
	};
