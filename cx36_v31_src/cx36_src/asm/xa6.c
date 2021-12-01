/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - PARSER
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

/* lexical parser for g.p. x-assembler	*/
/* various #define options
 *	DOTRESW	-	all reserved words form .nnn
 */
#define	DOTRESW

#include	<std.h>
#include	"xa.h"

/* access routines for rd_buf	*/
LOCAL	TEXT	*rd_buf		={0},
		*rd_ptr		={0},
		*rd_lim		={0};
struct	ic_item	ic_stk[MAX_ICOUNT + 1]	={0};

VOID	get_rdbuf()
{
	IMPORT	TINY	ic_cnt;
	IMPORT	COUNT	lineno;
	IMPORT	FILE	cin;
	FAST	struct	ic_item	*ic;

	ic = &ic_stk[ic_cnt++];
	if(ic_cnt > MAX_ICOUNT) perror(0,"include depth exceeded");
	ic->icln = lineno;
	ic->icch = cin;
	ic->icrb = rd_buf;
	ic->icrp = rd_ptr;
	ic->icrl = rd_lim;
	ic = &ic_stk[ic_cnt];
	if(!ic->icrb)			/* can we reuse buffer	*/
		{
		ic->icrb = alloc(LINE_SIZE + MAX_READ + 1,0);
		};
	ic->icrp = ic->icrl = ic->icrb;
	rd_buf = ic->icrb;
	rd_ptr = ic->icrp;
	rd_lim = ic->icrl;
}

VOID	res_rdbuf()
{
	IMPORT	TINY	ic_cnt;
	IMPORT	FILE	cin;
	IMPORT	COUNT	lineno;
	FAST	struct	ic_item	*ic;

	if(!ic_cnt) return;
	free(rd_buf);
	ic_stk[ic_cnt].icrb = 0;
	ic = &ic_stk[--ic_cnt];
	lineno = ic->icln;
	cin = ic->icch;
	rd_buf = ic->icrb;
	rd_ptr = ic->icrp;
	rd_lim = ic->icrl;
}
VOID	init_rdbuf()
{
	IMPORT	TINY	ic_cnt;

	ic_cnt = 0;
	ic_stk[0].icrb =			/* hold for 2nd pass */
	rd_buf = rd_lim = rd_ptr = gb_large(LINE_SIZE + MAX_READ + 1);
}

/* i/p char is first looked up in lextab
   single char symbols eg + - stand for themselves
   complex symbols eg ++ have an internal rep > 0200
   class names eg name,res word have an internal rep 01 <.. <040
   	0 == ignore (+ error if desired)
	1 == start of identifier
	2 == number
	3 == string
	4 == quoted char
	5 == macro par
	6 == reserved word (.aaaa)

*/

IMPORT	TINY	ucbit;
#define	EQ(a,b)		( a == b || ( a | ucbit) == b)
#define	BEFORE(a,b)	( a | ucbit) < b
TINY lextab[] {
	'\n',	0,	0,	0,	0,	0,	0,	0,
	0,	0,	'\n',	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0206,	0003,	'#',	0001,	'%',	'&',	0004,
	'(',	')',	'*',	0200,	',',	0202,	0006,	'/',
	0002, 	0002, 	0002, 	0002, 	0002, 	0002, 	0002, 	0002,
	0002,	0002,	':',	';',	0210,	0204,	0214,	0005,
	'@',	0001,	0001,	0001,	0001,	0001,	0001,	0001,
	0001,	0001,	0001,	0001,	0001,	0001,	0001,	0001,
	0001,	0001,	0001,	0001,	0001,	0001,	0001,	0001,
	0001,	0001,	0001,	'[',	0,	']',	'^',	0001,
	'`',	0001,	0001,	0001,	0001,	0001,	0001,	0001,
	0001,	0001,	0001,	0001,	0001,	0001,	0001,	0001,
	0001,	0001,	0001,	0001,	0001,	0001,	0001,	0001,
	0001,	0001,	0001,	'{',	'|',	'}',	'~',	0	};

/* table of double char specials eg ++	*/

