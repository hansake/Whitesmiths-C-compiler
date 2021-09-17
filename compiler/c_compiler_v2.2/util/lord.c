/*	ORDER A LIBRARY
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

#define MAXLIN	64
#define NDEFS	10

typedef struct {
	BYTES nd;
	TEXT *ns[NDEFS];
	} DEF;

typedef struct {
	TEXT *next;
	TEXT *nm;
	} DLIST;

typedef struct {
	TEXT *next;
	TEXT *name;
	COUNT nrefs;
	DLIST *defs, *refs;
	} MOD;

/*	flags:
	-c*		define command name to precede output
	-d*^	define a def format
	-i		ignore funny lines
	-r*^	define a ref format
	-s		don't print defs and refs with mod names
 */
BOOL iflag {NO};
BOOL sflag {NO};
DEF def {NDEFS};
DEF ref {NDEFS};
TEXT *cmd {NULL};

TEXT *_pname {"lord"};

/*	default def values
 */
DEF ddef {
	4,
	NULL,
	NULL,
	NULL,
	NULL,
	"0x????????B ",
	"0x????????D ",
	"0x????????T ",
	"0x????B ",
	"0x????D ",
	"0x????T ",
	};

/*	default ref values
 */
DEF dref {
	8,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	"0x????????U ",
	"0x????U ",
	};

/*	get a def or ref line
 */
BOOL getdef(pd, qd, buf)
	DEF *pd;
	FAST DLIST **qd;
	TEXT *buf;
	{
	FAST COUNT i;
	FAST DLIST *q;
	TEXT *s;

	for (i = pd->nd; i < NDEFS; ++i)
		if (s = smatch(pd->ns[i], buf))
			{
			q = alloc(sizeof (**qd), *qd);
			*qd = q;
			q->nm = buybuf(s, lenstr(s) + 1);
			return (YES);
			}
	return (NO);
	}

/*	look for matches between lists
 */
COUNT lmatch(p, q)
	FAST DLIST *p, *q;
	{
	FAST DLIST *r;
	COUNT n;

	for (n = 0; p; p = p->next)
		for (r = q; r; r = r->next)
			if (cmpstr(p->nm, r->nm))
				++n;
	return (n);
	}

/*	read defs and refs, then order modules
 */
BOOL main(ac, av)
	BYTES ac;
	TEXT **av;
	{
	IMPORT BOOL iflag, sflag;
	IMPORT DEF def, ref;
	IMPORT TEXT *cmd;
	FAST MOD *q, **qb;
	DEF *dpnt, *rpnt;
	COUNT n;
	MOD *mods;
	TEXT buf[MAXLIN];
	INTERN BOOL ok {YES};

	getflags(&ac, &av, "c*,d*>i,r*>s:F", &cmd, &def, &iflag, &ref, &sflag);
	dpnt = (def.nd == NDEFS) ? &ddef : &def;
	rpnt = (ref.nd == NDEFS) ? &dref : &ref;
	mods = NULL;
	if (cmd)
		{
		sflag = YES;
		putstr(STDOUT, cmd, NULL);
		}
	for (qb = &mods, q = NULL; 0 < (n = getlin(buf, MAXLIN)); )
		{
		buf[n - 1] = '\0';
		if (1 < n && buf[n - 2] == ':')
			{
			q = alloc(sizeof (*q), NULL);
			q->name = buybuf(buf, n);
			q->nrefs = 0;
			q->defs = NULL;
			q->refs = NULL;
			*qb = q;
			qb = &q->next;
			}
		else if (!q)
			{
			putstr(STDERR, "no module name\n", NULL);
			exit(NO);
			}
		else if (!getdef(dpnt, &q->defs, buf) && !getdef(rpnt, &q->refs, buf)
			&& !iflag)
			{
			putstr(STDERR, "not def or ref: ", buf, "\n", NULL);
			ok = NO;
			}
		}
	if (!tsort(&mods))
		ok = NO;
	for (q = mods; q; q = q->next)
		{
		if (sflag)
			q->name[lenstr(q->name) - 1] = '\0';
		if (cmd)
			putstr(STDOUT, " ", q->name, NULL);
		else
			putstr(STDOUT, q->name, "\n", NULL);
		if (!sflag)
			{
			putdef("d: ", q->defs);
			putdef("r: ", q->refs);
			}
		}
	if (cmd)
		putstr(STDOUT, "\n", NULL);
	exit(YES);
	}

/*	put a def or ref
 */
VOID putdef(s, p)
	FAST TEXT *s;
	FAST DLIST *p;
	{
	for (; p; p = p->next)
		putstr(STDOUT, s, p->nm, "\n", NULL);
	}

/*	look for pattern match
 */
TEXT *smatch(r, s)
	FAST TEXT *r, *s;
	{
	for (; *r; ++r, ++s)
		if (*r == '?')
			{
			if (!*s || *s == '\n')
				return (NULL);
			}
		else if (*s != *r)
			return (NULL);
	return (s);
	}

/*	topologically sort modules
 */
BOOL tsort(qb)
	MOD **qb;
	{
	FAST MOD *q, *r, **rb;
	BOOL better;
	MOD **nb, *new;

	for (q = *qb; q; q = q->next)
		for (r = *qb; r; r = r->next)
			q->nrefs =+ lmatch(q->defs, r->refs);
	for (nb = &new, better = YES; *qb && better; )
		{
		better = NO;
		for (rb = qb; r = *rb; )
			if (r->nrefs)
				rb = &r->next;
			else
				{
				*nb = r;
				nb = &r->next;
				*rb = r->next;
				better = YES;
				for (q = *qb; q; q = q->next)
					q->nrefs =- lmatch(q->defs, r->refs);
				}
		}
	if (*qb)
		{
		better = NO;
		putstr(STDERR, (*nb)->name, " not completely sorted", "\n\n", NULL);
		}
	else
		better = YES;
	*nb = *qb;
	*qb = new;
	return (better);
	}
