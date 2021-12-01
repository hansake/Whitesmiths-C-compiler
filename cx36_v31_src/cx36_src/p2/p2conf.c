/*	CONFIGURATION FOR PASS 2
 *	copyright (c) 1978, 1985, 1986 by Whitesmiths, Ltd.
 *	p2.36
 */
#include <std.h>
#include "int2.h"

#define HEADRTXT "/ c36 assembler code, v3.12;"

#ifdef IBMASM	/* IBM ASSEMBLER */

#define CDEF	" ENTRY %p;"
#define CEVEN	" ORG *,2,0;"
#define CPREAMB	""
#define CREF	" EXTRN %p;"
#define CREFBSS	" EXTRN %p;"
#define COMCHAR	'*'
#define LBLSTR	"$"
#define LITSTR	""
#define BLNCHAR ' '
#define COLSTR " EQU *\n"

BOOL hasdata = {NO};
BOOL hastext = {NO};
COUNT namelen = {6};
COUNT osect = {ISDATA};
TEXT hextab[17] = {"0123456789ABCDEF"};
TEXT moddata[LENNAME+2] = {""};
TEXT modtext[LENNAME+2] = {""};

/*	section switch
 */
VOID cswitch(nsect)
	COUNT nsect;
	{
	IMPORT BITS datacount;
	IMPORT BOOL hasdata, hastext, sflag;
	IMPORT FILE outfd;
	IMPORT FILE wrkfd;
	IMPORT TEXT *dofile, *ofile;
	IMPORT TEXT *getseq();
	FAST COUNT n;
	INTERN FILE savfd;
	INTERN FILE datafd = {-1};
	INTERN FILE doutfd = {-1};
	INTERN FILE textfd = {-1};
	INTERN TEXT buf[512];
	INTERN TEXT datfnm[16] = {0};
	INTERN TEXT doutfnm[16] = {0};
	INTERN TEXT txtfnm[16] = {0};

	if (nsect == 0)	/* write out tmp files */
		{
		putchr(-1);
		wrkfd = outfd;
		if (0 <= textfd)
			{
			close(textfd);
			if (!sflag || hastext)
				{
				if ((textfd = open(txtfnm, READ, 0)) < 0)
					panic("temp read");
				putthdr();
				putchr(-1);
				while (0 < (n = read(textfd, buf, sizeof (buf))))
					if (write(wrkfd, buf, n) != n)
						panic("temp write");
				close(textfd);
				puttftr();
				putchr(-1);
				}
			remove(txtfnm);
			}
		if (0 <= datafd)
			{
			close(datafd);
			if (hasdata)
				{
				if (hastext)
					{
					if (dofile)
						cpystr(doutfnm, dofile, NULL);
					else
						cpystr(doutfnm, "dat", getseq(), ".asm", NULL);
					if ((doutfd = create(doutfnm, WRITE, 0)) < 0)
						panic("data file create");
					wrkfd = doutfd;
					}
				else
					{
					close(outfd);
					if ((outfd = create(ofile, WRITE, 0)) < 0)
						panic("bad output file");
					else
						wrkfd = outfd;
					}
				if ((datafd = open(datfnm, READ, 0)) < 0)
					panic("temp read");
				putdhdr();
				putchr(-1);
				while (0 < (n = read(datafd, buf, sizeof (buf))))
					if (write(wrkfd, buf, n) != n)
						panic("temp write");
				close(datafd);
				putdftr();
				putchr(-1);
				}
			remove(datfnm);
			}
		}
	else if ((nsect == ISTEXT) || !sflag)
		{
		putchr(-1);
		if (textfd < 0)
			{
			cpystr(txtfnm, uname(), "p21", NULL);
			if ((textfd = create(txtfnm, WRITE, 0)) < 0)
				panic("temp create");
			}
		wrkfd = textfd;
		}
	else if (sflag)	/* nsect == ISDATA || nsect == ISBSS */
		{
		putchr(-1);
		if (datafd < 0)
			{
			cpystr(datfnm, uname(), "p22", NULL);
			if ((datafd = create(datfnm, WRITE, 0)) < 0)
				panic("temp create");
			}
		wrkfd = datafd;
		}
	}