struct	d2chars
{	TEXT ch2;
	TINY vv2;
} d2chars[] {
	'+',	AUTOI, 	'\0',	'+',
	'-',	AUTOD, 	'\0',	'-',
	'=',	EQUIV, 	'\0',	'=',
	'!',	NE, 	'\0',	'!',
	'<',	SHL, 	'=',	LE, 	'>',	NE, 	'\0',	'<',
	'>',	SHR, 	'=',	GE,		 	'\0',	'>',
	'&',	0, 	'\0',	'&',
	'|',	0, 	'\0',	'|',
0 };

#define	NUMRESWORDS	36
struct rsw_item	reswtab[NUMRESWORDS]{
	"addr",		ADDR,
	"ascii",	ASCII,
	"byte",		BYTE,
	"define",	DEF,
	"double",	DBLE,
	"else",		ELSESYM,
	"end",		ENDSYM,
	"endif",	ENDIF,
	"endm",		ENDM,
	"endr",		ENDR,
	"equ",		EQU,
	"even",		EVEN,
	"exitm",	EXITM,
	"external",	EXT,
	"float",	FLOATS,
	"if",		IFSYM,
	"include",	INCLD,
	"lbyte",	LBYTE,
	"lfloat",	LFLOAT,
	"lineno",	LINUM,
	"list",		LIST,
	"macro",	MACRO,
	"odd",		ODD,
	"option",	OPTN,
	"org",		ORIGIN,
	"page",		PAGE,
	"processor",	PROCESSOR,
	"psect",	PSECT,
	"public",	PUBLIC,
	"repeat",	REPEAT,
	"set",		SET,
	"text",		TEXTSYM,
	"title",	TITLE,
	"ubyte",	UBYTE,
	"word",		WORD 
	};
VOID	set_token(c,v)
	BS	c;
	LVAL	v;
{
	LOCAL	TINY	xc;		/* only want 1 byte for bs*/
	LOCAL	LVAL	xv;	/* in case v is widened */

	xc = c;
	put_tbuf(&xc,sizeof xc);
	if(c < 040)
		{
		xv = v;
		put_tbuf(&xv,sizeof xv);
		};
}

