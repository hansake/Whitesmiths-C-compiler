/*	OUTPUT TABLES
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "int2.h"
#include "int012.h"
#include "int12.h"

/*	CODES FOR CONTROL BYTES:
	byte 0: left operand
	byte 1:	right operand
	byte 2:	result

	for left or right operand:
	000-014	operand must be corresponding value
	015 high byte zero
	016 low byte zero
	017	any number
	020	operand on stack
	040 pointer to operand on stack
	060 pointer in hl
	100	operand in bc, de, or hl
	160	operand in hl
	200	operand at addable address
	260	pointer in bc, de, or hl
	300	operand at loadable address
	360	operand in memory
	374	operand must be -4
	375	operand must be -3
	376	operand must be -2
	377	operand must be -1

	for result:
	001 right operand is incremented if *bc or *hl
	002	right operand is mangled if *bc or *hl
	004	NC set properly
	010 left operand is incremented if *bc or *hl
	020 left operand is mangled if *bc or *hl
	040	Z set properly
	100	no result, CC only
	200 escape to routine n (byte following)

	type codes:
	001	signed character
	002	unsigned character
	003	signed integer
	004 bit field
	005	unsigned integer
	006	signed long
	007	long bit field (not used)
	010	unsigned long
	011	float
	012	double

	CODES FOR TEXT BYTES:
	X	put left address mode
	Y	put right address mode
	Wx	treat operand as word
	Dx	down address of operand by one
	Ux	up address of operand by one
	K	put call c.
	T	put created symbol
 */

LOCAL TEXT *andtab[] {
	"\302\202\144a=X&Y",
	"\302\202\044a=X&Y->X",
	"\302\205\044a=X&Y->X",
	"\302\210\046a=X&UUY->X",
	"\305\015\144a=X&Y",
	"\305\016\155a=UX&UY",
	"\305\015\010a=X&Y->X-a->UX",
	"\305\016\055a-a->X=UX&UY->X",
	"\305\205\011a=X&Y->X=UX&UY->X",
	"\305\210\012a=X&UUY->X=UX&UY->X",
	"\050\050\000Kland",
	0};

LOCAL TEXT *cltab[] {
	"\302\000\104a=X|a",
	"\000\202\104a-a::Y",
	"\302\202\104a=X::Y",
	"\305\000\114a=UX|a",
	"\000\205\105a-a-Y=0-^UY",
	"\305\205\115a=X-Y=UX-^UY",
	"\310\000\114a=UX|a",
	"\050\050\104Klclt",
	"\311\000\114a=UX|a",
	"\312\000\114a=UX|a",
	"\052\052\104Kdcmp",
	0};

LOCAL TEXT *cmptab[] {
	"\302\000\140a=X|a",
	"\302\202\140a=X::Y",
	"\205\000\140a=X|UX",
	"\305\205\140a=X::Y jnz T=UX::UY",
	"\210\000\140a=X|UX|UX|UX",
	"\050\050\140Klcmp",
	"\211\000\140a=X|UX",
	"\212\000\140a=X|UX",
	"\052\052\140Kdcmp",
	0};

LOCAL TEXT *comtab[] {
	"\305\000\010a=X=!a->X=UX=!a->X",
	"\050\000\000Klcom",
	0};

LOCAL TEXT *divtab[] {
	"\023\023\000Kidiv",
	"\025\025\000Kudiv",
	"\046\046\000Kldiv",
	"\050\050\000Kuldiv",
	"\052\052\044Kddiv",
	0};

LOCAL TEXT *gettab[] {
	"\062\017\000X=Y",
	"\302\302\000a=Y->X",
	"\302\305\000a=Y->X",
	"\302\310\002a=UUY->X",

	"\044\025\200\001",	/* BITS = UNSIGN */

	"\105\017\000WX=WY",
	"\305\000\054a-a->X->UX",
	"\305\016\011a-a->X=UY->UX",
	"\305\301\014a=Y->X+a-^a->UX",
	"\305\302\014a=Y->X-a->UX",
	"\105\103\000WX=WY",
	"\365\163\000hl->WX",
	"\305\303\011a=Y->X=UY->UX",
	"\305\064\211\002",	/* UNSIGN = BITS */
	"\105\105\000WX=WY",
	"\365\165\000hl->WX",
	"\305\305\011a=Y->X=UY->UX",
	"\105\210\012X=UUY;UX=UY",
	"\305\310\012a=UUY->X=UY->UX",
	"\105\052\000WX<=sp=>sp=>sp;Kdtr;sp=>WX",
	"\045\052\000Kdti",

	"\310\000\064a-a->X->UX->UX->UX",
	"\310\016\021a-a->X->UX->UX=UY->UX",
	"\310\301\004a=Y->UUX+a-^a->UX->DDX->DX",
	"\310\302\024a-a->X->UX=Y->UX-a->UX",
	"\310\303\024a=UY+a-^a->X->UX=Y->UUX=DY->DX",
	"\310\305\025a-a->X->UX=Y->UX=UY->UX",
	"\310\310\022a=Y->X=UY->UX=UY->UX=UY->UX",
	"\050\052\000Kdtl",

	"\311\311\022a=Y->X=UY->UX=UY->UX=UY->UX",
	"\051\052\000Kdtf",

	"\052\023\000Kitd",
	"\052\025\000Kutd",
	"\052\046\000Kltd",
	"\052\050\000Kultd",
	"\052\051\000Kftd",
	"\052\052\000Kdtd",
	0};