/*	generate address
 */
VOID genaddr(name, num)
	TEXT *name;
	COUNT num;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 2;
	if (name[0] && num)
		putasm(" DC AL2(%p%p%i);", name,
			(num < 0) ? "-" : "+", abs(num));
	else if (name[0])
		putasm(" DC AL2(%p);", name);
	else
		putasm(" DC AL2(%i);", num);
	}

/*	generate byte
 */
VOID genbyte(num)
	COUNT num;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 1;
	putasm(" DC IL1'%i';", num & BYTMASK);
	}

/*	generate constant
 */
VOID genconst(buf, n)
	TEXT buf[];
	FAST COUNT n;
	{
	FAST COUNT i;
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += n;
	putasm(" DC XL%i'", n);
	for (i = 0; i < n ; ++i)
		{
		putchr(hextab[(buf[i] >> 4) & 0x0f]);
		putchr(hextab[buf[i] & 0x0f]);
		}
	putasm("';");
	}

/*	generate label
 */
VOID genlabel(name)
	TEXT *name;
	{
	IMPORT BOOL hasdata, hastext, sflag;
	IMPORT BITS datacount;
	IMPORT FILE outfd;
	IMPORT FILE wrkfd;
	IMPORT TEXT moddata[], modtext[];
	IMPORT TEXT *getseq();
	COUNT savsect;
	FILE savfd;

	if (osect == ISTEXT)
		hastext = YES;
	else
		hasdata = YES;
	if (name[0] == '<')
		{
		if (sflag && osect == ISTEXT)
			{
			savsect = osect;
			csect(ISDATA);
			putasm(" EXTRN %p;", name);
			csect(savsect);
			}
		if (!modtext[0] && osect == ISTEXT)
			{
			cpystr(modtext, name, NULL);
			putasm("$text:;");
			}
		else if (!moddata[0] && !datacount &&
			!modtext[0] && osect != ISTEXT)
			{
			cpystr(moddata, name, NULL);
			if (!sflag)
				putasm("$data:;");
			else
				{
				savsect = osect;
				csect(ISTEXT);
				putasm("%p EQU %p%p;", name, "D$", getseq());
				csect(savsect);
				}
			}
		else if (sflag && (osect != ISTEXT))
			{
			putasm(" ENTRY %p;", name);
			putasm("%p EQU *;", name);
			savsect = osect;
			csect(ISTEXT);
			putasm("/%p;", name);
			putchr(-1);
			savfd = wrkfd;
			wrkfd = outfd;
			/* .define in IBM ASM ?? */
			putasm(".define %p %p%p+%i;", name, "D$", getseq(), datacount);
			putchr(-1);
			wrkfd = savfd;
			csect(savsect);
			}
		else
			{
			putasm(" ENTRY %p;", name);
			putasm("%p EQU *;", name);
			}
		}
	else
		{
		putasm("%p EQU *;", name);
		if (sflag && (osect != ISTEXT))
			{
			savsect = osect;
			csect(ISTEXT);
			putasm("/%p;", name);
			putchr(-1);
			savfd = wrkfd;
			wrkfd = outfd;
			/* .define in IBM ASM ?? */
			putasm(".define %p %p%p+%i;", name, "D$", getseq(), datacount);
			putchr(-1);
			wrkfd = savfd;
			csect(savsect);
			}
		}
	}

/*	generate long
 */
VOID genlong(lo)
	LONG lo;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 4;
	putasm(" DC IL4'%l';", lo);
	}

/*	generate space
 */
VOID genspace(n)
	COUNT n;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += n;
	putasm(" DS %iCL1;", n);
	}

/*	generate a string
 */
VOID genstring(n)
	FAST COUNT n;
	{
	FAST COUNT i;
	FAST LEX c;
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += n + 1;
	while (0 <= n)
		{
		putasm(" DC XL%i'", (16 <= n) ? 16 : n + 1);
		for (i = 1; 0 <= n && i <= 16; ++i, --n)
			{
			c = (n <= 0) ? 0 : needch();
			putchr(hextab[(c >> 4) & 0x0f]);
			putchr(hextab[c & 0x0f]);
			}
		putasm("';");
		}
	}