BOOL	eol_flag	YES;	/* force read	*/
BS	nbs()			/* returns next lex symbol value in lv */
{
	FAST	TEXT		*t,*s;
	FAST	COUNT		c;
	LOCAL	TINY	xbs;
	LOCAL	COUNT		i;
	LOCAL	struct	d2chars		*p2;
	LOCAL	COUNT	c1,ul,ll;
	LOCAL	TEXT		*tk_ptr;
	LOCAL	TEXT		*token;

	IMPORT	BOOL	l1_flag,copying;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	BS	bs;
	IMPORT	LVAL	lv;
	IMPORT	COUNT	lex_blk;
	IMPORT	COUNT	lineno;
	IMPORT	struct	mtoken	*mactab[];
	IMPORT	struct	m_mpar_info	*pstarts;
	IMPORT	COUNT	mt_pptr,mt_fptr;
	IMPORT	TINY	m_level;
	IMPORT	TEXT	*linebuffer;
	IMPORT	COUNT	linelength;
	IMPORT	BOOL	eol_flag;
	IMPORT	TINY	ctable[];
	IMPORT	FILE	cin;
	IMPORT	struct	opitem	(*optable)[];
	IMPORT	COUNT	optb_size;


/* copy onto tmp-file ready for 2nd pass */
	if(copying)
		{
		xbs = bs;
		put_tbuf(&xbs,sizeof xbs);
		if(bs < 040) put_tbuf(&lv,sizeof lv);
		};
/* if macro expansion read from mvars
 */
	while(pstarts)
		{
		FAST	COUNT	i,j;
		FAST	struct	mtoken	*mt;
		i = pstarts->mp_mt >> MAC_OFF;
		j = pstarts->mp_mt++ & MAC_MASK;
		mt = &mactab[i][j];
		c = mt->mc_val;
		lv = mt->mc_lv;
		if(c == MPAR)
			if(lv)			/* 0 == no. of pars	*/
				{
				pstarts->mp_smt = pstarts->mp_mt;
				pstarts->mp_slb = lex_blk;
		/* must evaluate in context of previous lex blk	*/
				lex_blk = pstarts->mp_plb;
				pstarts->mp_mt = pstarts->mp[lv];
				i = pstarts->mp_mt >> MAC_OFF;
				j = pstarts->mp_mt++ & MAC_MASK;
				mt = &mactab[i][j];
				c = mt->mc_val;
				lv = mt->mc_lv;
				}
			else
				{
				c = NUMBER;
				lv = pstarts->mp[0];
				};
		if(c )
			return(bs = 
				(c == ID && F_TYPE(lv) == MAC )? MACRO_ID
								 :c);
		if(pstarts->mp_smt)
			{
			pstarts->mp_mt = pstarts->mp_smt;
			pstarts->mp_smt = 0;
			lex_blk = pstarts->mp_slb;
			}
		else
			{
			return(bs = EXITM);
			};
		};
/* otherwise read from input -- ignoring any blanks	*/
	if(eol_flag)		/* need 'read' next line	*/
		{
		linebuffer = rd_ptr;
		do {
			if(rd_ptr >= rd_lim)
				{
/* may have a partial line left in buffer
 * move it to start of buffer + then read block
 */
				for(s = rd_buf,t = linebuffer;
					t < rd_ptr;
					t++)
					*s++ = *t;
				rd_ptr = s;
				linebuffer = rd_buf;
				if((i = read(cin,s,MAX_READ)) <= 0)
					return(bs = 0);	/* EOF */
				rd_lim = s + i;
				};
			} while (*rd_ptr++ != '\n');
/* ignore trailing new line	*/
		linelength = rd_ptr - linebuffer - 1;
		tk_ptr = linebuffer;
		eol_flag = NO;
		lineno++;
		put_tbuf("\r",1);
		put_tbuf(&lineno,sizeof lineno);
		if(l1_flag)
			{
			put_tbuf("\t",1);
			put_tbuf(&linelength,sizeof linelength);
			put_tbuf(linebuffer,linelength);
			};
		};
/* ignore all spaces,tabs + any illegal chars	*/
	while(!lextab[*tk_ptr]) tk_ptr++;
	if(*tk_ptr == '\n' || *tk_ptr == ';')
		{
		eol_flag = YES;
		return(bs = '\n');
		};
/* 3 classes
 * 0..037	start of strings etc
 * 040..0177	single char character
 * 200..0377	possible multichar subtract 0200 and look in doubles
 */
	switch(c1 = (lextab[*(token = tk_ptr++)] & 0377))
		{
case 0:						/* 0 == error	*/
		perror(1,"I char");	/* temp cant occur */
		eol_flag++;
		return(bs = '\n');
case 1:						/* 1 == identifier	*/
	read_id:
		while(ctable[*tk_ptr] & (ALFA)) tk_ptr++;
		c = *tk_ptr;		/* put temp end_marker	*/
		*tk_ptr = 0;
#ifndef	DOTRESW
/* check for reserved word	*/
		ll = 0;	ul = NUMRESWORDS - 1;
		while(ul >= ll)
			{
			i = (ll + ul) >> 1;
			t = token; s = reswtab[i].rw_ptr;
			while(*t == *s || ((*t | 040) == *s))
				{
				s++;
				if(*t++ == 0)
					{
					*tk_ptr = c;
					return(bs = reswtab[i].rw_val);
					};
				}
			if((*t | 040) < *s)
				ul = i-1;
			else
				ll = i+1;
			}
#endif
/* first check if a macro (note entry location in nlist if not there)
 */
enter_id:
		lv = e_token(tk_ptr - token,token,NO);
		if(lv >= 0)			/* exists already */
			{
			*tk_ptr = c;
/* can allow some names to be synonyns for special symbols	*/
			c = F_TYPE(lv);
			return(bs = (c & 0200 ? c : (c == MAC ?MACRO_ID:ID)));
			};
/* next check if opcode for selected machine	
 * based on binary-chop using optb_size to give upper limit
 */
		ll = 0;	ul = optb_size - 1;
		while(ul >= ll)
			{
			i = (ll + ul) >> 1;
			t = token; s = (*optable)[i].opcode;
			while(EQ(*t,*s))
				{
				s++;
				if(*t++ == 0)
					{
					*tk_ptr = c;
					lv = i;
					return(bs = OP);
					};
				}
			if(BEFORE(*t,*s))
				ul = i-1;
			else
				ll = i+1;
			}
/* otherwise enter into name list	*/
		lv = e_token(tk_ptr - token,token,YES);
		*tk_ptr = c;
		return(bs = ID);
case 2:						/* 2 == number		*/
		lv = 0;
/* assume hex then go back & check
 * non valid numbers == identifiers
 */
		while (ctable[*tk_ptr] & HC) tk_ptr++;
/* hex end with H	*/
		if((c1 =(*tk_ptr | 040)) == 'h')
			{
			if((tk_ptr[1]|040) == 'l')
				{
				tk_ptr++;
				goto clong;
				};
			for(t=token;t< tk_ptr;t++)
				lv = (lv << 4) +
					((ctable[*t] & NC) ? *t - '0'
							:((*t|040) - 'a' + 10));
			tk_ptr++;
			}
/* octal	ends with O or Q	*/
		else if(c1 == 'q' || c1 == 'o')
			{
			if((tk_ptr[1]|040) == 'l')
				{
				tk_ptr++;
				goto clong;
				};
			for(t=token;t< tk_ptr;t++)
				{
				if(*t > '7') goto read_id;
				lv = (lv << 3) + (*t - '0');
				}
			tk_ptr++;
			}
/* numeric label?	eg. 123$	*/
		else if(*tk_ptr == '$')		/* numeric lab	*/
			{
			for(t=token;t< tk_ptr;t++)
				{
				if(*t > '9') goto read_id;
				lv = (lv * 10) + (*t - '0');
				};
			tk_ptr++;
			return(bs = NLABEL);
			}
		else if(c1 == 'l')	/* long */
			{
	clong:
			tk_ptr++;
			bs = LGNUM;
			goto comlong;
			}
/* real number ? - 1.0 1.0e{+-}xxx	*/
		else if(*tk_ptr == '.')
			{
			if(!( ctable[*++tk_ptr] & HC))
				goto read_id;
			while(ctable[*tk_ptr] & HC) tk_ptr++;
			if((*tk_ptr|040) == 'e'		/* decimal */
				|| (*tk_ptr|040) == 'h'	/* hex	*/
				|| (*tk_ptr|040) == 'q')/* octal */
				{
				tk_ptr++;
				if(*tk_ptr == '-' ||*tk_ptr == '+') tk_ptr++;
				if(!(ctable[*tk_ptr] & NC)) goto read_id;
				while(ctable[*tk_ptr] & NC) tk_ptr++;
				};
			bs = FPNUM;
comlong:
/*	need at least 8 bytes to hold in place translation	*/
			t = gb_small(max(tk_ptr - token + 1,8));
			lv = estring(t);
			while(token < tk_ptr) *t++ = *token++;
			*t = '\0';
			return(bs);
			}
/* allow identifiers of form 123x	*/
		else if(ctable[*tk_ptr] & (UC|LC))
				 goto read_id;
		else
/* binary end with B	note confusion with hex !	*/
			{
			if((*(tk_ptr- 1) | 040) == 'b')
				{
				for(t=token;t< tk_ptr - 1;t++)
					{
					if(*t > '1') goto read_id;
					lv = (lv << 1) + (*t - '0');
					};
				}
			else
				{
/* decimal	allow ending with D	*/
				s = (*(tk_ptr - 1) | 040) == 'd' ?
					tk_ptr - 1:tk_ptr;
				for(t=token;t< s;t++)
					{
					if(*t > '9') goto read_id;
					lv = (lv * 10) + (*t - '0');
					}
				};
			};
		return(bs = NUMBER);
case 3:						/* strings	*/
	/* allow escaped "s	- all other escapes handled by text dir */
		while(ctable[*tk_ptr] & SC)
			if(*tk_ptr++ == '\\' && *tk_ptr == '"') tk_ptr++;
/* ignore leading " and trailing "	*/
		t = gb_small(tk_ptr - token);	/* nb +1 extra	*/
		lv = estring(t);
		while(++token < tk_ptr) *t++ = *token; /* skip over 1st " */
		*t = 0;
		if(*tk_ptr == '"')
			tk_ptr++;
		else
			perror(1,"non terminated string");
		return(bs = STRNG);
case 4:						/* quotes	*/
		switch(lv = *tk_ptr++)
			{
	case '\n':				/* in case error	*/
			eol_flag++;
			perror(1,"I character");
			tk_ptr--;
			break;
	case '\\':
			lv = eschar(&tk_ptr);
			break;
	case '^':
			if(*tk_ptr == '\'')
				lv = '^';
			else if(*tk_ptr >= '@')
				lv = *tk_ptr++ & 037;
			break;
	case '/':
			if(*tk_ptr == '\'')
				lv = '/';
			else if(*tk_ptr >= '@')
				lv = *tk_ptr++ ^ 040;
			break;
			};
		if(*tk_ptr == '\'')
			tk_ptr++;
		else
			perror(1,"' missing");
		return(bs = NUMBER);
case 5:					/* ? ..	*/
		if(ctable[*tk_ptr] & NC)
			{
			lv = *tk_ptr++ - '0';
			return(bs = MPAR);
			}
		else
			goto read_id;
case 6:					/* .aaaa		*/
		if(!(ctable[*tk_ptr] & (ALFA)))	/* . */
			return(bs = '.');
#ifdef	DOTRESW
		do {tk_ptr++;} while(ctable[*tk_ptr] & (ALFA));
		c = *tk_ptr;
		*tk_ptr = 0;
		ll = 0;	ul = NUMRESWORDS - 1;
		while(ul >= ll)
			{
			i = (ul + ll) >> 1;
			t = token + 1; s = reswtab[i].rw_ptr;
			while(*t == *s || ((*t | 040) == *s))
				{
				s++;
				if(*t++ == 0)
					{
					*tk_ptr = c;
					return(bs = reswtab[i].rw_val);
					};
				}
			if((*t | 040) < *s)
				ul = i-1;
			else
				ll = i+1;
			};
#endif
/* look in name list */
		goto enter_id;
default:				/* > 0200 = try doubles	*/
		if(c1 & 0200)
			{
			for(p2 = &d2chars[c1 & 0177];p2 -> ch2;p2++)
				if(p2->ch2 == *tk_ptr)
					{
					tk_ptr++;
					return(bs = (p2->vv2 & 0377));
					};
			return(bs = (p2->vv2 & 0377));
			};
		/* else fall thru	*/
		};
	return(bs = c1);
}

