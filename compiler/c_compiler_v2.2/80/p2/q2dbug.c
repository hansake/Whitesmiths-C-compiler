/*	DEBUGGING PRINTOUT FOR EXPRESSIONS
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

LOCAL TEXT *tyname[] {"char", "uchar", "short", "sfield", "unsign",
	"long", "lfield", "ulong", "float", "double", "?!"};
LOCAL TINY tytab[] {XCHAR, XUCHAR, XSHORT, XSFIELD, XUSHORT,
	XLONG, XLFIELD, XULONG, XFLOAT, XDOUBLE, 0};

/*	put indented (sub)expression
 */
VOID putexpr(q, indent)
	FAST EXPR *q;
	COUNT indent;
	{
	IMPORT BOOL pflag;
	IMPORT TEXT *tyname[];
	IMPORT TINY tytab[];
	FAST BITS ty;
	FAST COUNT i;

	if (!pflag)
		return;
	ty = q->e.v.ty;
	for (i = 0; i < indent; ++i)
		putfmt("\t");
	putfmt("%p", tyname[scnstr(tytab, ty & 017)]);
	if ((ty & 017) == XSFIELD || (ty & 017) == XLFIELD)
		putfmt("(%i, %i)", (ty >> 4) & 077, (ty >> 10) & 077);
	if (!q->op)
		putfmt(": TERM (%i)%b+%os[%oc]\n", q->e.v.refs,
			q->e.v.nm, inbuf(q->e.v.nm, LENNAME, "\0"),
			q->e.v.bias, q->e.v.idx);
	else
		{
		putfmt(": %o\n", q->op);
		putexpr(q->e.o.left, indent + 1);
		if (q->op == LQUERY)
			putexpr(q->e.o.mid, indent + 1);
		if (q->op & BINOP)
			putexpr(q->e.o.right, indent + 1);
		}
	}

/*	put a term
 */
VOID putterm(q, indent)
	FAST EXPR *q;
	COUNT indent;
	{
	IMPORT BOOL pflag;
	FAST COUNT i;

	if (!pflag)
		return;
	for (i = 0; i < indent; ++i)
		putfmt("\t");
	if (!q->op)
		{
		putfmt("f %o: ", q->got);
		putaddr(&q->f);
		putfmt("; v: ");
		putaddr(&q->e.v);
		putfmt("\n");
		}
	else
		{
		putfmt("%o %o : ", q->op, q->got);
		putaddr(&q->f);
		putfmt("; v: %p\n", tyname[scnstr(tytab, q->e.o.ty & 017)]);
		}
	}

/*	put the address field
 */
VOID putaddr(ad)
	FAST ADDR *ad;
	{
	IMPORT TEXT *tyname[];
	IMPORT TINY tytab[];

	putfmt("%p", tyname[scnstr(tytab, ad->ty & 017)]);
	putfmt(", (%i)%b+%os[%oc]",
		ad->refs, ad->nm, inbuf(ad->nm, LENNAME, "\0"), ad->bias, ad->idx);
	}

/*	formatted output to STDOUT
 */
VOID putfmt(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT BOOL pflag;
	IMPORT COUNT putl();
	IMPORT FIO stdout;

	if (!pflag)
		return;
	_putf(&putl, &stdout, fmt, &args);
	}
