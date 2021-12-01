/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include	<std.h>
#include	"xa.h"

TEXT	hex[]	"0123456789ABCDEF";
/* set parity bit - 7,8 odd or even */
LOCAL	TINY epbs[16]{1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1};

TINY	acnvrt(c)
	FAST	TEXT	c;
{
	IMPORT	BOOL	ascii_flag;

	c =& 0177;
	switch(ascii_flag)
		{
case 7:
		return(c);
case 8:
		return(c | 0200);
case 0:
		return(c | ((epbs[c>>4] ^ epbs[c & 017]) << 7));
case 1:
		return(c | ((epbs[c>>4] ^ epbs[c & 017] ^ 1)<< 7));
		}
}


struct	es_item {
	TEXT	es_c,es_v;
	};

LOCAL	struct	es_item es_map[] {
	'n',	'\n',
	't',	'\t',
	'r',	'\r',
	'b',	'\b',
	'e',	033,
	'i',	017,		/* shift in	*/
	'o',	016,		/* shift out	*/
	0};

TINY	eschar(refs)
	TEXT	**refs;		/* will update ptr to end */
{
	FAST	TEXT	*s;
	FAST	struct	es_item	*p1;
	FAST	COUNT	v;
	IMPORT	TINY	ctable[];

	for(s = *refs,p1=es_map;p1->es_c;p1++)
		if(p1->es_c == *s)
			{
			v = p1->es_v;
			(*refs)++;
			return(v);
			};
	v = 0;
	if(ctable[*s] & NC)
		while(ctable[*s] & NC)
			v = (v << 3)+(*s++ - '0');
	else if(*s >= ' ')
		v = *s++;
	*refs = s;
	return(v);
}

VOID	put_in_nametable(ref_to_presets)
	FAST	struct	presets (*ref_to_presets)[];
{
	FAST	COUNT	j;
	FAST	struct	presets	*p;
	IMPORT	struct	nlist_item	nlist[];
	LOCAL	TEXT	cpy[32];
	FAST	TEXT	*s;
	IMPORT	TINY	ucbit;
	for(p = &(*ref_to_presets)[0];p->pset_name;p++)
		{
		j = e_token(lenstr(p->pset_name),
				p->pset_name,
				YES);
		S_TYPE(j,p->pset_type);
		S_VAL(j,p->pset_value);
		};
	if(ucbit)
			{
			for(p = &(*ref_to_presets)[0];p->pset_name;p++)
				{
				cpystr(cpy,p->pset_name,NULL);
				for(s=cpy;*s;s++) *s = toupper(*s);
				j = e_token(lenstr(cpy),
					cpy,
					YES);
				S_TYPE(j,p->pset_type);
				S_VAL(j,p->pset_value);
				};
			};
}

/* returns index in nlist for generated name of form tag$blk  */
COUNT	find_numlabel(blk,tag)
	FAST	COUNT	blk,tag;
{
	LOCAL	TEXT	name[12],d1[8],d2[8];

	cpystr(name,c_to_string(tag,d1),"$",c_to_string(blk,d2),NULL);
	return(e_token(lenstr(name),name,YES));
}

LOCAL	BS	unops[]	{'+','-','~',LBYTE,UBYTE,0};

LOCAL	BS	binops[]	{'+','-','*','/','%','^','|','&',SHL,SHR,0};

BOOL	un_op(c)
	FAST	BS	c;
{
	FAST	i;

	for(i=0;unops[i];i++)
		if(unops[i] == c) return(YES);
	return(NO);
}

BOOL	bin_op(c)
	FAST	BS	c;
{
	FAST	i;

	for(i=0;binops[i];i++)
		if(binops[i] == c) return(YES);
	return(NO);
}