/*	generate word
 */
VOID genword(num)
	COUNT num;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 2;
	putasm(" DC IL2'%i';", num);
	}

/*	get sequence id
 */
TEXT *getseq()
	{
	IMPORT TEXT *dseqfile;
	IMPORT TEXT *nn();
	COUNT seqno;
	FILE seqfd;
	INTERN TEXT seqid[10] = {0};

	if (seqid[0])
		return(seqid);
	if ((seqfd = open(dseqfile, 2, 1)) < 0)
		{
		seqfd = create(dseqfile, 1, 1);
		seqno = 0;
		}
	else
		{
		read(seqfd, &seqno, sizeof seqno);
		lseek(seqfd, 0L, 0);
		}
	cpystr(seqid, nn(seqno, 16), NULL);
	seqno++;
	write(seqfd, &seqno, sizeof seqno);
	return(seqid);
	}

/*	put footer
 */
VOID putftr()
	{
	cswitch(0);
	}

/*	put data footer
 */
VOID putdftr()
	{
	IMPORT BITS datacount;
	IMPORT TEXT moddata[], modtext[];

	putasm("/ %i DATA BYTES;", datacount);
	putasm(" END;");
	}

/*	put text footer
 */
VOID puttftr()
	{
	IMPORT BOOL lstflag, sflag;
	IMPORT TEXT moddata[], modtext[];

	if (!sflag && moddata[0] && modtext[0])
		{
		putasm(" ENTRY %p;", moddata);
		putasm("%p EQU $DATA;", moddata);
		}
	putasm("#XR1 EQU X'01';");
	putasm("#XR2 EQU X'02';");
	putasm("#ARR EQU X'08';");
	putasm("#IAR EQU X'10';");
	putasm("#WR4 EQU X'44';");
	putasm("#WR5 EQU X'45';");
	putasm("#WR6 EQU X'46';");
	putasm("#WR7 EQU X'47';");
	putasm(" EXTRN @ac0;");
	putasm(" EXTRN @fp;");
	putasm(" EXTRN @xr2;");
	putasm(" EXTRN @const;");
	putasm(" END;");
	}

/*	put header
 */
VOID puthdr()
	{
	}

/*	put data header
 */
VOID putdhdr()
	{
	IMPORT BOOL hastext, lstflag, sflag;
	IMPORT COUNT dcateg;
	IMPORT TEXT *dmodule, *module;
	IMPORT TEXT *getseq();
	IMPORT TEXT moddata[], modtext[];
	IMPORT TEXT *nn();

	if (lstflag)
		putasm(" OPTIONS NOXREF;");
	else
		putasm(" OPTIONS NOLIST,NOXREF;");
	putasm(" HEADERS CATG-%p;", nn(dcateg, -10));
	putasm(HEADRTXT);
	putasm("/ created with %p;", module ? module : modtext);
	if (dmodule)
		putasm("%p START 0;", dmodule);
	else if (!hastext || moddata[0])
		{
		putasm("%p START 0;", moddata);
		if (hastext)
			{
			putasm(" ENTRY %p%p;", "D$", getseq());
			putasm("%p%p EQU *;", "D$", getseq());
			}
		}
	else
		putasm("%p%p START 0;", "D$", getseq());
	}

/*	put text header
 */
VOID putthdr()
	{
	IMPORT BITS datacount;
	IMPORT BOOL lstflag, sflag;
	IMPORT COUNT categ;
	IMPORT TEXT moddata[], modtext[];
	IMPORT TEXT *module;
	IMPORT TEXT *nn();

	if (lstflag)
		putasm(" OPTIONS NOXREF;");
	else
		putasm(" OPTIONS NOLIST,NOXREF;");
	putasm(" HEADERS CATG-%p;", nn(categ, -10));
	putasm(HEADRTXT);
	if (hastext)
		{
		putasm("%p START 0;", module ? module : modtext);
		putasm(" bd $text;");
		}
	else
		putasm("%p START 0;", module ? module : moddata);
	if (sflag && datacount)
		putasm(" EXTRN %p%p;", "D$", getseq());
	}