COUNT	estring(t)
	TEXT	*t;
{
	IMPORT	TEXT	**strings[];
	IMPORT	COUNT	str_idx;
	if(!strings[str_idx >> STR_OFF])
		strings[str_idx >> STR_OFF] = alloc((STR_MASK +1) * sizeof t);
	strings[str_idx >> STR_OFF][str_idx & STR_MASK] = t;
	return(str_idx++);
}

COUNT	e_token(l,tt,insert)
	COUNT	l;
	TEXT	*tt;
	BOOL	insert;
{
	LOCAL	COUNT	i;
	FAST	TEXT	*f,*t;
	LOCAL	COUNT	no_collisions;
	FAST	COUNT	n;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	COUNT	siglength;

	l = min(l,siglength);		/* wipe to size */
	no_collisions = 0;
	t = tt; n = l;
	for(i = ((*t << 5) + *(t+l-1)) % NLS_SIZE;	/* start here */
		f = nlist[i].n_ptr;			/* 0 == free	*/
		i = (i + no_collisions) % NLS_SIZE)
		{
/* cases |t| < |f| -> n==0 && *f != 0
		 |t| = |f| -> n==0 && *f == 0 (match)
		 |t| > |f| -> n>0 && *f != *t due to 0 terminating f
 */
		while(n && (*f == *t++)) { n--;f++;};
		if(!n && !*f)
			{
			if(F_TYPE(i) == NRQRD && copying) S_TYPE(i,UNK);
			return(i);
			};
		t = tt; n = l;
		if((no_collisions++ << 1) >= NLS_SIZE)	/* cant find it	*/
			perror(0,"nlist O");
		};
	if(!insert) return(~ i);		/* nb allows -0	*/
	for(f = nlist[i].n_ptr = gb_small(l+1);		/* +1 for \0 */
		l;
		l--)
			*f++ = *t++;
	*f = 0;							/* terminate */
	S_VAL(i,lineno);			/* so can error unknowns */
	S_TYPE(i,copying ? UNK : NRQRD);
	return(i);
}


