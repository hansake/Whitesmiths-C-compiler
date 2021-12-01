/*	DUMP AN EXPRESSION
 *	copyright (c) 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

#undef dbexpr
#undef dbfmt
#undef dbprint
#undef dbtree

#define	none	""

LOCAL VOID dobits();
LOCAL VOID dolong();
LOCAL VOID doname();

typedef	struct map {
	ULONG val;
	TEXT *name;
	} MAP;

/*	register names
 */
LOCAL MAP regnames[] = {
	AC0, "AC0",
	XR2, "XR2",
	F0, "F0",
	XR1, "XR1",
	0, none,
	};

/*	operation code names
 */
LOCAL MAP opnames[] = {
	DINDIR, "DINDIR",
	DADDR, "DADDR",
	LNOT, "LNOT",
	LANDAND, "LANDAND",
	LOROR, "LOROR",
	LLESS, "LLESS",
	LLEQ, "LLEQ",
	LGREAT, "LGREAT",
	LGEQ, "LGEQ",
	LISEQ, "LISEQ",
	LNOTEQ, "LNOTEQ",
	LQUERY, "LQUERY",
	DFNCALL, "DFNCALL",
	DFNONLY, "DFNONLY",
	DELIST, "DELIST",
	DCAST, "DCAST",
	LPLUS, "LPLUS",
	DCMP, "DCMP",
	DLESS, "DLESS",
	LCOMP, "LCOMP",
	DMINUS, "DMINUS",
	LTIMES, "LTIMES",
	LDIVIDE, "LDIVIDE",
	LMODULO, "LMODULO",
	LMINUS, "LMINUS",
	LLSHIFT, "LLSHIFT",
	LRSHIFT, "LRSHIFT",
	LAND, "LAND",
	LXOR, "LXOR",
	LOR, "LOR",
	LGETS, "LGETS",
	GGETSTR, "GGETSTR", 
	LGTIM, "LGTIM",
	LGDIV, "LGDIV",
	LGMOD, "LGMOD",
	LGPLU, "LGPLU",
	LGMIN, "LGMIN",
	LGRSH, "LGRSH",
	LGAND, "LGAND",
	LGXOR, "LGXOR",
	LGOR, "LGOR",
	DGMIN, "DGMIN",
	DGPLU, "DGPLU",
	0, "leaf",
	};

/*	want and got names
 */
LOCAL MAP wgnames[] = {
	WNULL, "WNULL",
	WTERM, "WTERM",
	WMEM, "WMEM",
	WVMEM, "WVMEM",
	WIMM, "WIMM",
	WXR2, "WXR2",
	WVXR2, "WVXR2",
	WPMEM, "WPMEM",
	WREREF, "WREREF",
    WAC0, "WAC0",
    WVAC0, "WVAC0",
	PTRSET, "PTRSET",
	VOLSET, "VOLSET",
	GRVOL, "GRVOL",
	GCCZERO, "GCCZERO",
	GCCSET, "GCCSET",
	GJUNK, "GJUNK",
	GVOL, "GVOL",
	GNOCODE, "GNOCODE",
	0, none,
	};

/*	index names
 */
LOCAL MAP idxnames[] = {
	XXR1, "XXR1",
	XXR2, "XXR2",
	XAC0, "XAC0",
	XF0, "XF0",
	XVXR1, "XVXR1",
	0, none,
	};

/*	type names
 */
LOCAL MAP typnames[] = {
	XCHAR, "XCHAR",
	XUCHAR, "XUCHAR",
	XSHORT, "XSHORT",
	XSFIELD, "XSFIELD",
	XUSHORT, "XUSHORT",
	XLONG, "XLONG",
	XLFIELD, "XLFIELD",
	XULONG, "XULONG",
	XFLOAT, "XFLOAT",
	XDOUBLE, "XDOUBLE",
	XPTRTO, "XPTRTO",
	XLPTRTO, "XLPTRTO",
	XSTRUCT, "XSTRUCT",
	0, none,
	};

/*	space names
 */
LOCAL MAP spcnames[] = {
	SVOL, "SVOL",
	SCONST, "SCONST",
	SRVAL, "SRVAL",
	SFAR, "SFAR",
	SPORT, "SPORT",
	SBUI, "SBUI",
	SFUNC, "SFUNC",
	0, none,
	};

/*	table attribute names
 */
LOCAL MAP tatrnames[] = {
	CPWR2OP, "CPWR2OP",
	CLT256OP, "CLT256OP",
	IMMOP, "IMMOP",
	CONOP, "CONOP",
	XROP, 	"XROP",
	PXR2OP, "PXR2OP",
	XR2OP, "XR2OP",
	AC0OP, "AC0OP",
	MEMOP, "MEMOP",
	NOOP, "NOOP",
	0, none,
	};