/* handles 1 term of expression */
LVAL	term(r)
	FAST	struct	result	*r;
{
	FAST	COUNT v;
	FAST	BS	op;
	IMPORT	LVAL	pc;
	IMPORT	BS	bs,NBS;
	IMPORT	LVAL	lv;
	IMPORT	TINY	c_psects;
	IMPORT	COUNT	lex_blk;
	IMPORT	struct	nlist_item	nlist[];

	r->r_value = r->r_offset = r->r_unk = r->r_ps = 0;
	r->r_label = -1;
	while(bs == '\n') NBS;
	if(un_op(op = bs))
		{
		NBS;
		term(r);
		if(r->r_ps)
			perror(2,"S E %z I",op);
		switch(op)
			{
	case '+':
			break;
	case '-':
			r->r_value = - r->r_value;
			break;
	case '~':
			r->r_value =^ -1;
			break;
	case LBYTE:
			r->r_value =& 0377;
			break;
	case UBYTE:
			r->r_value = (r->r_value >> 8) & 0377;
			break;
			};
		r->r_label = -1;
		return(r->r_offset = r->r_value);
		}
	else
		switch (bs)
			{
case '.':				/* == program counter	*/
			r->r_offset = r->r_value = pc;
			r->r_ps = c_psects;
			break;
case NUMBER:
		 	r->r_value = r->r_offset = lv;
			break;
case ID:
			r->r_label = lv;
			switch(F_TYPE(lv) & 07)
				{
	case UNK:			/* only on pass 1	*/
				r->r_unk++;
				break;
	case LABEL:
/* if external then before pass 2 will sort names + set
 * val field = index in symbol table
 */
				r->r_value = F_VAL(lv);
				r->r_ps = F_PS(lv);
				break;
	case VAR:
	case CONST:
				r->r_label = -1;
				r->r_value = r->r_offset = F_VAL(lv);
				break;
	default:
				perror(1,"S E - I N %n",lv);
				r->r_label = -1;
				}
			break;
case NLABEL:			/* numeric label	*/
				r->r_label = v = find_numlabel(lex_blk,lv);
				if(F_TYPE(v) == UNK)
					{
					r->r_unk++;
					r->r_ps = c_psects;
					r->r_value = pc;
					}
				else
					{
					r->r_ps = F_PS(v);
					r->r_value = F_VAL(v);
					};
				break;
case '(':
				NBS;
				expression(r);
				failifnot(')');
				return(r->r_value);
default:
				perror(1,"S E <%z> I in expression",bs);
				};
	NBS;	/* consume term */
	return(r->r_value);
}


LVAL	expression(r)
	FAST	struct	result	*r;
{
	FAST	LVAL  rel;
	FAST	BS	op;
	struct	result	r1;
	IMPORT	BS	bs,NBS;
	IMPORT	LVAL	lv;

/* nb no precedence - just work out from left to right	*/
	term(r);
	for(;;)
		{
/* '+' & '-' are different as loader can add but nothing else */
		switch(bs)
			{
case '+':				/* can't add two relative terms */
			NBS;
			term(&r1);
			if(r->r_ps && r1.r_ps)
				{
				perror(2,"R E");
				r->r_ps = 0;
				};
			r->r_offset =+ r1.r_offset;
			r->r_ps =+ r1.r_ps;
			r->r_value =+ r1.r_value;
			r->r_label = r1.r_label != -1 ? r1.r_label : r->r_label;
			r->r_unk =| r1.r_unk;
			break;
case '-':			/* can have a(rel r) - k but not k - a(rel r)
				 * a(rel r) - b(rel r) == const	*/
			NBS;
			term(&r1);
			if(r->r_ps == r1.r_ps)
				{
				r->r_ps = 0;
				r->r_label = -1;	/* none */
				r->r_offset = (r->r_value =- r1.r_value);
				break;
				};
			if(r1.r_ps)			/* a - k only	*/
				{
				r->r_ps = 0;
				perror(2,"R E");
				};
			r->r_offset =- r1.r_offset;
			r->r_value =- r1.r_value;
			break;
default:
			if(bin_op(op = bs))
				{
				NBS;
				term(&r1);
				if(r->r_ps || r1.r_ps)
					perror(2,"R E in expression");
				rel = r->r_value - r->r_offset;
				switch(op)
					{
	case '*':
					r->r_offset =* r1.r_offset;
					break;
	case '/':
					r->r_offset =/ r1.r_offset;
					break;
	case '%':
					r->r_offset =% r1.r_offset;
					break;
	case '^':
					r->r_offset =^ r1.r_offset;
					break;
	case '&':
					r->r_offset =& r1.r_offset;
					break;
	case '|':
					r->r_offset =| r1.r_offset;
					break;
	case SHL:
					r->r_offset =<< r1.r_offset;
					break;
	case SHR:
					r->r_offset =>> r1.r_offset;
					break;
					};
				r->r_value = rel + r->r_offset;
				}
			else
				{
/*
printf("expression returns %d label %d value %d offset %d unk %d\n",
		r->r_value,r->r_label,r->r_value,r->r_offset,r->r_unk);
*/
				return(r->r_value);
				};
			};
		};
}

