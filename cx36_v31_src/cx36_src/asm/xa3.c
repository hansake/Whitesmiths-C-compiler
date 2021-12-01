/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - PASS 1 DRIVER
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include	<std.h>
#include	"xa.h"

VOID x_set();

/* table driven parser - picks up handler from p1handlers
 *	sets copying etc. reads next bs + calls handler
 * assumes each subsection has read until 1st symbol after
 * its handle
 */
struct	p1h_item	{
	VOID	(*p1h_code)();
	BOOL	p1h_copy;
	};
IMPORT	VOID
	xy_addr(), x_ascii(), xy_byte(), xy_long(), x_def(), x_else(),
	x_endif(), x_end(), x_even(), x_exitm(),
	x_ext(), x_if(), x_incld(), x_linum(),
	x_list(), x_macro(), x_option(), x_origin(), x_page(), x_proc(),
	x_psect(), x_public(), x_repeat(), xy_text(), x_title(), xy_word(),
	rpt_error();

LOCAL	struct	p1h_item	p1handler[]	{
&rpt_error,	NO,	/* AUTOD 0200			*/
&rpt_error,	NO,	/* AUTOI 0201			*/
&xy_addr,	YES,	/* ADDR	0202	 addr(ess)	*/
&x_ascii,	YES,	/* ASCII	0203	 ascii 7/8 odd/even	*/
&xy_byte,	YES,	/* BYTE	0204	 byte	*/
&xy_long,	YES,	/* DBLE	0205	 double 	*/
&x_def, 	NO,	/* DEF	0206	 def(ine)	*/
&x_else,	NO,	/* ELSESYM	0207	 else	*/
&x_endif,	NO,	/* ENDIF	0210	 endif*/
&rpt_error,	NO,	/* ENDM	0211	 endm(acro)	*/
&rpt_error,	NO,	/* ENDR	0212	 endr(epeat)	*/
&x_end, 	YES,	/* ENDSYM	0213	 end	*/
&rpt_error,	NO,	/* EQU	0214	 equ			*/
&rpt_error,	NO,	/* EQUIV	0215	 equivalent (==)	*/
&x_even,	YES,	/* EVEN	0216	 even	*/
&x_exitm,	NO,	/* EXITM	0217	 exitm(acro)	*/
&x_ext, 	NO,	/* EXT	0220	 ext(ernal)	*/
&rpt_error,	NO,	/* GE	0221	 greater than or eq	*/
&x_if,  	NO,	/* IFSYM	0222	 if	*/
&x_incld,	NO,	/* INCLD	0223	 include	*/
&rpt_error,	NO,	/* LBYTE	0224	 l(ower)byte	*/
&rpt_error,	NO,	/* LE	0225	 less than or equal	*/
&x_linum,	NO,	/* LINUM	0226	 lineno	*/
&x_list,	YES,	/* LIST	0227	 list	*/
&rpt_error,	NO,	/* LOGAND	0230	 logical and && */
&rpt_error,	NO,	/* LOGOR	0231	 logical or ||	*/
&x_macro,	NO,	/* MACRO	0232	 macro	*/
&rpt_error,	NO,	/* NE	0233	 not equal	*/
&rpt_error,	NO,	/* ODD	0234	 odd		*/
&x_option,	YES,	/* OPTN	0235	 opt(io)n	*/
&x_origin,	YES,	/* ORIGIN	0236	 or(i)g(en)	*/
&x_page,	YES,	/* PAGE	0237	 page	*/
&x_proc,	NO,	/* PROCESSOR	0240	 processor	*/
&x_psect,	YES,	/* PSECT	0241	 p(rogram) sect(ion)	*/
&x_public,	NO,	/* PUBLIC	0242	 public (definition)	*/
&x_repeat,	NO,	/* REPEAT	0243	 repeat	*/
&x_set,  	YES,	/* SET	0244	 set	*/
&rpt_error,	NO,	/* SHL	0245	SHift Left	*/
&rpt_error,	NO,	/* SHR	0246	SHift Right	*/
&xy_text,	YES,	/* TEXTSYM	0247	 text	*/
&rpt_error,	NO,	/* UBYTE	0250	 u(pper) byte	*/
&xy_word,	YES,	/* WORD	0251	 word */
&xy_long,	YES,	/* LFLOAT	0252	 l(ong) float	*/
&xy_long,	YES,	/* FLOATS	0253	 4 bytes	*/
&x_title,	NO,	/* TITLE	0254	 title	*/
0};

