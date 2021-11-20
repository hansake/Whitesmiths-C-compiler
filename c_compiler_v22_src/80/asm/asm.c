/*	GENERATOR FOR A-NATURAL
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "a.h"

LOCAL CODETAB gettab[] {
	Ir, Nn, "1L4,WR",
	IBC, A, "2",
	A, IBC, "a",
	IDE, A, "12",
	A, IDE, "1a",
	INN, HL, "22,WL",
	HL, INN, "2a,WR",
	INN, A, "32,WL",
	A, INN, "3a,WR",
	Cr, N, "6L3,R",
	Cr, Cr, "40L3R",
	SP, HL, "f9",
	BC, DE, "4b,42",
	BC, HL, "4d,44",
	DE, BC, "59,50",
	DE, HL, "5d,54",
	HL, BC, "69,60",
	HL, DE, "6b,62",
	0};

LOCAL CODETAB poptab[] {
	BC, SP, "c1",
	DE, SP, "d1",
	HL, SP, "e1",
	AF, SP, "f1",
	SP, BC, "c5",
	SP, DE, "d5",
	SP, HL, "e5",
	SP, AF, "f5",
	0};

LOCAL CODETAB swaptab[] {
	HL, ISP, "e3",
	ISP, HL, "e3",
	HL, DE, "eb",
	DE, HL, "eb",
	0};

LOCAL CODETAB plutab[] {
	HL, Ir, "9R4",
	Ir, 1, "3L4",
	Cr, 1, "4L3",
	A, Cr, "80R",
	A, N, "c6,R",
	0};

LOCAL CODETAB mintab[] {
	Ir, 1, "bL4",
	Cr, 1, "5L3",
	A, Cr, "90R",
	A, N, "d6,R",
	0};

LOCAL CODETAB pluctab[] {
	A, Cr, "88R",
	A, N, "ce,R",
	0};

LOCAL CODETAB minctab[] {
	A, Cr, "98R",
	A, N, "de,R",
	0};

LOCAL CODETAB andtab[] {
	A, Cr, "a0R",
	A, N, "e6,R",
	0};

LOCAL CODETAB xortab[] {
	A, Cr, "a8R",
	A, N, "ee,R",
	0};

LOCAL CODETAB ortab[] {
	A, Cr, "b0R",
	A, N, "f6,R",
	0};

LOCAL CODETAB cmptab[] {
	A, Cr, "b8R",
	A, N, "fe,R",
	0};

LOCAL CODETAB rottab[] {
	A, 1, "7",
	A, -1, "f",
	0};

LOCAL CODETAB rotctab[] {
	A, 1, "17",
	A, -1, "1f",
	0};

LOCAL CODETAB rototab[] {
	A, 1, "37,17",
	A, -1, "37,1f",
	0};

LOCAL CODETAB rotztab[] {
	A, 1, "b7,17",
	A, -1, "b7,1f",
	0};

LOCAL CODETAB jmptab[] {
	Jc, INN, "c3,WR",
	Jc, IHL, "e9",
	0};

LOCAL CODETAB jctab[] {
	Jc, INN, "c2L0,WR",
	0};

LOCAL CODETAB goesup[] {
	BC, HL, "71,23,70",
	DE, HL, "73,23,72",
	0};

LOCAL CODETAB getsup[] {
	BC, HL, "4e,23,46",
	DE, HL, "5e,23,56",
	0};

LOCAL CODETAB getsbya[] {
	HL, HL, "7e,23,66,6f",
	0};

LOCAL CODETAB getsnot[] {
	A, A, "2f",
	0};

/*	indexed by bincodes
 */
GLOBAL CODETAB *tabtab[] {
	gettab, poptab, swaptab, plutab, mintab, pluctab, minctab,
	andtab, xortab, ortab, cmptab, rottab, rotctab, rototab, rotztab,
	jmptab, jctab, getsup, goesup, getsbya, getsnot, 0};

LOCAL TINY jcvals[] {002, 012, 022, 032, 042, 052, 062, 072,
		000, 010, 020, 030, 040, 050, 060, 070, 013};

/*	define q as p
 */