typedef struct xref {
	struct xref *next;
	TEXT *xref;
	} XREF;

/*	memorize external reference
 */
VOID xref(s)
	FAST TEXT *s;
	{
	IMPORT TEXT cref[];
	FAST COUNT i;
	FAST XREF *p;
	COUNT n;
	INTERN TEXT name[10] = {"@"};
	INTERN XREF *root = {NULL};

	if (s)
		n = scnstr(s, ';');
	for (p = root; p; p = p->next)
		{
		i = scnstr(p->xref, ';');
		if (!s)
			{
			cpybuf(name + 1, p->xref, i);
			name[i + 1] = '\0';
			putasm(cref, name);
			}
		else if (i == n)
			{
			while (0 <= --i && s[i] == p->xref[i])
				;
			if (i < 0)
				break;
			}
		}
	if (s && !p)
		{
		root = (XREF *)alloc(sizeof (*root), root);
		root->xref = s;
		}
	}

#endif /* IBMASM */

#ifdef WSLASM /* WSL ASSEMBLER */

#define CDEF	".public %p;"
#define CEVEN	".even;"
#define CPREAMB	""
#define CREF	".external %p;"
#define CREFBSS	".external %p;"
#define COMCHAR	';'
#define LBLSTR	"L"
#define LITSTR	""
#define BLNCHAR ' '
#define PRECHAR '_'

BOOL hasdata = {NO};
BOOL hastext = {NO};
COUNT namelen = {6};
COUNT osect = {ISDATA};
TEXT hextab[17] = {"0123456789abcdef"};
TEXT moddata[LENNAME+2] = {""};
TEXT modtext[LENNAME+2] = {""};

/*	section switch
 */
VOID cswitch(nsect)
	COUNT nsect;
	{
	IMPORT BITS datacount;
	IMPORT BOOL hasdata, hastext, sflag;
	IMPORT FILE outfd;
	IMPORT FILE wrkfd;
	IMPORT TEXT *dofile, *ofile;
	IMPORT TEXT *getseq();
	FAST COUNT n;
	INTERN FILE savfd;
	INTERN FILE datafd = {-1};
	INTERN FILE doutfd = {-1};
	INTERN FILE textfd = {-1};
	INTERN TEXT buf[512];
	INTERN TEXT datfnm[16] = {0};
	INTERN TEXT doutfnm[16] = {0};
	INTERN TEXT txtfnm[16] = {0};

	if (nsect == 0)	/* write out tmp files */
		{
		putchr(-1);
		wrkfd = outfd;
		if (0 <= textfd)
			{
			close(textfd);
			if (!sflag || hastext)
				{
				if ((textfd = open(txtfnm, READ, 0)) < 0)
					panic("temp read");
				putthdr();
				putchr(-1);
				while (0 < (n = read(textfd, buf, sizeof (buf))))
					if (write(wrkfd, buf, n) != n)
						panic("temp write");
				close(textfd);
				puttftr();
				putchr(-1);
				}
			remove(txtfnm);
			}
		if (0 <= datafd)
			{
			close(datafd);
			if (hasdata)
				{
				if (hastext)
					{
					if (dofile)
						cpystr(doutfnm, dofile, NULL);
					else
						cpystr(doutfnm, "dat", getseq(), ".asm", NULL);
					if ((doutfd = create(doutfnm, WRITE, 0)) < 0)
						panic("data file create");
					wrkfd = doutfd;
					}
				else
					{
					close(outfd);
					if ((outfd = create(ofile, WRITE, 0)) < 0)
						panic("bad output file");
					else
						wrkfd = outfd;
					}
				if ((datafd = open(datfnm, READ, 0)) < 0)
					panic("temp read");
				putdhdr();
				putchr(-1);
				while (0 < (n = read(datafd, buf, sizeof (buf))))
					if (write(wrkfd, buf, n) != n)
						panic("temp write");
				close(datafd);
				putdftr();
				putchr(-1);
				}
			remove(datfnm);
			}
		}
	else if ((nsect == ISTEXT) || !sflag)
		{
		putchr(-1);
		if (textfd < 0)
			{
			cpystr(txtfnm, uname(), "p21", NULL);
			if ((textfd = create(txtfnm, WRITE, 0)) < 0)
				panic("temp create");
			}
		wrkfd = textfd;
		}
	else if (sflag)	/* nsect == ISDATA || nsect == ISBSS */
		{
		putchr(-1);
		if (datafd < 0)
			{
			cpystr(datfnm, uname(), "p22", NULL);
			if ((datafd = create(datfnm, WRITE, 0)) < 0)
				panic("temp create");
			}
		wrkfd = datafd;
		}
	}