BOOL	parse1()			/* 1st pass	*/
{

	FAST	LVAL	i;
	FAST	COUNT	j,k;
	IMPORT	BITS	list_mode;
	IMPORT	BOOL	copying;
	IMPORT	BOOL	eol_flag;
	IMPORT	BOOL	end_flag;
	IMPORT	BS	bs,NBS;
	IMPORT	COUNT	lex_blk,n_lex_blk;
	IMPORT	COUNT	sdi_count;
	IMPORT	FILE	cin;
	IMPORT	LVAL	lv;
	IMPORT	LVAL	pc;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	struct	p1h_item	p1handler[];
	IMPORT	struct	psect_item	psects[];
	IMPORT	TINY	c_psects;
	IMPORT	TINY	ic_cnt;
	IMPORT	VOID	(*assemble)();

	copying = NO;		/* init to prevent junk in tmp file	*/
	NBS;				/* read 1st to kick off	*/
	copying = YES;			/* default	*/
	end_flag = NO;
	FOREVER
		{
/* printf("dealing with %z\n",bs); */
		if(bs < 040)
			switch(bs)
			{
case 0:						/* EOF !! */
/* if eof then test if this was an included file */
			if(ic_cnt)
				{
				close(cin);
				res_rdbuf();
				bs	= EIF;		/* dummy internal EOF */
				eol_flag = YES;
				if(! ic_cnt) set_l1flag(list_mode & ~LIST_include);
				NBS;
				break;
				};
			psects[c_psects].ps_high = pc;
			psects[c_psects].ps_osdi = sdi_count;
			return(end_flag);
case NLABEL:			/* numeric labels	*/
			if(F_TYPE(k = find_numlabel(lex_blk,(COUNT)lv)) == UNK)
				{
				S_TYPE(k,LABEL);
				S_VAL(k,pc);
				S_PS(k,c_psects);
				set_sdinum(k, sdi_count);
				}
			else
				perror(1,"%d$ already Dd",lv);
			NBS;
			failifnot(':');
			break;
case MACRO_ID:			/* macro expansion	*/
			copying = NO;
			xpndmacro(lv);
			copying = YES;
			break;
case ID:					/* label or exrpsn ? */
			k = lv;			/* save id	*/
			NBS;
			if(consume(EQU))	/* EQU ?	*/
				{
				i = const_expression();
				if(F_TYPE(k) == VAR || q_undefined(k))
					{
					S_TYPE(k,VAR);
					S_VAL(k,i);
					};
				continue;
				};
			if(!consume(':'))
				if(bs == OP)		/* prob missing ':'	*/
					failifnot(':');
				else
					{
					perror(1,"V C %n",k);
					skip_past('\n');
					continue;
					};
			lex_blk = n_lex_blk++;
			i = F_TYPE(k) & 077;
			if((i & ~PUB_T) == UNK ||
				i == (LABEL|EXT_T) ||
			   q_undefined(k))
				{
				if(i & EXT_T)
					S_TYPE(k,LABEL|PUB_T);
				else
					S_TYPE(k,(i & PUB_T)|LABEL);
				S_VAL(k,pc);
				S_PS(k,c_psects);
				set_sdinum(k, sdi_count);
				};
			break;
case '\n':					/* newline */
			NBS;
			break;
case OP:
			(*assemble)((COUNT)lv);
			break;
default:
			rpt_error(bs);
			copying = NO;
			NBS;
			copying = YES;
			}
		else if( bs & 0200)
			{
			k = bs & 077;
			copying = p1handler[k].p1h_copy;
			if(bs != EXITM) NBS;
			(*p1handler[k].p1h_code)(k | 0200);
			copying = YES;
			}
		else
			{
			copying = NO;
			rpt_error(bs);
			NBS;
			copying = YES;
			}
	};
}
VOID	x_end()		/* end	*/
{
	IMPORT	LVAL	st_address;
	LOCAL	struct	result	rs;

	psects[c_psects].ps_high = pc;
	psects[c_psects].ps_osdi = sdi_count;
	if(bs && bs  != '\n')
		{
		expression(&rs);
		st_address = rs.r_value;	/* nb not correct */
		};
	copying = NO;
	while(bs) NBS;		/* until EOF	*/
	end_flag++;
}