/*	table type names
 */
LOCAL MAP ttypnames[] = {
	YCHAR, "YCHAR",
	YUCHAR, "YUCHAR",
	YSHORT, "YSHORT",
	YSFIELD, "YSFIELD",
	YUSHORT, "YUSHORT",
	YPTRTO, "YPTRTO",
	YLONG, "YLONG",
	YLFIELD, "YLFIELD",
	YULONG, "YULONG",
	YFLOAT, "YFLOAT",
	YDOUBLE, "YDOUBLE",
	YTEST, "YTEST",
	0, none,
	};

/*	table result names
 */
LOCAL MAP tresnames[] = {
	NOCC, "NOCC",
	CCZERO, "CCZERO",
	CCSET, "CCSET",
	CCONLY, "CCONLY",
	ESCR, "ESCR",
	0, none,
	};

/*	compress output under dbprint
 */
LOCAL COUNT expflag = {0};
LOCAL COUNT tabs = {0};

/*	print the ADDR block
 */
LOCAL VOID doaddr(msg, ad)
	TEXT *msg;
	ADDR *ad;
	{
	dbfmt("%p ", msg);
	if (ad->refs || ad->bias || ad->nm)
		{
		dbfmt(" addr refs: %uc, bias: %l, nm: %p",
			ad->refs, ad->bias, ad->nm[0] ? ad->nm : "NULL");
		}
	if (ad->ty)
		doname(", ty: ", typnames, ad->ty);
	if (ad->idx)
		doname(", idx:", idxnames, ad->idx);
	if (ad->space)
		dobits(", space: ", spcnames, ad->space);
	if (ad->stsize)
		dbfmt(", stsize: %u", ad->stsize);
	}

/*	print the ATTR block
 */
LOCAL VOID doattr(msg, ty, space, stsize)
	TEXT *msg;
	TYPE ty;
	SPACE space;
	BITS stsize;
	{
	if (space || stsize)
		{
		dbfmt("%p attr", msg);
		if (space)
			dobits(", space: ", spcnames, space);
		if (stsize)
			dbfmt(", stsize: %ui", stsize);
		}
	}

LOCAL VOID dobits(msg, tab, code)
	TEXT *msg;
	MAP tab[];
	BITS code;
	{
	
	dolong(msg, tab, (ULONG) code);

    }
/*	print verbose bits according to tab map
 */
LOCAL VOID dolong(msg, tab, code)
	TEXT *msg;
	MAP tab[];
	ULONG code;
	{
	FAST MAP *p;

	dbfmt("%p", msg);
	for (p = tab; p->val; ++p)
		if ((code & p->val) == p->val)
			{
			dbfmt("%p", p->name);
			code &= ~p->val;
			if (code)
				dbfmt("|");
			}
	if (code)
		dbfmt("0%os", code);
	}

/*	print name from name map
 */
LOCAL VOID doname(msg, tab, namno)
	TEXT *msg;
	MAP tab[];
	BITS namno;
	{
	FAST MAP *p;

	for (p = tab; p->val && p->val != namno; p++)
		;
	if (*p->name)
		dbfmt("%p%p", msg, p->name);
	else if (namno)
		dbfmt("%p[%us]", msg, namno);
	}

/*	dump code bytes
 */
LOCAL VOID dbbyte(p, len)
	TEXT *p;
	BYTES len;
	{
	IMPORT BITS debug;
	IMPORT COUNT expflag;
	IMPORT COUNT tabs;

	if (!debug)
		return;
	for (; len; --len, ++p)
		{
		if (!expflag && *p == ';')
			dbfmt("\n");
		else if (*p == 'K')
			dbfmt("b ");
		else if (*p == '`')
			{
			dbfmt("\n_______\n");
			tabs = 0;
			}
		else
			dbfmt("%ac", *p);
		}
	}

/*	show what table entry
 */
LOCAL VOID dbtab(tab)
	TAB *tab;
	{
	if (!tab)
		{
		dbfmt("(notab)");
		return;
		}
	dbfmt("tab: ");
	doname("", tatrnames, (tab->lattr & ATTRMASK));
	doname("|", ttypnames, (tab->lattr & TYPEMASK));
	doname("  ", tatrnames, (tab->rattr & ATTRMASK));
	doname("|", ttypnames, (tab->rattr & TYPEMASK));
	dobits("  ", tresnames, tab->res);
	if (tab->res & ESCR)
		dbfmt(" %p:#%uc ", "ESCR", *tab->str);
	else
		{
		dbfmt(" (");
		dbbyte(tab->str, lenstr(tab->str));
		dbfmt(") ");
		}
	}