/*	generate address
 */
VOID genaddr(name, num)
	TEXT *name;
	COUNT num;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 2;
	if (name[0] && num)
		putasm(" .addr %p%p%i;", name,
			(num < 0) ? "-" : "+", abs(num));
	else if (name[0])
		putasm(" .addr %p;", name);
	else
		putasm(" .addr %i;", num);
	}

/*	generate byte
 */
VOID genbyte(num)
	COUNT num;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 1;
	putasm(" .byte %i;", num & BYTMASK);
	}

/*	generate constant
 */
VOID genconst(buf, n)
	TEXT buf[];
	FAST COUNT n;
	{
	FAST COUNT i;
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += n;
	putasm(" .byte");
	for (i = 0; i < n ; ++i)
		{
		putchr('0');
		putchr(hextab[(buf[i] >> 4) & 0x0f]);
		putchr(hextab[buf[i] & 0x0f]);
		putchr('h');
		if (i < (n - 1))
			{
			putasm(", ");
			}
		}
	putasm(";");
	}

/*	generate label
 */
VOID genlabel(name)
	TEXT *name;
	{
	IMPORT BOOL hasdata, hastext, sflag;
	IMPORT BITS datacount;
	IMPORT FILE outfd;
	IMPORT FILE wrkfd;
	IMPORT TEXT moddata[], modtext[];
	IMPORT TEXT *getseq();
	COUNT savsect;
	FILE savfd;

	if (osect == ISTEXT)
		hastext = YES;
	else
		hasdata = YES;
	if (name[0] == '<')
		{
		if (sflag && osect == ISTEXT)
			{
			savsect = osect;
			csect(ISDATA);
			putasm(".external %p;", name);
			csect(savsect);
			}
		if (!modtext[0] && osect == ISTEXT)
			{
			cpystr(modtext, name, NULL);
			putasm(".psect _text;");
			putasm("__text:;");
			}
		else if (!moddata[0] && !datacount &&
			!modtext[0] && osect != ISTEXT)
			{
			cpystr(moddata, name, NULL);
			if (!sflag)
				putasm(".psect _data;");
			else
				{
				savsect = osect;
				csect(ISTEXT);
				putasm("%p EQU %p%p;", name, "D$", getseq());
				csect(savsect);
				}
			}
		else if (sflag && (osect != ISTEXT))
			{
			putasm(".public %p;", name);
			putasm("%p:;", name);
			savsect = osect;
			csect(ISTEXT);
			putasm("/%p;", name);
			putchr(-1);
			savfd = wrkfd;
			wrkfd = outfd;
			putasm(".define %p %p%p+%i;", name, "= ", getseq(), datacount);
			putchr(-1);
			wrkfd = savfd;
			csect(savsect);
			}
		else
			{
			putasm(".public %p;", name);
			putasm("%p:;", name);
			}
		}
	else
		{
		putasm("%p:;", name);
		if (sflag && (osect != ISTEXT))
			{
			savsect = osect;
			csect(ISTEXT);
			putasm("/%p;", name);
			putchr(-1);
			savfd = wrkfd;
			wrkfd = outfd;
			putasm(".define %p %p%p+%i;", name, "= ", getseq(), datacount);
			putchr(-1);
			wrkfd = savfd;
			csect(savsect);
			}
		}
	}

