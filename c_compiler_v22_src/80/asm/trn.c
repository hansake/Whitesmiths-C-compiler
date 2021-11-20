/*	GENERATOR FOR TRANSLATOR VERSION
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "t.h"

GLOBAL BOOL outmode {0};

/*	segment selectors
 */
LOCAL TEXT *cseg {"\tcseg\n"};
LOCAL TEXT *dseg {"\tdseg\n"};

/*	register names for output
 */
LOCAL TEXT *crname[] {"b", "c", "d", "e", "h", "l", "m", "a", "?"};
LOCAL TEXT *irname[] {"b", "d", "h", "sp", "??"};
LOCAL TEXT *jcname[] {
	"cnz", "cz", "cnc", "cc", "cpo", "cpe", "cp", "cm",
	"jnz", "jz", "jnc", "jc", "jpo", "jpe", "jp", "jm",
	"call", "jmp", "j??"};

/*	define q as p
 */
VOID define(q, p)
	FAST TERM *q, *p;
	{
	IMPORT TERM *cobase, *dabase, *elc;
	IMPORT TEXT *cseg, *dseg;
	FAST TERM *pseg;
	LEX pty;

	if (!q)
		err("constant redefined");
	else
		{
		pseg = p->base;
		pty = p->ty & TMASK;
		if (q == elc)
			{
			if (pseg != cobase && pseg != dabase)
				err("illegal .:=");
			else if (pseg != q->base)
				{
				q->base = pseg;
				putfmt((pseg == cobase) ? cseg : dseg);
				}
			putsp(p->val - q->val);
			}
		else if (pseg && p != elc)
			err("illegal definition of %.8p", q->nm);
		else if (q->ty & DEFF || q->ty & TDEF && pty != INN)
			err("redefinition of %.8p", q->nm);
		else
			{
			q->ty = q->ty & ~TMASK | DEFF | pty;
			q->val = p->val;
			q->base = p->base;
			if (p == elc)
				{
				putval(NN, q);
				putfmt(":\n");
				}
			}
		}
	}

/*	do binary operator
 */
VOID dobin(l, r, ot)
	TERM *l, *r;
	LEX ot;
	{
	IMPORT CODETAB *tabtab[];
	IMPORT LEX bincodes[];
	FAST CODETAB *tab;
	FAST COUNT i;
	FAST TEXT *s;
	LEX t;
	TERM term, *x;

	t = ot;
	if (CNZ <= t && t <= JMP)
		{
		term.ty = t;
		if (t != JMP)
			t = JCOND;
		l = &term;
		}
	l->ty =| PUBF;
	if (t == GOESTO || t == PUSH)
		{
		x = l, l = r, r = x;
		t = (t == GOESTO) ? GETS : POP;
		}
	for (i = 0; bincodes[i]; ++i)
		if (t == bincodes[i])
			break;
	if (!bincodes[i])
		{
		err("unknown binop!");
		return;
		}
	for (tab = tabtab[i]; tab->cl; ++tab)
		if (bmatch(tab->cl, l) && bmatch(tab->cr, r))
			break;
	if (!tab->cl)
		{
		err("illegal operands for %p", opname(ot));
		return;
		}
	putch('\t');
	for (s = tab->code; *s; ++s)
		switch (*s)
			{
		case 'L':
			putval(tab->cl, l);
			break;
		case 'R':
			putval(tab->cr, r);
			break;
		case ';':
			putch('\n');
			putch('\t');
			break;
		case ' ':
			putch('\t');
			break;
		default:
			putch(*s);
			}
	putch('\n');
	}

/*	do a literal operator
 */
VOID dolit(p)
	FAST TERM *p;
	{
	IMPORT LIT *littab;
	IMPORT TERM *cobase;
	IMPORT TEXT string[];
	FAST LIT *q;
	FAST TERM *pseg;
	INTERN COUNT crs;

	pseg = p->base;
	for (q = littab; q; q = q->t.next)
		if (q->ty != STRING &&
			pseg == q->t.base && p->val == q->t.val &&
			(!pseg || cmpbuf(p->nm, q->t.nm, LENNAME)))
			break;
	if (!q)
		{
		q = alloc(sizeof (*q), littab), littab = q;
		q->t.ty = p->ty;
		q->t.val = (p->ty != STRING) ? p->val : buybuf(string, string[0] + 1);
		q->t.base = pseg;
		cpybuf(q->t.nm, p->nm, LENNAME);
		q->lbl = ++crs;
		}
	p->ty = p->ty & ~TMASK | INN;
	p->val = 0;
	p->base = cobase;
	litlbl(p, q->lbl);
	if (q->ty == STRING)
		string[0] = 0;
	}