VOID define(q, p)
	FAST TERM *q, *p;
	{
	IMPORT TERM *cobase, *dabase, *elc;
	FAST TERM *pseg;
	LEX pty;
	TEXT *msg;
	INTERN TEXT buf[LENNAME+1];

	if (!q)
		err("constant redefined");
	else
		{
		msg = NULL;
		pseg = p->base;
		pty = p->ty & TMASK;
		if (q == elc)
			{
			if (pseg != cobase && pseg != dabase)
				err("illegal .:=");
			else if (pseg != q->base)
				{
				q->val = pseg->val;
				q->base = pseg;
				putcode(XSEG);
				}
			putsp(p->val - q->val);
			}
		else if (pseg && p != elc)
			msg = "illegal definition of";
		else if (q->ty & DEFF || q->ty & TDEF && pty != INN)
			msg = "redefinition of";
		else
			{
			q->ty = q->ty & ~TMASK | DEFF | pty;
			q->val = p->val;
			q->base = p->base;
			}
		if (msg)
			{
			cpybuf(buf, q->nm, LENNAME);
			nmerr(msg, buf);
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
	COUNT n;
	LEX t;
	TERM term, *x;
COUNT temp;

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
		nmerr("illegal operands for", opname(ot));
		return;
		}
	i = 0;
	for (s = tab->code; *s; ++s)
		switch (*s)
			{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			i = (i << 4) + *s - '0';
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			i = (i << 4) + *s - ('a' - 10);
			break;
		case 'L':
			n = *++s - '0';
			i =+ putval(tab->cl, l) << n;
			break;
		case 'R':
/*			TO WORK AROUND BUG IN C68k
			i =+ putval(tab->cr, r) << (s[1] ? *++s - '0' : 0);
*/
			temp = putval(tab->cr, r);
			temp =<< (s[1] ? *++s - '0' : 0);
			i =+ temp;
			break;
		case ',':
			putbyte(i);
			i = 0;
			break;
		default:
			putword(*++s == 'L' ? l : r);
			i = -1;
			}
	if (0 <= i)
		putbyte(i);
	}

/*	do a literal operator
 */
VOID dolit(p)
	FAST TERM *p;
	{
	IMPORT LIT *littab;
	IMPORT TEXT string[];
	FAST LIT *q;
	FAST TERM *pseg;

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
		q->lbl = addsym("<LIT>");
		}
	p->ty = p->ty & ~TMASK | INN;
	p->val = 0;
	p->base = q->lbl;
	if (q->ty == STRING)
		string[0] = 0;
	}

/*	put a byte
 */
VOID putbyte(c)
	META c;
	{
	IMPORT TERM *elc;

	putcode(DATA, c);
	++elc->val;
	elc->base->val = elc->val;
	}

/*	put program footer
 */
VOID putftr()
	{
	IMPORT LIT *littab;
	IMPORT TERM *cobase, *dabase, *elc;
	FAST LIT *q;
	FAST TEXT *s;
	COUNT n;
	TERM term;
	TEXT c;

	term.ty = 0;
	term.val = 0;
	term.base = cobase;
	if (littab)
		{
		define(elc, cobase);
		for (q = littab; q; q = q->t.next)
			{
			define(q->lbl, elc);
			if (q->t.ty != STRING)
				putword(&q->t);
			else
				for (s = q->t.val, c = *s++; c; --c, ++s)
					{
					putbyte(*s);
					}
			}
		}
	putcode(END);
	relout(cobase->val, dabase->val);
	}

/*	put zeros for space
 */
VOID putsp(val)
	COUNT val;
	{
	IMPORT TERM *elc;

	if (val < 0)
		err("can't backup .");
	else
		for (; 0 < val; --val)
			putbyte(0);
	}

/* put an operand value
 */
COUNT putval(code, p)
	LEX code;
	FAST TERM *p;
	{
	IMPORT TINY crlist[], irlist[], jcvals[];
	FAST COUNT i;
	FAST LEX pty;

	pty = p->ty & TMASK;
	switch (code)
		{
	case Cr:
		return (scnstr(crlist, pty));
	case Ir:
		return (scnstr(irlist, pty));
	case Jc:
		return (jcvals[pty - CNZ]);
	case N:
		return (p->val & BYTMASK);
	default:
		err("putval!");
		return (0);
		}
	}

/*	put a word constant
 */
VOID putword(p)
	TERM *p;
	{
	IMPORT TERM *elc;

	if (p->base)
		putcode(RELOC, p->base);
	putbyte(p->val);
	putbyte(p->val >> 8);
	}