BS	nbs_1d()	/* returns next lexical symbol (+ debug) pass 1	*/
{
	IMPORT	BS	bs;
	IMPORT	LVAL	lv;

	nbs();
	perror(1,"token = %z",bs);
	perror(1,"value = %d",lv);
	return(bs);
}
BS	nbs_2()		/* returns nbs on 2nd pass	*/
{
	IMPORT	BOOL	pl_flag;
	IMPORT	BS	bs;
	IMPORT	LVAL	lv;
	IMPORT	COUNT	lineno;
	IMPORT	TEXT	*linebuffer;
	IMPORT	COUNT	linelength;
	LOCAL	TINY	xbs;
/* here on 2nd pass thru' -- now read from save file */
try:
	get_tbuf(&xbs,sizeof xbs);
	bs = xbs & BYTMASK;
	if(bs < 040)
		{
		if(bs == '\r')
			{
			if(!pl_flag) listline();
			get_tbuf(&lineno,sizeof lineno);
			goto try;
			}
		else if(bs == '\t')
			{
			get_tbuf(&linelength,sizeof linelength);
			get_tbuf(linebuffer,linelength);
/* note line comes complete with \n at end	*/
			pl_flag = NO;		/* not yet printed it */
			goto try;
			};
	 	get_tbuf(&lv,sizeof lv);	/* these tokens have a value	*/
		};
	return(bs);
}