/* exitm special as may need to get next bs from
 *	different environment hence test for exit in p1
 */
VOID	x_exitm()		/* either .exitm or end of macro */
{
	IMPORT	COUNT	lex_blk,n_lex_blk;
	IMPORT	COUNT	mt_pptr;
	IMPORT	struct	m_mpar_info	*pstarts;
	IMPORT	TINY	iflevel;
	IMPORT	TINY	m_level;

	if(pstarts)			/* are we within macro	*/
		{
		set_token(EOB,lex_blk = pstarts->mp_plb);
		iflevel = pstarts->mp_pif;
	/* repeat or macro ?	*/
		if(pstarts->mp_rpt && --(pstarts->mp[0]) )
			{
			set_token(BOB,lex_blk = n_lex_blk++);
			pstarts->mp_mt = pstarts->mp_rpt;
			}
		else
			{
			mt_pptr = pstarts->mp[1];	/* restore ptr */
			pstarts = pstarts->mp_pst;
			m_level--;
			}
		}
	else
		rpt_error(EXITM);
	NBS;				/* get from restored environment */
}

VOID	x_def()		/* def(ine)	*/
{
	FAST	LVAL	i;
	FAST	COUNT	k,t;
	do {
		while(consume('\n')) {};
		if(bs == ID) 
			{
			k = lv;
			t = F_TYPE(k) & 077;
			NBS;
			failifnot('=');
			i = const_expression();
			if((t & ~PUB_T) == UNK ||
				q_undefined(k))
				{
				S_TYPE(k,(t & PUB_T)|CONST);
				S_VAL(k,i);
				};
			};
		} while(consume(','));
}

VOID	x_ext()		/* ext(ernal)	*/
{
	FAST	COUNT	j,k;
	IMPORT	TINY	c_psects;
	k = c_psects;		/* default = current psect	*/
	do {
	extl:
		while(consume('\n')) {};
		if(bs == ID)
			{
			j = F_TYPE(lv) & 07;
			if(j == PSCT)
				{
				k = F_VAL(lv);
				NBS;
				goto extl;
				};
			if(j == LABEL || j == CONST)
				S_TYPE(lv,j|PUB_T);
			else
				if(q_undefined((COUNT)lv))
					{
					S_TYPE(lv,LABEL | EXT_T);
					S_VAL(lv,0);
					S_PS(lv,k);
					};
			NBS;
			};
	} while(consume(','));
}

VOID	x_if()		/* if	*/
{
	FAST	LVAL	i;
	FAST	BOOL	v;
	IMPORT	BITS	list_mode;
	IMPORT	BOOL	c_if[];
	IMPORT	TINY	iflevel;

	i = const_expression();
	switch(bs)
		{
case ODD:	NBS;v = i&1;break;
case EVEN:	NBS;v = !(i&1);break;
case '=':	NBS;v = (i == const_expression());break;
case NE:	NBS;v = (i != const_expression());break;
case '>':	NBS;v = (i > const_expression());break;
case GE:	NBS;v = (i >= const_expression());break;
case '<':	NBS;v = (i < const_expression());break;
case LE:	NBS;v = (i <= const_expression());break;
default:	v = i;
		};
	if(++iflevel >= MAX_IF_LEVEL)
		perror(0,"%z depth exceeded",IFSYM);
	c_if[iflevel] = v;
	if(! v)
		{
		set_l1flag(list_mode | LIST_if);
		while(bs && bs != ELSESYM && bs != ENDIF)
			skip_ifs();
		set_l1flag(list_mode & ~LIST_if);
		};
}
VOID	x_else()
{
	if(iflevel)
		{
		if(c_if[iflevel])
			{
			set_l1flag(list_mode | LIST_if);
			do
				{
				skip_ifs();
				} while(bs != ENDIF && bs);
			set_l1flag(list_mode & ~LIST_if);
			}
		}
	else
		rpt_error(ELSESYM);
}