LVAL	const_expression()
{
	struct	result	r;

	expression(&r);
/* error if either not absolute or involves a label
 * (strictly speaking unk is not required but may be in future
 */
	if(r.r_ps ||
		r.r_unk)
		perror(2,"constant X");
	return(r.r_value);
}
/* check number lies between limits	*/
LVAL	check(i,ll,ul)
	FAST	LVAL	i,ll,ul;
{
	if(i < ll || i > ul)
		perror(2,"F O");
	return(i);
}

LVAL	checkbyte(i)
	FAST	LVAL	i;
{
	FAST	COUNT	ii;
	ii = i;
	if(!(-128 <= ii && ii < 256))
		perror(2,"F O");
	return(i);
}


BOOL	in_sequence(frst,scnd)	/* compares as 16 bit unsigned values	*/
	FAST	COUNT	frst,scnd;	/* BYTES ?	*/
{
	return(frst < 0 ?	(scnd < 0 ? frst <= scnd : NO)
			:	(scnd < 0 ? YES : frst <= scnd) );
}
VOID	failifnot(wanted)
	FAST	BS	wanted;
{
	IMPORT	BS	bs,NBS;
	if(bs  == wanted)
		NBS;
	else
		{
		set_token(wanted,0);	/* make sure its there for 2nd */
		perror(1,"S E %z X",wanted);
		};
}

BOOL	consume(what)
	FAST	BS	what;
{
	IMPORT	BS	bs,NBS;
	if(bs == what)
		{
		NBS;			/* consume it	*/
		return(YES);
		};
	return(NO);
}
VOID	skip_past(what)
	FAST	BS	what;
{
	while(bs && !consume(what)) {NBS;};
}

LOCAL	TEXT	*err_xpsn[26] {
	"can\'t",		/* A	*/
	"branch",		/* B	*/
	"op-code",		/* C	*/
	"define",		/* D	*/
	"error",		/* E	*/
	"field",		/* F	*/
	"register",		/* G	*/
	"address",		/* H	*/
	"invalid",		/* I	*/
	"mode",			/* J	*/
	"create",		/* K	*/
	"label",		/* L	*/
	"too many",		/* M	*/
	"identifier",		/* N	*/
	"overflow",		/* O	*/
	"processor",		/* P	*/
	"open",			/* Q	*/
	"relocation",		/* R	*/
	"syntax",		/* S	*/
	"type",			/* T	*/
	"unsupported",		/* U	*/
	"unknown",		/* V	*/
	"wrong",		/* W	*/
	"expected",		/* X	*/
	"system",		/* Y	*/
	"symbol"};		/* Z	*/

VOID	perror(p,s,a)
	COUNT	p;
	TEXT	*s;
	ARGINT	a;
{
	LOCAL	BOOL	 fatal;
	FAST	COUNT i;
	FAST	TEXT	*s1,*s2;
	LOCAL	FILE sav_ch;
	LOCAL	TEXT	message[64];
	IMPORT	BOOL	ep_flag;
	IMPORT	BOOL	pass;
	IMPORT	COUNT	e_count;
	IMPORT	COUNT	lineno,f_name;
	IMPORT	FILE	cout,err_ch;
	IMPORT	TEXT	*_pname,*file_names[];
	IMPORT	TINY	ctable[];
	IMPORT	struct	ic_item	ic_stk[];
	IMPORT	TINY	ic_cnt;

	fatal = 0;
	if(!p)
		fatal++;
	else
		if((p - 1) != pass) return;
	sav_ch = cout;
	if(e_count == 0)	/* 1st time thru - make error file if necs */
		{
		if(ep_flag < 0)			/* errors to file */
			err_ch = make_new_file(file_names[0],
					XERR,
					message,
					0);
		if(ep_flag > 0)
			err_ch = STDOUT;		/* errors to standard o/p	*/
		cout = err_ch;
		printf("%s (%1d):\n",_pname,pass + 1);
		};
	cout = err_ch;				/* nb otherwise default to 2 */
/* give file names	*/
	if(f_name > 15)
		printf("unknown file name");
	else
		printf("%-12s:",file_names[F_NO(lineno)]);
	if(ic_cnt)
 		for(i=0;i<ic_cnt;i++)
			printf("%4d:",L_NO(ic_stk[i].icln));
	printf("%4d\t",L_NO(lineno));
	for(s1 = message;*s;s++)
		if(ctable[*s] & UC)
			s1 = cpystr(s1,err_xpsn[*s - 'A'],NULL);
		else
			*s1++ = *s;
	*s1++ = '\n';
	*s1 = 0;
	printf(message,a);
	e_count++;
	cout = sav_ch;
	if(fatal) cleanup();
}
/* printable form of all resvered words/symbols	*/
struct	rsw_item	pnames[]{
	"Name",		ID,
	"Number",	NUMBER,
	"String",	STRNG,
	"Macro Parameter",	MPAR,
	"Macro Name",	MACRO_ID,
	"OpCode",	OP,
	"$-label",	NLABEL,
	"fp number",	FPNUM,
	"long number",	LGNUM,
	"BOB",		BOB,
	"BIF",		BIF,
	"EOB",		EOB,
	"EIF",		EIF,
	"EOF",		0,
	"\\n",		'\n',	/* newline	*/
	"++",		AUTOI,
	"--",		AUTOD,
	"<<",		SHL,
	">>",		SHR,
	"!=",		NE,
	"<=",		LE,
	">=",		GE,
	"&&",		LOGAND,
	"||",		LOGOR,
	"==",		EQUIV,
	0};