/*	dump an expression
 */
VOID dbexpr(msg, p, set)
	TEXT *msg;
	FAST EXPR *p;
	RESOURCE set;
	{
	dbfmt("%p", msg);
	if (!p || p == ~0)
		{
		dbfmt("(no expr)\n");
		return;
		}
	doname(" op: ", opnames, p->op);
	dbfmt("%p%p", (*p->name) ? " name: " : "       ",  p->name);
	if (p->got || set != ~0)
		dolong(", got: ", wgnames, (ULONG)p->got);
	if (set != ~0)
		dobits(", set: ", regnames, set);
	doaddr("\n ->f", &p->f);
	if (!p->op)
		doaddr("\n ->e.v", &p->e.v);
	else
		{
		doname("\n ->e.o.ty: ", typnames, p->e.o.ty);
		doattr("\n ->e.o", p->e.o.ty, NULL, p->e.o.stsize);
		}
	dbfmt("\n");
	}

/*	formatted debug output
 */
VOID dbfmt(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT BITS debug;
	IMPORT COUNT putl();
	IMPORT FIO stderr;

	if (!debug)
		return;
	_putf(&putl, &stderr, fmt, &args);
	putc(&stderr, -1);
	}

/*	put formatted debug output
 */
VOID dbprint(fmt, args)
	TEXT *fmt, *args;
	{
	IMPORT BITS debug;
	IMPORT COUNT expflag;
	IMPORT COUNT tabs;
	FAST COUNT x;
	FAST TEXT **p;
	FAST TEXT *f;
	BYTES np;
	TEXT *sp;

	if (!debug)
		return;
	p = &args;
	++expflag;
	for (f = fmt; *f; ++f)
		{
		switch (*f)
			{
		case '\n':
			dbfmt("\n");
			break;
		case '/':
			dbfmt("\n");
			for (x = tabs; 0 < x; --x)
				dbfmt("/");
			break;
		case '}':
			--tabs;
			break;
		case '{':
			++tabs;
			break;
		case '%':
			if (f[1])
				++f;
			switch (*f)
				{
			case 'A':
				dbtree("\n", *p, ~0);
				break;
			case 'B':
				dbfmt((*p) ? "YES" : "NO ");
				break;
			case 'D':
				sp = *p++;
				np = (BYTES)*p;
				dbbyte(sp, np);
				break;
			case 'E':
				dbexpr("\n", *p, ~0);
				break;
			case 'G':
				dolong("got: ", wgnames,(ULONG) *p);
				dbfmt(", ");
				p += (sizeof (WANTS) - sizeof(*p))/ sizeof(*p);
				break;
			case 'I':
				doname("idx: ", idxnames, *p);
				dbfmt(", ");
				break;
			case 'O':
				doname("op: ", opnames, *p);
				dbfmt(", ");
				break;
			case 'R':
				if (!*p)
					dbfmt("NOREG");
				else
					dobits("reg: ", regnames, *p);
				dbfmt(", ");
				break;
			case 'S':
				if (!*p)
					dbfmt("FAILSET");
				else
					dobits("set: ", regnames, *p);
				dbfmt(", ");
				break;
			case 'T':
				if (*p)
					dbprint("\n    ");
				dbtab(*p);
				break;
			case 'Y':
				doname("ty: ", typnames, *p);
				dbfmt(", ");
				break;
			case 'W':
				dolong("want: ", wgnames, * ((ULONG *)p));
				dbfmt(", ");
				p += (sizeof (WANTS) - sizeof(*p))/ sizeof(*p);
				break;
			default:
				dbfmt("(%%%ac?)", *f);
				break;
				}
			++p;
			break;
		default:
			dbfmt("%ac", *f);
			}
		}
	--expflag;
	}

/*	dump expression tree
 */
VOID dbtree(msg, p, set)
	TEXT *msg;
	FAST EXPR *p;
	BITS set;
	{
	IMPORT BITS debug;

	if (!debug)
		return;
	dbexpr(msg, p, set);
	if (!p || p == ~0)
		return;
	if (p->op)
		{
		dbtree("\n--left ", p->e.o.left, ~0);
		if (p->e.o.mid)
			dbtree("\n--mid ", p->e.o.mid, ~0);
		if (p->op & BINOP)
			dbtree("\n--right ", p->e.o.right, ~0);
		}
	}