VOID	x_endif()
{
	if(iflevel)
		iflevel--;
	else
		rpt_error(ENDIF);
}

VOID	x_incld()		/* include	*/
{
	LOCAL	TEXT	*name;
	IMPORT	TEXT	**strings[];
	if(bs == STRNG)
		{
		name = strings[lv >> STR_OFF][lv & STR_MASK];
		NBS;
		if(include_file(name,NO))
			{
			set_l1flag(list_mode | LIST_include);
			set_token(BIF,0);
			}
		else
			perror(1,"can\'t find %s",name);
		}
	else
		perror(1,"file name X");
}

VOID	x_linum()		/* lineno (#) file_name	*/
{
	FAST	LVAL	i;
	IMPORT	COUNT	f_name,lineno;
	IMPORT	TEXT	*file_names[];
	IMPORT	TEXT	**strings[];

	i = const_expression();
	if(bs == STRNG)
		{
		if(lineno == 1) --f_name;	/* 1st line overrides */
		file_names[f_name] = strings[lv >> STR_OFF][lv & STR_MASK];
		if(f_name > 15)
			perror(1,"too many files used non fatal E",NULL);
		lineno = SF_NO(f_name++,0);
		NBS;
		};
	lineno = SF_NO(F_NO(lineno),i);
}

VOID	x_macro()
{
	FAST	BS	j;
	FAST	LVAL	i,k;
	IMPORT	struct	opitem	(*optable)[];
	if(!((bs == ID && q_undefined(lv)) ||
		 bs == MACRO_ID || bs == OP))
		{
		perror(1,"I macro N");
		skip_past(ENDM);
		return;
		};
	j = bs;k = lv;
/* store away definition - enter name of macro at end to
 * avoid any recursive definitions	*/
	NBS;
	i = getmbody(ENDM);
	if(j == OP) k = e_token(lenstr((*optable)[k].opcode),
					(*optable)[k].opcode,
					YES);
	S_TYPE(k,MAC);
	S_VAL(k,i);
}

VOID	x_proc()		/* processor	*/
{

	if(bs == ID)
		{
		set_up_proc(nlist[lv].n_ptr);
		nlist[lv].n_ptr = 0;		/* remove name	*/
		NBS;
		}
	else
		perror(0,"U P");
}

VOID	x_public()				/* define as known	*/
{
	FAST	COUNT	k;
	do {
		while(consume('\n')) {};
		if(bs == ID)
			{
			k = F_TYPE(lv) & 07;
			if(k == LABEL || k == UNK || k == CONST)
				S_TYPE(lv,k|PUB_T);
			else
				perror(1,"%n W T",lv);
			NBS;
			};
		} while(consume(','));
}

VOID	x_set()		/* set	*/
{
	FAST	LVAL	i;
	FAST	COUNT	k;
	LOCAL	struct	result	rs;

	do {
		while(consume('\n'))  {};
		if(bs == ID) 
			{
			k = lv;
			NBS;
			failifnot('=');
			i = expression(&rs);	/* nb prob wrong on 1st pass	*/
			if(F_TYPE(k) == VAR ||
				q_undefined(k))
				{
				S_TYPE(k,VAR);
				S_VAL(k,i);
				};
			};
		} while(consume(','));
}

VOID	x_title()
{
	FAST	LVAL	k;
	IMPORT	TEXT	*title_s;
	IMPORT	TEXT	**strings[];
	k = lv;
	if(consume(STRNG))
		title_s = strings[k >> STR_OFF][k & STR_MASK];
}

VOID	x_even()		/* even	*/
{
	if(pc&1) emit(0);
}