BS	nbs_2d()
{
	IMPORT	BS	bs;
	IMPORT	LVAL	lv;
	nbs_2();
	perror(2,"token = %z",bs);
	perror(2,"value = %d",lv);
	return(bs);
}

/* various string handling primitives */
TINY	ctable[128] {
0,	0,	0,	0,	0,	0,	0,	0,	/* ^g */
0,	SC,	0,	0,	0,	0,	0,	0,	/* ^o */
0,	0,	0,	0,	0,	0,	0,	0,	/* ^w */
0,	0,	0,	0,	0,	0,	0,	0,
SC,	SC,	0,	SC,	XC|SC,	SC,	SC,	SC,	/* sp-quote */
SC,	SC,	SC,	SC,	SC,	SC,	XC|SC,	SC,	/* / */
NC|HC|SC, NC|HC|SC, NC|HC|SC, NC|HC|SC, NC|HC|SC, NC|HC|SC, NC|HC|SC, NC|HC|SC, /* 7 */
NC|HC|SC, NC|HC|SC,SC,	SC,	SC,	SC,	SC,	XC|SC,	/* ? */
XC|SC, UC|HC|SC, UC|HC|SC, UC|HC|SC, UC|HC|SC, UC|HC|SC, UC|HC|SC, UC|SC, /* G */
UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	/* O */
UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	UC|SC,	/* W */
UC|SC,	UC|SC,	UC|SC,	SC,	SC,	SC,	SC,	XC|SC,	/* _ */
XC|SC, LC|HC|SC, LC|HC|SC, LC|HC|SC, LC|HC|SC, LC|HC|SC, LC|HC|SC, LC|SC, /* g */
LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	/* o */
LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	LC|SC,	/* w */
LC|SC,	LC|SC,	LC|SC,	SC,	SC,	SC,	SC,	0};	/* rub */


/* test for equality ignoring any case diffs */
BOOL	equal_nc(a,b)
	TEXT	*a,*b;
{
	FAST	TEXT *c1,*c2;


	c1 = a; c2 = b;
	while( (*c1 == *c2) ||
		(((*c1 ^ 040) == *c2) && (ctable[*c1] & (UC|LC))) )
		{
		c1++;
		if(*c2++ == 0) return(YES);
		};
	return(NO);
}

/* converts string to decimal no.	*/
COUNT	get_val(s)
	FAST	TEXT	*s;
{
	FAST	COUNT	v;

	v = 0;
	while(ctable[*s] & NC)
		v = (v * 10) + (*s++ - '0');
	return(v);
}

/* converts number to string	*/
TEXT	*c_to_string(n,s)
	FAST	COUNT	n;
	FAST	TEXT	*s;
{
	FAST	COUNT	i;

	s[i = 6] = 0;
	do {
		s[--i] = ( n % 10) + '0';
		} while(n=/10);
	return(s + i);
}


