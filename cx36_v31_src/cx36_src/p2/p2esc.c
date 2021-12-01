/*	ESCAPE TO FUNCTIONS FOR LAST MINUTE CODE GENERATION
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"
#include "int12.h"
#include "int012.h"

LOCAL TEXT *leftshift();
LOCAL TEXT *rightshift();
LOCAL VOID scompare();

/*	for constructing addresses and tab strings
 */
LOCAL TEXT textbuf[256] = {0};
GLOBAL TAB esctab = {0, 0, 0, textbuf};

/*	move bit field to unsigned location
 */
TAB *bitstou(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	FAST TEXT *s;
	FAST TYPE ty = p->e.o.right->f.ty;
	BITS himask, lomask, mask;
	COUNT i;
	TINY off = ty >> 4 & 077;
	TINY size = ty >> 10;

	mask = (1 << size) - 1;
	lomask = mask & BYTMASK;
	himask = (mask >> 8) & BYTMASK;
	s = cpystr(textbuf, "^mvcW L,R", NULL);
	if (off != 0)
		s = bldfmt(s, ";^src W L,", off, NULL);
	if (size < 16 - off)
		{
		if (~himask)
			s = bldfmt(s, ";^sbfB NL,", ~himask, NULL);
		if (~lomask)
			bldfmt(s, ";^sbfB NUL,", ~lomask, NULL);
		}
	esctab.res = NOCC;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	divide by immediate
 */
TAB *divimm(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	COUNT bias = (COUNT)p->e.o.right->f.bias;
	COUNT n;
	TYPE ty = p->e.o.left->f.ty;

	for (n = 0; 1 < bias; bias >>= 1)
		n++;
	rightshift(textbuf, n, ty);
	esctab.res = NOCC;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	do left shift
 */
TAB *dolsh(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	COUNT bias = (COUNT)p->e.o.right->f.bias;
	TYPE ty = p->e.o.left->f.ty;

	leftshift(textbuf, bias, ty);
	esctab.res = NOCC;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	do right shift
 */
TAB *dorsh(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	COUNT bias = (COUNT)p->e.o.right->f.bias;
	TYPE ty = p->e.o.left->f.ty;

	rightshift(textbuf, bias, ty);
	esctab.res = CCZERO;
	esctab.str = textbuf;
	return (&esctab);
	}

/* make string to do left shift
 */
LOCAL TEXT *leftshift(s, bias, ty)
	FAST TEXT *s;
	FAST COUNT bias;
	TYPE ty;
	{
	if (ty <= YUCHAR)
		bias = min(bias, 8);
	else if (ty <= YUSHORT)
		bias = min(bias, 16);
	else
		bias = min(bias, 32);
	*s = '\0';
	for (; bias;)
		{
		switch(ty)
			{
		case YCHAR:
		case YUCHAR:
			bias--;
			s = cpystr(s, "^alcB L,NL", NULL);
			break;
		case YSHORT:
		case YUSHORT:
			if (8 <= bias)
				{
				bias -= 8;
				s = cpystr(s, "^mvcB L,NUL;^mviB NUL,0", NULL);
				}
			else
				{
				bias--;
				s = cpystr(s, "^alcW L,NL", NULL);
				}
			break;
		case YLONG:
		case YULONG:
			if (24 <= bias)
				{
				bias -= 24;
				s = cpystr(s, "^mvcB L,NUUUL;^mviB NUL,0;",
					"^mviB NUUL,0;^mviB NUUUL,0", NULL);
				}
			else if (16 <= bias)
				{
				bias -= 16;
				s = cpystr(s, "^mvcW L,NUUL;",
					"^mviB NUUL,0;^mviB NUUUL,0", NULL);
				}
			else if (8 <= bias)
				{
				bias -= 8;
				s = cpystr(s, "^mvcB L,NUL;^mvcB UL,NUUL;",
					"^mvcB UUL,NUUUL;^mviB NUUUL,0", NULL);
				}
			else
				{
				bias--;
				s = cpystr(s, "^alcS L,NL", NULL);
				}
			break;
		default:
			panic("LSH");
			}
		if (bias)
			*s++ = ';';
		}
	return (s);
	}

/*	multiply by immediate
 */
TAB *mulimm(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	IMPORT TEXT textbuf[];
	COUNT bias = (COUNT)p->e.o.right->f.bias;
	COUNT n;
	TYPE ty = p->e.o.left->f.ty;

	for (n = 0; 1 < bias; bias /= 2)
		n++;
	leftshift(textbuf, n, ty);
	esctab.res = NOCC;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	move a constant to bit field
 */
TAB *ntobits(p)
	EXPR *p;
	{
	IMPORT TAB esctab;
	IMPORT TEXT textbuf[];
	FAST BITS mask = bmask(p->e.o.left->f.ty);
	FAST BITS lomask, himask, loval, hival, val;
	FAST TEXT *s;
	TINY off = (p->e.o.left->f.ty >> 4) & 077;

	val = p->e.o.right->f.bias;
	val = (val << off) & mask;
	s = textbuf;
	if (mask != val)
		{
		lomask = mask & BYTMASK;
		himask = (mask >> 8) & BYTMASK;
		if (himask)
			s = bldfmt(s, "^sbfB NL,", himask, NULL);
		if (lomask)
			s = bldfmt(s, ";^sbfB NUL,", lomask, NULL);
		}
	if (val)
		{
		loval = val & BYTMASK;
		hival = (val >> 8) & BYTMASK;
		if (hival)
			s = bldfmt(s, ";^sbnB NL,", hival, NULL);
		if (loval)
			s = bldfmt(s, ";^sbnB NUL,", loval, NULL);
		}
	esctab.res = NOCC;
	esctab.str = textbuf;
	return (&esctab);
	}

/* make string to do right shift
 */
LOCAL TEXT *rightshift(s, bias, ty)
	FAST TEXT *s;
	FAST COUNT bias;
	TYPE ty;
	{
	BOOL bigshift;
	TEXT *biasnm;

	*s = '\0';
	if (ty <= YUCHAR)
		bias = min(bias, 8);
	else if (ty <= YUSHORT)
		bias = min(bias, 16);
	else
		{
		bias = min(bias, 32);
		if (16 < bias)
			{
			bias -= 16;
			bigshift = YES;
			}
		else
			bigshift = NO;
		}
	biasnm = nn(bias, 10);
	switch (ty)
		{
	case YCHAR:
		s = cpystr(s, "^mvi @ac0+6,0;^tbn @ac0+7,128;^jf T;",
			"^mvi @ac0+6,255;T: src @ac0+7(2),", biasnm,
			NULL);
		break;
	case YUCHAR:
		s = cpystr(s, "^srcB L,", biasnm, NULL);
		break;
	case YSHORT:
		if (bias <= 8)
			s = cpystr(s, "^mvi @ac0+5,0;^tbn @ac0+6,128;^jf T;",
				"^mvi @ac0+5,255;T: src @ac0+7(3),", biasnm,
				NULL);
		else
			s = cpystr(s, "^tbn @ac0+6,128;^jf Ta;^mvc @ac0+5(2),@const+3;",
				"^j Tb;Ta: mvc @ac0+5(2),@const-1;Tb: src @ac0+7(4),", biasnm,
				NULL);
		break;
	case YUSHORT:
		s = cpystr(s, "^srcW L,", biasnm, NULL);
		break;
	case YLONG:
		if (bias <= 8 && !bigshift)
			s = cpystr(s, "^mvi @ac0+3,0;^tbn @ac0+4,128;^jf T;",
				"^mvi @ac0+3,255;T: src @ac0+7(5),", biasnm,
				NULL);
		else
			s = cpystr(s, "^tbn @ac0+4,128;^jf Ta;^mvc @ac0+3(4),@const+3;",
				"^j Tb;Ta: mvc @ac0+3(4),@const-1;Tb: src @ac0+7(8),", biasnm,
				bigshift ? ";^src @ac0+7(8),16" : "", NULL);
		break;
	case YULONG:
		s = cpystr(s, "^srcS L,", biasnm,
			bigshift ? ";^srcS L,16" : "", NULL);
		break;
	default:
		panic("RSH");
		}
	return (s);
	}

/*	signed compare with constant
 */
TAB *signicmp(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	IMPORT TEXT textbuf[];

	scompare(textbuf, p, set, YES);
	esctab.res = CCSET;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	signed compare
 */
TAB *signcmp(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	IMPORT TEXT textbuf[];

	scompare(textbuf, p, set, NO);
	esctab.res = CCSET|RVOL;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	signed compare
 */
LOCAL VOID scompare(s, p, set, rcons)
	FAST TEXT *s;
	FAST EXPR *p;
	RESOURCE set;
	BOOL rcons;
	{
	TYPE ty = p->e.o.left->f.ty;

	switch(ty)
		{
	case YCHAR:
		s = cpystr(s, "^aliB NL,128;", NULL);
		if (rcons)
			p->e.o.right->f.bias += 0x80;
		else
			s = cpystr(s, "^aliB R,128;", NULL);
		cpystr(s, "^clcB L,R", NULL);
		break;
	case YSHORT:
		s = cpystr(s, "^aliB NL,128;", NULL);
		if (rcons)
			p->e.o.right->f.bias += 0x8000;
		else
			s = cpystr(s, "^aliB R,128;", NULL);
		cpystr(s, "^clcW L,R", NULL);
		break;
	case YLONG:
		s = cpystr(s, "^aliB NL,128;", NULL);
		if (rcons)
			p->e.o.right->f.bias += 0x80000000L;
		else
			s = cpystr(s, "^aliB R,128;", NULL);
		cpystr(s, "^clcS L,R", NULL);
		break;
	default:
		panic("SIGNCMP");
		}
	if (rcons)
		mklit(p->e.o.right, ty);
	}

/*	move structure to structure
 */
TAB *strtostr(p, set)
	EXPR *p;
	FAST RESOURCE set;
	{
	IMPORT TAB esctab;
	IMPORT TEXT textbuf[];
	FAST TEXT *s;
	COUNT stsize = p->e.o.stsize;
	TEXT nnm[4];

	s = cpystr(textbuf, "^st @ac0+7,L;", NULL);
	for (;stsize;)
		{
		if (stsize <= 256)
			{
			cpystr(nnm, nn(stsize - 1, 10), NULL);
			s = cpystr(s, "^mvc ", nnm, "(", nn(stsize, 10), ",L),",
				nnm, "(,R)", NULL);
				stsize = 0;
			}
		else
			{
			s = cpystr(s, "^mvc 254(255,L),254(,R);",
				"^la 255(,L),L;^la 255(,R),R", NULL);
			stsize -= 255;
			}
		if (stsize)
			*s++ = ';';
		}
	esctab.res = NOCC;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	test bit field
 */
TAB *tstbits(p)
	FAST EXPR *p;
	{
	IMPORT TAB esctab;
	IMPORT TEXT textbuf[];
	FAST BITS mask = bmask(p->e.o.left->e.v.ty);
	FAST TEXT *s;
	BITS lomask, himask;
	TINY size = p->e.o.left->e.v.ty >> 10;

	lomask = mask & BYTMASK;
	himask = (mask >> 8) & BYTMASK;
	s = textbuf;
	if (size != 16)
		{
		if (~himask)
			s = bldfmt(s, ";^sbfB NL,", ~himask, NULL);
		if (~lomask)
			s = bldfmt(s, ";^sbfB NUL,", ~lomask, NULL);
		}
	cpystr(s, ";^clcW L,@const+3", NULL);
	esctab.res = CCZERO;
	esctab.str = textbuf;
	return (&esctab);
	}

/*	move an unsigned to bit field
 */
TAB *utobits(p, set)
	FAST EXPR *p;
	RESOURCE set;
	{
	IMPORT TAB esctab;
	IMPORT TEXT textbuf[];
	FAST BITS mask = bmask(p->e.o.left->e.v.ty);
	FAST TEXT *s, *ts;
	BITS lomask, himask;
	TINY off = (p->e.o.left->e.v.ty >> 4) & 077;
	TINY size = p->e.o.left->e.v.ty >> 10;

	lomask = mask & BYTMASK;
	himask = (mask >> 8) & BYTMASK;
	s = leftshift(textbuf, off, YUSHORT);
	for (ts = textbuf; *ts; ts++)
		if (*ts == 'L')
			*ts = 'A';
	if (size != 16)
		{
		if (himask)
			s = bldfmt(s, ";^sbfB NL,", himask, NULL);
		if (lomask)
			s = bldfmt(s, ";^sbfB NUL,", lomask, NULL);
		if (size < 16 - off)
			{
			if (~himask)
				s = bldfmt(s, ";^sbfB R,", ~himask, NULL);
			if (~lomask)
				s = bldfmt(s, ";^sbfB UR,", ~lomask, NULL);
			}
		cpystr(s, ";^alcW L,R", NULL);
		}
	else
		cpystr(s, ";^mvcW L,R", NULL);
	esctab.res = NOCC;
	esctab.str = textbuf;
	return (&esctab);
	}