VOID	x_origin()		/* or(i)g(in)	*/
{
	FAST	LVAL	i;
	LOCAL	struct	result	rs;
	FAST	TINY	cps;
	IMPORT BOOL	rl_flag;
	IMPORT	TINY	c_psects;

	cps = rl_flag ? c_psects : ABS_PS;
	i = expression(&rs);
	if(rs.r_ps != cps)
		perror(1,"new origin incompatible with current psect");
	else if(rl_flag && !in_sequence(pc,i))
		perror(1,"can\'t move origin backwards");
	else
		move_pc_by(i - pc);
}

VOID	x_page()		/* page	*/
{
	consume(STRNG);
}

VOID	rpt_error(sym)
	BS	sym;
{
	perror(1,"S E %z unX",sym);
}

VOID	x_psect()
{
	IMPORT	TINY	c_psects,f_psects;

/* dosnt allow user declaration of new psects
 * if rqrd must amend code after checking type
 */
	if(bs != ID || ((F_TYPE(lv) & 07) != PSCT))
		{
		perror(1,"psect name required");
		return;
		};
	psects[c_psects].ps_high = pc;
	psects[c_psects].ps_osdi = sdi_count;
	c_psects = F_VAL(lv);
	pc = psects[c_psects].ps_high;
	sdi_count = psects[c_psects].ps_osdi;
	NBS;
}
VOID	xpndmacro(i)
	FAST	COUNT	i;
{
	FAST	struct	m_mpar_info	*p2;
	FAST	BS	end_char;
	IMPORT	BS	bs,NBS;
	IMPORT	struct	m_mpar_info	*pstarts;
	IMPORT	struct	m_mpar_info	mp_starts[];
	IMPORT	TINY	m_level;

	if(m_level >= MAX_MLEVEL)
		 perror(0,"%z depth exceeded", MACRO);
	p2 = &mp_starts[m_level++];
	p2 -> mp_plb = lex_blk;
	p2->mp_pif = iflevel;
	p2->mp_mt = F_VAL(i);	/* start of body */
	p2->mp_smt = 0;
	p2->mp_rpt = 0;
	p2->mp_pst = pstarts;
	NBS;
	end_char = consume('{') ? '}' : '\n';
	getmpars(end_char,&p2->mp[0]);
/* set marker for 2nd pass	*/
/* macro evaluated in a new lex block	*/
	set_token(BOB,lex_blk = n_lex_blk++);
	pstarts = p2;		/* get next from macro	*/
	failifnot(end_char);
}
VOID	x_repeat()
{
	FAST	struct	m_mpar_info	*p2;
	FAST	COUNT	i;
	IMPORT	BS	bs,NBS;
	IMPORT	struct	m_mpar_info	*pstarts;
	IMPORT	struct	m_mpar_info	mp_starts[];
	IMPORT	TINY	m_level;
	IMPORT	TINY	iflevel;

	if(m_level >= MAX_MLEVEL)
		 perror(0,"%z depth exceeded", REPEAT);
	p2 = &mp_starts[m_level++];
	p2->mp_plb = lex_blk;
	p2->mp_pif = iflevel;
	p2->mp_smt = 0;
	p2->mp_pst = pstarts;
	consume('[');
	i  = const_expression();
	consume(']');
	getmpars(consume('{') ? '}' : '\n',&p2->mp[0]);
	p2->mp[0] = i;
	NBS;
	p2->mp_mt = p2->mp_rpt = getmbody(ENDR);
	if(p2->mp[0])		/* 0 == no repeats	*/
		{
		set_token(BOB,lex_blk = n_lex_blk++);
		pstarts = p2;
		}
	else
		m_level--;
}

