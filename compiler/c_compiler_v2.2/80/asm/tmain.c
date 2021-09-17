/*	A NATURAL TO ISIS TRANSLATOR
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "t.h"

/*	FLAGS:
	-i name: output for ISIS, not Microsoft
	-o name: send output to file name
	-s name: start address is name
 */

TEXT *_pname {"anat"};

/*	the table of keywords
 */
LOCAL PRETAB keytab[] {
	"\1a", A,
	"\1b", B,
	"\1c", C,
	"\1d", D,
	"\1e", E,
	"\1h", H,
	"\1l", L,
	"\2af", AF,
	"\2bc", BC,
	"\2cc", CC,
	"\2cm", CM,
	"\2cp", CP,
	"\2cz", CZ,
	"\2de", DE,
	"\2hl", HL,
	"\2jc", JC,
	"\2jm", JM,
	"\2jp", JP,
	"\2jz", JZ,
	"\2sp", SP,
	"\3cnc", CNC,
	"\3cnz", CNZ,
	"\3cpe", CPE,
	"\3cpo", CPO,
	"\3jmp", JMP,
	"\3jnc", JNC,
	"\3jnz", JNZ,
	"\3jpe", JPE,
	"\3jpo", JPO,
	"\4call", CALL,
	"\6public", PUBLIC,
	0};
#define NKEYS	31

/*	the table of operators
 */
LOCAL PRETAB optab[] {
	"\1!", OR,
	"\1&", AND,
	"\1*", INDIR,
	"\1+", PLUS,
	"\1,", COMMA,
	"\1-", MINUS,
	"\1:", LABEL,
	"\1=", GETS,
	"\1^", XOR,
	"\1|", OR,
	"\2+^", PLUSC,
	"\2->", GOESTO,
	"\2-^", MINUSC,
	"\2::", CMP,
	"\2:=", DEFINE,
	"\2<=", POP,
	"\2<>", SWAP,
	"\2=!", GETSNOT,
	"\2=>", PUSH,
	"\2=^", GETSUP,
	"\3->^", GOESUP,
	"\3<*>", ROT,
	"\3<+>", ROTO,
	"\3<->", ROTZ,
	"\3<^>", ROTC,
	"\3=a^", GETSBYA,
	0};
#define NOPS	26

/*	the table of predefined values
 */
LOCAL PRETAB valtab[] {
	"\2di", 0363,
	"\2ei", 0373,
	"\2in", 0333,
	"\2rc", 0330,
	"\2rm", 0370,
	"\2rp", 0360,
	"\2rz", 0310,
	"\3cmc", 0077,
	"\3daa", 0047,
	"\3hlt", 0166,
	"\3nop", 0400,
	"\3out", 0323,
	"\3ret", 0311,
	"\3rnc", 0320,
	"\3rnz", 0300,
	"\3rpe", 0350,
	"\3rpo", 0340,
	"\3stc", 0067,
	"\4rst0", 0307,
	"\4rst1", 0317,
	"\4rst2", 0327,
	"\4rst3", 0337,
	"\4rst4", 0347,
	"\4rst5", 0357,
	"\4rst6", 0367,
	"\4rst7", 0377,
	0};
#define NVALS	26

/*	file control variables:
 */
GLOBAL BOOL iflag {NO};
GLOBAL COUNT lno {1};
GLOBAL COUNT nerrors {0};
GLOBAL FIO stderr {STDERR, 0, WRITE, 0};
LOCAL TEXT *oname {NULL};
GLOBAL TEXT *sname {NULL};

/*	symbol control
 */
GLOBAL COUNT tval {0};
GLOBAL TERM *cobase {NULL};
GLOBAL TERM *dabase {NULL};
GLOBAL TERM *elc {NULL};
GLOBAL TERM *symtab {NULL};
GLOBAL TERM *tptr {NULL};
GLOBAL TEXT string[MAXSTR+1] {0};

/*	get identifier
 */
LEX getal(c)
	META c;
	{
	IMPORT COUNT tval;
	IMPORT PRETAB keytab[], valtab[];
	IMPORT TERM *symtab, *tptr;
	FAST COUNT i;
	FAST LEX t;
	FAST TERM *q;
	TEXT tname[LENNAME];

#ifdef debug
err("/getal %ac", c);
#endif
	tname[0] = c;
	for (i = 1; isal(c = gchar()) || isdig(c); )
		if (i < LENNAME)
			tname[i++] = c;
	bchar(c);
	if (t = scntab(keytab, NKEYS, tname, i))
		return (t);
	else if (tval = scntab(valtab, NVALS, tname, i))
		{
		tval =& BYTMASK;
		return (N);
		}
	else
		{
		while (i < LENNAME)
			tname[i++] = '\0';
		for (q = symtab; q; q = q->next)
			if (cmpbuf(q->nm, tname, LENNAME))
				break;
		if (!q)
			q = addsym(tname);
		if ((q->ty & TMASK) == N)
			{
			tval = q->val;
			return (N);
			}
		else
			{
			tptr = q;
			return (q->ty & TMASK);
			}
		}
	}

/*	get an operator
 */
LEX getop(c)
	META c;
	{
	IMPORT PRETAB optab[];
	FAST COUNT i;
	FAST LEX t;
	TEXT op[3];

#ifdef debug
err("/getop %ac", c);
#endif
	op[0] = c;
	op[1] = gchar();
	op[2] = gchar();
	for (i = 3; 0 < i; bchar(op[--i]))
		if (t = scntab(optab, NOPS, op, i))
			return (t);
	err("illegal operator %ac", gchar());
	return (0);
	}

/*	get an expression
 */