/*	make a literal label into a name
 */
TERM *litlbl(p, lbl)
	FAST TERM *p;
	FAST COUNT lbl;
	{
	FAST COUNT i;

	p->nm[0] = '_';
	p->nm[1] = '_';
	for (i = 2; i <= 4; ++i)
		{
		p->nm[i] = (lbl & 017) + 'a';
		lbl =>> 4;
		}
	return (p);
	}

/*	put a byte
 */
VOID putbyte(c)
	META c;
	{
	putfmt("\tdb %i\n", c & BYTMASK);
	}

/*	put program footer
 */
VOID putftr(p)
	FAST TERM *p;
	{
	IMPORT LIT *littab;
	IMPORT TERM *cobase;
	IMPORT TEXT *cseg, *sname;
	FAST LIT *q;
	FAST TEXT *s;
	TERM term;
	TEXT c;

	term.ty = 0;
	term.val = 0;
	term.base = cobase;
	if (littab)
		{
		putfmt(cseg);
		for (q = littab; q; q = q->t.next)
			{
			putval(INN, litlbl(&term, q->lbl));
			putfmt(": ");
			if (q->t.ty != STRING)
				putword(&q->t);
			else
				for (s = q->t.val, c = *s++; c; --c, ++s)
					{
					putbyte(*s);
					}
			}
		}
	for (; p; p = p->next)
		{
		if (p->base && p->base != cobase && p->base != dabase)
			{
			putfmt("\textrn\t");
			p->ty =| PUBF;
			}
		else if (p->ty & PUBF)
			putfmt("\tpublic\t");
		if (p->ty & PUBF)
			{
			putval(NN, p);
			putfmt("\n");
			}
		}
	if (sname)
		putfmt("\tend %p\n", sname);
	else
		putfmt("\tend\n");
	}

/*	put program header
 */
VOID puthdr(s)
	FAST TEXT *s;
	{
	IMPORT BOOL iflag;
	IMPORT TERM *cobase;
	IMPORT TEXT *cseg;
	FAST COUNT i;

	addsym("stack")->base = cobase;
	addsym("memory")->base = cobase;
	if (s)
		{
		while(s[i = instr(s, "/:]")])
			s =+ i + 1;
		putfmt(iflag ? "\tname\t%b@\n" : "\ttitle\t%b\n", s, instr(s, "."));
		}
	putfmt(cseg);
	}

/*	put zeros for space
 */
VOID putsp(val)
	COUNT val;
	{
	FAST COUNT i, j;

	if (val & 1)
		putfmt("\tdb\t0\n");
	for (i = val >> 1; 0 < i; i =- 8)
		{
		putfmt("\tdw\t0");
		for (j = min(i, 8) - 1; 0 < j; --j)
			putfmt(",0");
		putfmt("\n");
		}
	}

/* put an operand value
 */
VOID putval(code, p)
	LEX code;
	FAST TERM *p;
	{
	IMPORT BOOL iflag;
	IMPORT TEXT *crname[], *irname[], *jcname[];
	IMPORT TINY crlist[], irlist[];
	FAST COUNT i;
	FAST LEX pty;

	pty = p->ty & TMASK;
	switch (code)
		{
	case Cr:
		putfmt(crname[scnstr(crlist, pty)]);
		break;
	case Ir:
		putfmt(irname[scnstr(irlist, pty)]);
		break;
	case Jc:
		putfmt(jcname[pty - CNZ]);
		break;
	default:
		if (p->base)
			{
			for (i = 0; i < LENNAME && p->nm[i]; ++i)
				switch (p->nm[i])
					{
				case '_':
					putfmt(iflag ? "?" : ".");
					break;
				case '.':
					putfmt(iflag ? "@" : "$");
					break;
				default:
					putfmt("%ac", p->nm[i]);
					}
			if (0 < p->val)
				putfmt("+%i", p->val);
			else if (p->val < 0)
				putfmt("%i", p->val);
			}
		else if (code == N)
			putfmt("%i", p->val & BYTMASK);
		else
			putfmt("%i", p->val);
		}
	}

/*	put a word constant
 */
VOID putword(p)
	TERM *p;
	{
	putfmt("\tdw ");
	putval(NN, p);
	putfmt("\n");
	}