/*	generate long
 */
VOID genlong(lo)
	LONG lo;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 4;
	putasm(" .double%l;", lo);
	}

/*	generate space
 */
VOID genspace(n)
	COUNT n;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += n;
	putasm(" .byte (%i);", n);
	}

/*	generate a string
 */
VOID genstring(n)
	FAST COUNT n;
	{
	FAST COUNT i;
	FAST LEX c;
	BOOL putcomma;
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += n + 1;
	while (0 <= n)
		{
		putasm(" .byte ");
		putcomma = NO;
		for (i = 1; 0 <= n && i <= 8; ++i, --n)
			{
			if (putcomma)
				putasm(", ");
			c = (n <= 0) ? 0 : needch();
			putchr('0');
			putchr(hextab[(c >> 4) & 0x0f]);
			putchr(hextab[c & 0x0f]);
			putchr('h');
			putcomma = YES;
			}
		putasm(";");
		}
	}

/*	generate word
 */
VOID genword(num)
	COUNT num;
	{
	IMPORT BITS datacount;

	if (osect != ISTEXT)
		datacount += 2;
	putasm(" .word %i;", num);
	}

/*	get sequence id
 */
TEXT *getseq()
	{
	IMPORT TEXT *dseqfile;
	IMPORT TEXT *nn();
	COUNT seqno;
	FILE seqfd;
	INTERN TEXT seqid[10] = {0};

	if (seqid[0])
		return(seqid);
	if ((seqfd = open(dseqfile, 2, 1)) < 0)
		{
		seqfd = create(dseqfile, 1, 1);
		seqno = 0;
		}
	else
		{
		read(seqfd, &seqno, sizeof seqno);
		lseek(seqfd, 0L, 0);
		}
	cpystr(seqid, nn(seqno, 16), NULL);
	seqno++;
	write(seqfd, &seqno, sizeof seqno);
	return(seqid);
	}

/*	put footer
 */
VOID putftr()
	{
	cswitch(0);
	}

/*	put data footer
 */
VOID putdftr()
	{
	IMPORT BITS datacount;
	IMPORT TEXT moddata[], modtext[];

	putasm("/ %i data bytes;", datacount);
	putasm(" .end;");
	}

/*	put text footer
 */
VOID puttftr()
	{
	IMPORT BOOL lstflag, sflag;
	IMPORT TEXT moddata[], modtext[];

	if (!sflag && moddata[0] && modtext[0])

		{
		putasm(".public %p;", moddata);
		putasm(".define %p = __data;", moddata);
		}
	putasm(".external @ac0;");
	putasm(".external @fp;");
	putasm(".external @xr2;");
	putasm(".external @const;");
	putasm(".end;");
	}

/*	put header
 */
VOID puthdr()
	{
	}

/*	put data header
 */
VOID putdhdr()
	{
	IMPORT BOOL hastext, lstflag, sflag;
	IMPORT COUNT categ;
	IMPORT TEXT *dmodule, *module;
	IMPORT TEXT *getseq();
	IMPORT TEXT moddata[], modtext[];
	IMPORT TEXT *nn();

	putasm(".processor s36;");
	putasm("/ HEADERS CATG-%p;", nn(categ, -10));
	putasm(HEADRTXT);
	putasm("/ created with %p;", module ? module : modtext);
	if (dmodule)
		putasm("%p:;", dmodule);
	else if (!hastext || moddata[0])
		{
		putasm("%p:;", moddata);
		if (hastext)
			{
			putasm("/ ENTRY %p%p;", "D$", getseq());
			putasm("/ %p%p EQU *;", "D$", getseq());
			}
		}
	else
		putasm("/ %p%p START 0;", "D$", getseq());
	}

/*	put text header
 */