LOCAL TEXT *lshtab[] {
	"\302\001\044a=X+a->X",
	"\302\002\044a=X+a+a->X",
	"\302\003\044a=X+a+a+a->X",
	"\302\004\044a=X+a+a+a+a->X",
	"\302\005\044a=X+a+a+a+a+a->X",
	"\302\006\044a=X+a+a+a+a+a+a->X",
	"\302\007\044a=X+a+a+a+a+a+a+a->X",
	"\165\001\000WX+WX",
	"\165\002\000WX+WX+WX",
	"\165\003\000WX+WX+WX+WX",
	"\165\004\000WX+WX+WX+WX+WX",
	"\165\005\000WX+WX+WX+WX+WX+WX",
	"\165\006\000WX+WX+WX+WX+WX+WX+WX",
	"\165\007\000WX+WX+WX+WX+WX+WX+WX+WX",
	"\165\010\000WX+WX+WX+WX+WX+WX+WX+WX+WX",
	"\025\025\000Kilsh",
	"\050\025\000Kllsh",
	"\050\030\000sp=>af;Kllsh",
	0};

LOCAL TEXT *mintab[] {
	"\062\001\040X-1",
	"\302\202\044a=X-Y->X",
	"\302\205\044a=X-Y->X",
	"\302\210\046a=X-UUY->X",
	"\105\001\000WX-1",
	"\105\002\000WX-1-1",
	"\305\016\015a=UX-UY->X",
	"\305\205\015a=X-Y->X=UX-^UY->X",
	"\305\210\016a=X-UUy->X=UX-^Uy->X",
	"\050\050\000Klsub",
	"\052\052\044Kdsub",
	0};

LOCAL TEXT *modtab[] {
	"\023\023\000Kimod",
	"\025\025\000Kumod",
	"\046\046\000Klmod",
	"\050\050\000Kulmod",
	"\052\052\004Kdmod",
	0};

LOCAL TEXT *negtab[] {
	"\305\000\014a=0-X->X=0-^UX->X",
	"\050\000\000Klneg",
	"\052\000\000Kdneg",
	0};

LOCAL TEXT *iortab[] {
	"\302\202\044a=X|Y->X",
	"\302\205\044a=X|Y->X",
	"\302\210\046a=X|UUY->X",
	"\305\015\000a=X|Y->X",
	"\305\016\015a=UX|UY->X",
	"\305\205\015a=X|Y->X=UX|UY->X",
	"\305\210\016a=X|UUY->X=UX|UY->X",
	"\050\050\000Klor",
	0};

LOCAL TEXT *plutab[] {
	"\062\377\040X-1",
	"\062\001\040X+1",
	"\302\202\044a=X+Y->X",
	"\302\205\044a=X+Y->X",
	"\302\210\046a=X+UUY->X",
	"\105\374\000WX-1-1-1-1",
	"\105\375\000WX-1-1-1",
	"\105\376\000WX-1-1",
	"\105\377\000WX-1",
	"\105\001\000WX+1",
	"\105\002\000WX+1+1",
	"\105\003\000WX+1+1+1",
	"\105\004\000WX+1+1+1+1",
	"\105\016\015a=UX+UY->X",
	"\105\017\015a=X+Y->X=UX+^UY->X",
	"\165\105\000WX+WY",
	"\305\016\015a=UX+UY->X",
	"\265\205\015a=X+Y->X=UX+^UY->X",
	"\050\050\000Kladd",
	"\052\052\044Kdadd",
	0};

LOCAL TEXT *rshtab[] {
	"\301\001\000a=X+a=X<^>-1->X",
	"\302\001\000a=X<->-1->X",
	"\303\001\000a=UX+a=X<^>-1->X=DX<^>-1->X",
	"\023\023\000Kirsh",
	"\305\001\000a=UX<->-1->X=DX<^>-1->X",
	"\025\025\000Kursh",
	"\046\025\000Klrsh",
	"\046\030\000sp=>af;Klrsh",
	"\050\025\000Kulrsh",
	"\050\030\000sp=>af;Kulrsh",
	0};