/* version of printf -- o/p channel chosen via cout */
VOID	printf(s,a)
	TEXT	*s;
	ARGINT	a;
{
	FAST	COUNT i, c;
	FAST	TEXT *sptr;
	LOCAL	ARGINT	*p;
	LOCAL	COUNT fw;			/* field width */
	LOCAL	BOOL	left_adj;	/* left adjust field ?	*/
	LOCAL	COUNT j;
	LOCAL	TEXT	t, n[8];
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	struct	rsw_item	reswtab[];
	IMPORT	FILE	cout;
	IMPORT	TEXT	hex[];

	p = &a;		/* ptr to arguments (if any)	*/
	while(c = *s++)
		if(c == '%')
			{
			fw = 0;		/* if non 0 == trunc */
			left_adj = NO;
			if((c = *s++) == '-')
				{
				left_adj++;
				c = *s++;
				};
			while((c >= '0') && (c <= '9'))
				{
				fw = (fw * 10) + c - '0';
				c = *s++;
				};
			switch(c)
				{
	case 's':			/* string	*/
				sptr = *(TEXT **)p++;
				goto p_name;
	case 'n':			/* name table (nlist) entry	*/
				sptr = nlist[*p++].n_ptr;
		p_name:			/* stnrd entry to print string	*/
				j = sptr ? lenstr(sptr) : 0;
				if(left_adj)
					for(i=0;i<fw-j;i++)
						write(cout," ",1);
				write(cout,sptr,j);
				if(! left_adj)
					for(i=0;i<fw-j;i++)
						write(cout," ",1);
				continue;
	case 'c':			/* character	*/
				t = *p++;
				write(cout,&t,1);
				continue;
	case 'd':			/* decimal	*/
				if((c = *p++) < 0)
					{
					write(cout,"-",1);
					c = -c;
					};
				for(i=4;i>=0;i--)
					{
					n[i] = c%10 + '0';
					c =/ 10;
					}
				i = 5;
		pnum:			/* common entry to print num */
				c = fw?fw:i;
				write(cout,&n[i-c],c);
				continue;
	case 'x':			/* hex */
				c = *p++;
				for(i=3;i>=0;i--)
					{
					n[i] = hex[c & 017];
					c =>> 4;
					}
				i = 4;
				goto pnum;
	case 'o':			/* octal	*/
				c = *p++;
		poct:
				for(i=5;i>=0;i--)
					{
					n[i] = (c & 07) + '0';
					c =>> 3;
					}
				i = 6;
				goto pnum;
	case 'z':			/* internal symbol */
				c = *p++;
				if(c >= ' ' && c < 0177)
					{
					t = c;
					write(cout,&t,1);
					}
				else
		/* look it up */
					{
					for(i=0;reswtab[i].rw_ptr;i++)
						if(c == reswtab[i].rw_val)
							{
							sptr = reswtab[i].rw_ptr;
							goto p_name;
							}
					for(i=0;pnames[i].rw_ptr;i++)
						if(c==pnames[i].rw_val)
							{
							sptr=pnames[i].rw_ptr;
							goto p_name;
							}
					goto poct;	/* cant find it	*/
					}
				continue;
	default:
				t = c;
				write(cout,&t,1);
				continue;
				};
			}
		else
			{
			t = c;
			write(cout,&t,1);
			};
}