COUNT	getmbody(end_char)
	BS	end_char;
{
	FAST	COUNT	i,j;
	FAST	struct	mtoken	*mt;
	IMPORT	COUNT	mt_fptr;
	IMPORT	struct	mtoken	*mactab[];

	while(consume('\n')) {};
	i = mt_fptr >> MAC_OFF;j = mt_fptr & MAC_MASK;
	while(bs != end_char && bs != 0)
		{
		if(!j && !mactab[i])
			mactab[i] = alloc((MAC_MASK + 1)*sizeof (struct mtoken));
		mt = &mactab[i][j++];
		mt->mc_val = bs;
		mt->mc_lv = lv;
		if(!(j =& MAC_MASK))
			if(++i >= MAX_MTAB) perror(0,"M %zs",MACRO);
		NBS;
		};
	if(!j && !mactab[i])
		mactab[i] = alloc((MAC_MASK + 1)*sizeof (struct mtoken));
	mt = &mactab[i][j++];
	mt->mc_val = 0;
	failifnot(end_char);
	j =+ (i << MAC_OFF);
	i = mt_fptr; mt_fptr = j;
	return(i);
}

VOID	getmpars(end_char,p)
	BS	end_char;
	COUNT	p[];
{
	FAST	COUNT	i,j,k;
	struct	mtoken	*mt;
	IMPORT	COUNT	mt_pptr,mt_fptr;
	IMPORT	struct	mtoken	*mactab[];

	p[1] = mt_pptr;	/* start of pars*/
	i = mt_pptr >> MAC_OFF; j = mt_pptr & MAC_MASK;
	for(k=1;bs && bs != end_char;NBS)
		{
		if(bs == '\n') continue;
		if(!j && !mactab[i])
			mactab[i] = alloc((MAC_MASK + 1)*sizeof (struct mtoken));
		mt = &mactab[i][j++];
		if(bs == ',' && k++ < 9)
			{
			mt->mc_val = 0;
			p[k] = (i << MAC_OFF) + j;
			}
		else
			{
			mt->mc_val = bs;
			mt->mc_lv = lv;
			};
		if(!(j =& MAC_MASK))
			if(++i >= MAX_MTAB) perror(0,"M %zs",MACRO);
		};
	p[0] = ((i << MAC_OFF)+j) == mt_pptr ? 0 : k;
	for(++k;k<10;k++)
		p[k] = (i << MAC_OFF) + j;
	if(!j && !mactab[i])
		mactab[i] = alloc((MAC_MASK + 1)*sizeof (struct mtoken));
	mt = &mactab[i][j++];
	mt->mc_val = 0;
	mt_pptr = (i << MAC_OFF) + j;
}

VOID	set_l1flag(new_lmode)
	BITS	new_lmode;
{
	IMPORT	BITS	list_mode,list_flags;
	IMPORT	BOOL	l1_flag,l_flag;

	list_mode = new_lmode;
	l1_flag = l_flag && (list_flags >= list_mode);
}

BOOL	q_undefined(i)
	FAST	COUNT	i;
{
	if( (F_TYPE(i) & 077) == UNK) return(YES);
	perror(1,"attempt to reD %n",i);
	return(NO);
}
VOID	skip_ifs()
{
	IMPORT	BS	bs,NBS;
	if(NBS == IFSYM)
		{
		do {skip_ifs();} while (bs && (bs != ENDIF));
		if (bs) NBS;
		}
}

BOOL	include_file(file,e_flag)
	TEXT *file;
	BOOL	e_flag;
{
	FAST	 FILE	ch;
	IMPORT	BOOL	eol_flag;
	IMPORT	TEXT	*file_names[];
	IMPORT	FILE	cin;
	IMPORT	COUNT	lineno;
	IMPORT	COUNT	f_name;

	if((ch = open(file,READ,0)) > 0)
		{
		get_rdbuf();
		file_names[f_name] = file;
		if(f_name > 15)
			perror(1,"too many files used non fatal E",NULL);
		lineno = SF_NO(f_name++,0);
		eol_flag = YES;
		cin = ch;
		return(YES);
		};
	if(e_flag) perror(1,"A Q %s",file);
	return(NO);
}

/* data objects used only during pass 1	*/
struct	m_mpar_info	*pstarts	0;
struct	m_mpar_info	mp_starts[MAX_MLEVEL]	0;
BOOL			c_if[MAX_IF_LEVEL]	0;