VOID putthdr()
	{
	IMPORT BITS datacount;
	IMPORT BOOL lstflag, sflag;
	IMPORT COUNT categ;
	IMPORT TEXT moddata[], modtext[];
	IMPORT TEXT *module;
	IMPORT TEXT *nn();

	putasm(".processor s36;");
	putasm("/ HEADERS CATG-%p;", nn(categ, -10));
	putasm(HEADRTXT);
	if (hastext)
		{
		putasm("%p:;", module ? module : modtext);
		putasm(" bd __text;");
		}
	else
		putasm("%p:;", module ? module : moddata);
	if (sflag && datacount)
		putasm(" EXTRN %p%p;", "D$", getseq());
	}


typedef struct xref {
	struct xref *next;
	TEXT *xref;
	} XREF;

/*	memorize external reference
 */
VOID xref(s)
	FAST TEXT *s;
	{
	IMPORT TEXT cref[];
	FAST COUNT i;
	FAST XREF *p;
	COUNT n;
	INTERN TEXT name[10] = {"@"};
	INTERN XREF *root = {NULL};

	if (s)
		n = scnstr(s, ';');
	for (p = root; p; p = p->next)
		{
		i = scnstr(p->xref, ';');
		if (!s)
			{
			cpybuf(name + 1, p->xref, i);
			name[i + 1] = '\0';
			putasm(cref, name);
			}
		else if (i == n)
			{
			while (0 <= --i && s[i] == p->xref[i])
				;
			if (i < 0)
				break;
			}
		}
	if (s && !p)
		{
		root = (XREF *)alloc(sizeof (*root), root);
		root->xref = s;
		}
	}

#endif /* WSLASM */

/*	objects maps
 */
typedef struct secmap {
	UTINY from, to;
	} SECMAP;

LOCAL SECMAP sections[] = {
	ISDATA, 0,
	ISLITS, 0,
	ISTEXT, 0,
	ISBSS, 0,
	0};

GLOBAL BOOL upcase = {YES};

/*	system dependent strings
 */
GLOBAL TEXT cdef[] = {CDEF};
GLOBAL TEXT ceven[] = {CEVEN};
GLOBAL TEXT clitstr[] = {LITSTR};
GLOBAL TEXT cpreamb[] = {CPREAMB};
GLOBAL TEXT cref[] = {CREF};
GLOBAL TEXT crefbss[] = {CREFBSS};

GLOBAL TEXT runname[] = {"@"};

/*	switch to proper section
 */
VOID csect(nsect)
	COUNT nsect;
	{
	IMPORT COUNT osect;
	IMPORT SECMAP sections[];
	FAST BYTES i;

	for (i = 0; i < 4; ++i)
		if (sections[i].from == nsect)
			{
			if (osect != sections[i].to)
				cswitch(osect = sections[i].to);
			break;
			}
	}

GLOBAL BITS datacount = {0};

/*	put assembler code
 */
BOOL putasm(fmt, args)
	TEXT *fmt;
	BYTES args;
	{
	IMPORT BOOL asmflag, inemit;
	IMPORT COUNT lineno;
	FAST TEXT *f;
	FAST BYTES *p;
	INTERN LABEL lno = {0};
	INTERN TEXT *jtype = {NULL};

	if (!asmflag)
		{
		asmflag = YES;
		if (!inemit)
			dolist(lineno);
		asmflag = NO;
		}
	p = &args;
	for (f = fmt; *f; ++f)
		switch (*f)
			{
		case '`':
			dbprint("%D", f, 1);
			return (NO);
		case ';':
			if (jtype)
				{
				putasm("%p", jtype);
				jtype = NULL;
				}
			putchr('\n');
			if (lno)
				{
#ifdef IBMASM
				putasm("%p ORG *\n", lbln(lno));
#else
				putasm("%p:\n", lbln(lno));
#endif
				lno = 0;
				}
			break;
		case ':':
#ifdef IBMASM
			putasm(COLSTR);
#else
			putchr(':');
			putchr('\n');
#endif
			break;
		case '<':
#ifndef IBMASM
			putchr(PRECHAR);
#endif
			break;
		case '/':
			putchr(COMCHAR);
			break;
		case '>':
			putasm(LBLSTR);
			break;
		case '~':
#ifndef IBMASM
			putchr(PRECHAR);
#endif
			break;
		case '#':
#ifdef IBMASM
			putchr('#');
#endif
			break;
		case '@':
#ifdef IBMASM
			putchr('@');
#else
			putchr('_');
			putchr('_');
#endif
			break;
		case '^':
			putchr(BLNCHAR);
			break;
		case '\020':
		case '\022':
		case '\024':
			break;
		case '\032':
			lno = crs();
			putasm(" %p\n^b", lbln(lno));
			break;
		case '%':
			switch (*++f)
				{
			case 'i':
				putasm(nn(*p++, -10));
				break;
			case 'l':
				putasm(nnl(*(LONG *)p, -10));
				p = (BYTES *)((LONG *)p + 1);
				break;
			case 'n':
				putasm(lbln(*p++));
				break;
			case 'o':
				putasm(nn(*p++, 8));
				break;
			case 'p':
				putasm(*(TEXT **)p);
				p = (BYTES *)((TEXT **)p + 1);
				break;
			default:
				putchr('%');
				putchr(*f);
				++p;
				}
			break;
		default:
			putchr(*f);
			}
	return (YES);
	}