LEX gexpr(p, mand)
	FAST TERM *p;
	BOOL mand;
	{
	FAST LEX t, u;
	TERM term;

	t = gterm(p, mand ? "expression" : NULL);
	if (p->ty)
		for (; (t & (ISTERM|BINOP)) == BINOP; t = u)
			if (t == COMMA)
				{
				gstring(p, "x, string");
				u = gterm(&term, "string");
				gstring(&term, "string, x");
				}
			else
				{
				u = gterm(&term, "term");
				dobin(p, &term, t);
				}
	return (t);
	}

/*	concatenate a string
 */
VOID gstring(p, mes)
	FAST TERM *p;
	TEXT *mes;
	{
	IMPORT COUNT tval;
	FAST LEX t;

	switch (p->ty)
		{
	case N:
		strcat(p->val);
		break;
	case NN:
		strcat(p->val);
		strcat(p->val >> 8);
		break;
	case STRING:
		break;
	default:
		err("illegal %p", mes);
		}
	p->ty = STRING;
	}

/*	get a term
 */
LEX gterm(p, mesg)
	FAST TERM *p;
	TEXT *mesg;
	{
	IMPORT TERM *cobase, *dabase, *elc, *tptr;
	FAST LEX t;
	FAST TERM *q;
	LEX u;
	TERM term;

	p->val = 0;
	cpybuf(p->nm, "\0\0\0\0\0\0\0", LENNAME);
	while (((t = gtok()) == N || t == NN || t == INN) && (u = gtok()) == LABEL)
		define((t == N) ? NULL : tptr, elc);
#ifdef debug
err("/gterm %o", t);
#endif
	switch (t)
		{
	case N:
	case NN:
	case INN:
		q = (t == N) ? NULL : tptr;
		if (u == DEFINE)
			{
			t = gexpr(p, YES);
			define(q, p);
			p->ty =| PUBF;
			}
		else if (t == N)
			{
			p->ty = N;
			p->val = tval;
			p->base = NULL;
			t = u;
			}
		else
			{
			q->ty =| TDEF;
			p->ty = q->ty & TMASK;
			p->val = q->val;
			p->base = q->base;
			cpybuf(p->nm, q->nm, LENNAME);
			t = u;
			}
		break;
	case LPAREN:
		t = gexpr(p, YES);
		if (t != RPAREN)
			err("missing )");
		else
			t = gtok();
		break;
	default:
		if (t < 0)
			{
			p->ty = 0;
			if (mesg)
				err("missing %p", mesg);
			return (t);
			}
		else if (t & ISTERM)
			{
			p->ty = t;
			t = gtok();
			}
		else if (t & UNOP)
			{
			u = gterm(p, "operand");
			dounop(p, t);
			t = u;
			}
		else
			{
			err("token! %o", t);
			return (t);
			}
		}
	for (; t == LBRACK; t = gtok())
		{
		t = gexpr(&term, YES);
		if (t != RBRACK)
			err("missing ]");
		dosub(p, &term);
		}
	return (t);
	}

/*	PARSE A-NATURAL PROGRAM
 */
BOOL main(ac, av)
	COUNT ac;
	TEXT **av;
	{
	IMPORT BOOL iflag, outmode;
	IMPORT COUNT nerrors;
	IMPORT FIO stderr, stdin, stdout;
	IMPORT TERM *elc, *symtab;
	IMPORT TEXT *oname, *sname, string[];
	FAST LEX t, ty;
	FAST TEXT *s;
	COUNT i;
	TERM term;
	TEXT nbuf[16];

	getflags(&ac, &av, "i,o*,s*:F <files>", &iflag, &oname, &sname);
	if (!oname && 0 <ac && av[0][i = lenstr(av[0]) - 1] == '8' && i < 15)
		{
		oname = nbuf;
		cpystr(nbuf, av[0], NULL);
		nbuf[i] = outmode ? 'r' : 'm';
		}
	if (!oname)
		;
	else if ((stdout._fd = create(oname, WRITE, outmode)) < 0)
		{
		err("can't create %p", oname);
		exit(NO);
		}
	else
		{
		finit(&stdout, stdout._fd, WRITE);
		stderr._fd = STDOUT;
		}
	puthdr(oname ? oname : (0 < ac) ? av[0] : NULL);
	cobase = addsym(".text");
	dabase = addsym(".data");
	elc = addsym(".");
	elc->base = cobase;
	while (0 <= (stdin._fd = getfiles(&ac, &av, STDIN, STDERR)))
		if (stdin._fd == STDERR)
			err("can't read %p", av[-1]);
		else
			{
			finit(&stdin, stdin._fd, READ);
			while ((t = gexpr(&term, NO)) != EOF)
				{
				if (t != EOL)
					err("syntax error");
				ty = term.ty & TMASK;
				if (!ty || term.ty & PUBF)
					;
				else if (ty == N)
					putbyte(term.val);
				else if (ty == NN || ty == INN)
					putword(&term);
				else if (ty == STRING)
					for (s = string + 1; string[0]; --string[0], ++s)
						putbyte(*s);
				else
					err("illegal constant");
				}
			fclose(&stdin);
			}
	putftr(symtab);
	return (nerrors == 0);
	}

/*	convert token back to name
 */
TEXT *opname(t)
	FAST LEX t;
	{
	IMPORT PRETAB keytab[], optab[];
	FAST PRETAB *p;

	for (p = optab; p->prename; ++p)
		if (p->pretype == t)
			break;
	if (!p->prename)
		for (p = keytab; p->prename; ++p)
			if (p->pretype == t)
				break;
	return (p->prename ? p->prename + 1 : "!?");
	}