LOCAL TEXT *timtab[] {
	"\302\002\044a=X+a->X",
	"\302\003\044a=X+a+X->X",
	"\302\004\044a=X+a+a->X",
	"\302\005\044a=X+a+a+X->X",
	"\302\006\044a=X+a+X+a->X",
	"\302\007\044a=X+a+a+a-X->X",
	"\302\010\044a=X+a+a+a->X",
	"\302\011\044a=X+a+a+a+X->X",
	"\302\012\044a=X+a+a+X+a->X",
	"\302\013\044a=X+a+X+a+a-X->X",
	"\302\014\044a=X+a+X+a+a->X",
	"\165\002\000hl+hl",
	"\165\003\000bc=>sp=hl;hl+hl+bc;bc<=sp",
	"\165\004\000hl+hl+hl",
	"\165\005\000bc=>sp=hl;hl+hl+hl+bc;bc<=sp",
	"\165\006\000bc=>sp=hl;hl+hl+bc+hl;bc<=sp",
	"\165\007\000bc=>sp=hl;hl+hl+bc+hl+bc;bc<=sp",
	"\165\010\000hl+hl+hl+hl",
	"\165\011\000bc=>sp=hl;hl+hl+hl+hl+bc;bc<=sp",
	"\165\012\000bc=>sp=hl;hl+hl+hl+bc+hl;bc<=sp",
	"\165\013\000bc=>sp=hl;hl+hl+hl+bc+hl+bc;bc<=sp",
	"\165\014\000bc=>sp=hl;hl+hl+bc+hl+hl;bc<=sp",
	"\305\002\014a=X+a->X=UX+^a->X",
	"\305\004\014a=X+a->X=UX+^a->X=DX+a->X=UX+^a->X",
	"\025\025\000Kimul",
	"\050\050\000Klmul",
	"\052\052\044Kdmul",
	0};

LOCAL TEXT *xortab[] {
	"\302\202\044a=X^Y->X",
	"\302\205\044a=X^Y->X",
	"\302\210\046a=X^UUY->X",
	"\305\015\000a=X^Y->X",
	"\305\016\015a=UX^UY->X",
	"\305\205\015a=X^Y->X=UX^UY->X",
	"\305\210\016a=X^UUY->X=UX^UY->X",
	"\050\050\000Klxor",
	0};

/*	table selectors
 */
GLOBAL TEXT *(*bintab[])[] {timtab, divtab, modtab, plutab, mintab,
	lshtab, rshtab, andtab, xortab, iortab, comtab, negtab, cmptab, cltab,
	gettab, timtab, divtab, modtab, plutab, mintab,
	lshtab, rshtab, andtab, xortab, iortab, plutab, mintab};

/*	switch to proper section
 */
VOID csect(nsect)
	COUNT nsect;
	{
	IMPORT BYTES xmask;
	INTERN COUNT osect {1};

	nsect = (nsect & xmask) ? 1 : 0;
	if (nsect ^ osect)
		{
		putasm(nsect ? ".:=.text\n" : ".:=.data\n");
		osect = nsect;
		}
	}

/*	put assembler code
 */
BOOL putasm(fmt, args)
	TEXT *fmt, *args;
	{
	FAST TEXT *f, **p;
	TEXT buf[12];
	LABEL lbl;

	p = &args;
	for (f = fmt; *f; ++f)
		switch (*f)
			{
		case '`':
			return (NO);
		case '#':
			putch('L');
			break;
		case '$':
			putch('_');
			break;
		case '\1':
			putasm("call ");
			break;
		case '\2':
			putasm("call c.lcpy");
			break;
		case '\3':
			putasm("bc=^(hl+1+1)=>sp=^(hl-1-1-1)=>sp");
			break;
		case '\4':
			putasm("call c.dcpy");
			break;
		case '\5':
			lbl = crs();
			putasm("a=4\n%n:\nb=*(hl-1);c=*(hl-1);bc=>sp\na-1 jnz %n", lbl, lbl);
			break;
		case '\20':
		case '\21':
		case '\22':
		case '\23':
		case '\24':
			putasm("c.r");
			putch(*f + ('0' - '\20'));
			break;
		case '%':
			switch(*++f)
				{
			case 'i':
				buf[stob(buf, *p, -10)] = '\0';
				putasm(buf);
				break;
			case 'n':
				putasm(ln(*p));
				break;
			case 'o':
				putch('0');
				if (*p)
					{
					buf[stob(buf, *p, 8)] = '\0';
					putasm(buf);
					}
				break;
			case 'p':
				putasm(*p);
				break;
			default:
				panic("FMT");
				}
			++p;
			break;
		default:
			putch(*f);
			}
	return (YES);
	}

/*	get a character buffered
 */
COUNT getch()
	{
	INTERN COUNT nin {0};
	INTERN TEXT buf[128], *bnext;

	if (!nin)
		nin = read(STDIN, bnext = buf, 128);
	if (nin <= 0)
		return (nin = -1);
	--nin;
	return (*bnext++ & BYTMASK);
	}

/*	put a character buffered
 */
VOID putch(c)
	COUNT c;
	{
	IMPORT FILE outfd;
	INTERN COUNT nout {0};
	INTERN TEXT buf[128];

	if (nout == 128 || c < 0 && nout)
		{
		if (write(outfd, buf, nout) != nout)
			{
			panic("can't write!");
			exit(0);
			}
		nout = 0;
		}
	buf[nout++] = c;
	}