LOCAL COUNT nin = {0};
LOCAL TEXT ibuf[128] = {""};
LOCAL TEXT *bnext = {NULL};

/*	back the char
 */
VOID backch(c)
	COUNT c;
	{
	IMPORT COUNT nin;
	IMPORT TEXT ibuf[], *bnext;

	if (sizeof (ibuf) <= nin)
		panic("BACKCH");
	*--bnext = c;
	++nin;
	}

/*	get a character buffered
 */
COUNT getch()
	{
	IMPORT BOOL lstflag, asmflag, keepline, preverr, errflag;
	IMPORT COUNT nin, lineno, lnmarker;
	IMPORT TEXT ibuf[], *bnext;

	if (!nin)
		nin = read(0, bnext = ibuf, sizeof (ibuf));
	if (nin <= 0)
		{
		if (!preverr)
			return (nin = -1);
		asmflag = YES;
		keepline = NO;
		lnmarker = 0;
		dolist(lineno);
		exit((errflag) ? YES : NO);
		}
	--nin;
	return (*bnext++ & 0377);
	}

/*	put a character buffered
 */
VOID putchr(c)
	COUNT c;
	{
	IMPORT FILE wrkfd;
	INTERN COUNT nout = {0};
	INTERN TEXT buf[512];
	INTERN TEXT lastc = {0};

	if (lastc == '\n' && c == '\n')
		return;
	lastc = c;
	if (nout == sizeof (buf) || c < 0 && nout)
		{
		if (write(wrkfd, buf, nout) != nout)
			{
			panic("can't write!");
			exit(NO);
			}
		nout = 0;
		}
	if (0 <= c)
#ifdef IBMASM
		if (upcase)
			buf[nout++] = toupper(c);
		else
			buf[nout++] = c;
#else
		buf[nout++] = c;
#endif
	}

/*	put mlib calls
 */
VOID putcall(bs, s)
	TEXT *bs, *s;
	{
	IMPORT TEXT runname[];

	xref(s);
	putasm("%p %p%p;", bs, runname, s);
	}

/*	set jump string
 */
TEXT *sjmp(js, s)
	TEXT *js;
	TEXT *s;
	{
	IMPORT TEXT runname[];
	INTERN TEXT buff[30] = {""};

	xref(s);
	cpystr(buff, js, " ", runname, s, ";", NULL);
	return (buff);
	}

/*	set up the xmask map
 */
VOID setmap(xmask)
	FAST BYTES xmask;
	{
	IMPORT BOOL bssflag;
	IMPORT SECMAP sections[];

	sections[0].to = (xmask & ISDATA) ? ISTEXT : ISDATA;
	sections[1].to = (xmask & ISLITS) ? ISTEXT : ISDATA;
	sections[2].to = (xmask & ISTEXT) ? ISTEXT : ISDATA;
	if (!bssflag)
		sections[3].to = (xmask & ISDATA) ? ISTEXT : ISBSS;
	else
		sections[3].to = (xmask & ISDATA) ? ISTEXT : ISDATA;
	}

