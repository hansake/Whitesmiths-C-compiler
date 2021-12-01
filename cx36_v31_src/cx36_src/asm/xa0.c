/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include	<std.h>
#include	"xa.h"

LOCAL	TEXT	SCCDID[]	"@(#)XA8	V1.5b	release	1:May:84";

/*
 * 1.0: 28:May:81	- improved sdis,macros etc.
 * 1.1:  2:Jul:81	- zpage machines
 * 1.2	16:Jul:81	- bugs;6809;.lineno,new magic #s,.float
 * 1.3	22:Jan:82	- bugs;fp nums
 * 1.4	23:Mar:82	- bugs; new fs alloc
 * 1.5	1:Jul:82	- new R format,extra features
 */

/* initialse globals etc	*/

IMPORT	BOOL	ef_flag,l_flag,ms_flag,op_flag,ptk_flag,rl_flag,sym_flag;
IMPORT	BOOL	hd_flag,ms_flag,sc_flag;
IMPORT	BOOL	ep_flag,gu_flag,px_flag,nm_flag;

/* misuse some bools by having -1,0,+1 or some other small values	*/
BITS	list_flags	LIST_on;
BITS	list_mode	LIST_on;
BITS	nlist_mode	={0};	/* OR'd with list_mode at end of line */
BITS	optn_flags	={0};
BOOL	ascii_flag	7;	/* how to handle parity bit	*/
BOOL	copying		={NO};	/* sending token to tmp channel ?	*/
BOOL	end_flag	={NO};
BOOL	hilo_flag	={YES};	/* byte sex	*/
BOOL	l1_flag		={YES};	/* save source (for listing) pass 1 ? */
BOOL	l2_flag		={NO};	/* list on pass 2 ?	*/
BOOL	pass		={0};	/* passes count 0,1	*/
BOOL	pl_flag		={NO};
BOOL	ps_flag		={NO};	/* enter presets	*/
BOOL	ptk_flag	={0};	/* debugging flag prints tokens	*/
BOOL	start_flag	={NO};
BOOL	zpflag		={NO};
BS	(*yylex)()	={0};	/* set for nbs according to pass	*/
BS	bs		'\r';	/* force line no. for pass 2	*/
COUNT	e_count		={0};	/* total error count	*/
COUNT	f_name		={0};	/* index into fnames	*/
COUNT	int_bnd		={0};	/* boundary conditions for ints	*/
COUNT	int_size	2;	/* # bytes/integer				*/
COUNT	lex_blk		={0};	/* lexical block level (numeric lablels) */
COUNT	lineno		={0};
COUNT	mt_fptr		={0};
COUNT	mt_pptr		={0};
COUNT	n_lex_blk	={0};
COUNT	optb_size	={0};	/* will set to length of current opcode table */
COUNT	sdi_count	={0};	/* sdi number in current psect */
COUNT	str_idx		={0};	/* free index into string tab	*/
COUNT	u_type		={0};	/* micro type (index)	*/
FILE	cin		={0};	/* standard i/p chn	*/
FILE	cout		1;	/* standard o/p chn	*/
FILE	err_ch		={STDERR};	/* error channel	*/
FILE	l_ch		={0};	/* listing channel	*/
FILE	op_ch		={0};	/* output channel	*/
FILE	tmp_ch		={0};	/* tmp channel for 2nd pass	*/
LVAL	inps[MAX_PSECT]	={0,0,0,0};	/* where psects start */
LVAL	lv		={0};	/* lexical value	*/
LVAL	pc		={0};
LVAL	st_address	={0};
LVAL	ts_length	={0};	/* size of text segment	*/
TEXT	*file_names[32]	={0};	/* refs to i/p file names	*/
TEXT	*op_file	={0};
TEXT	*title_s	={""};	/* page header program title	*/
TEXT	tmp_file[MAX_FILENAME]	= {0};		/* current name */
TEXT	tmp_prefix[MAX_FILENAME] = {0};	/*  tmp files prefix	*/
TINY	c_psects	={0};	/* current psect no.	*/
TINY	f_psects	1;	/* 1st free psect no.	*/
TINY	ic_cnt	={0};
TINY	iflevel		={0};	/* if nesting depth	*/
TINY	m_level		={0};	/* macro depth	*/
VOID	(*assemble)()	={0};
/* ref to table of op_codes
 * length of which is calculated by calc_opt_lngth
 */
struct	opitem	(*optable)[]	{0};

#define	MAX_PRESET	10

struct	presets	 g_presets[MAX_PRESET+1] {
	"_text",	PSCT|EXT_T,	1,
	"_data",	PSCT|EXT_T,	2,
	0 };
COUNT	pset_index	2;		/* 1st free	*/

struct	nlist_item	nlist[NLS_SIZE]	={0};
struct	psect_item	psects[MAX_PSECT] 0;
TEXT	**strings[MAX_STAB]	={0};
struct	mtoken	*mactab[MAX_MTAB]	={0};
TEXT	*linebuffer	={0};	/* start of current line in rb_buffer	*/
COUNT	linelength	={0};	/* length of current line	*/

/* use a look-up table to associate each command character
 * with its flag
 * -a  -> flag being cleared
 * +a  -> flag = -1
 * +A  -> flag = +1
 *
 * iv_flag set for invalid commands
 */
BOOL	iv_flag		={NO};
BOOL	uc_flag		={NO};	/* allows ul == L etc.	*/
BOOL	*flag_refs[26]{
	&iv_flag,		/* A - start absolute psect	*/
	&op_flag,		/* B - turn off o/p	*/
	&sc_flag,		/* C - force sep assembly if -c	*/
	&ps_flag,		/* D - define symbol	*/
	&ep_flag,		/* E - print errors other than error chl */
	&iv_flag,		/* F - fp format (0 = W,1 = AMI9511,2=9512)	*/
	&gu_flag,		/* G - make any unknowns global	*/
	&hd_flag,		/* H - include header file	*/
	&iv_flag,		/* I - include prefixes	*/
	&ef_flag,		/* J - force pass 2 even if errors in pass 1 */
	&iv_flag,		/* K - significant # chars	*/
	&l_flag,		/* L - listing	*/
	&iv_flag,		/* M - error	*/
	&nm_flag,		/* N - add names to o/p	*/
	&iv_flag,		/* O - -o used to select o/p file	*/
	&px_flag,		/* P - processor in header	*/
	&ms_flag,		/* Q - issue wellcome message etc	*/
	&rl_flag,		/* R - relocatable obj required	*/
	&sym_flag,		/* S - symbol table	*/
	&iv_flag,		/* T - usr tmp prefix	*/
	&uc_flag,		/* U - treat rest of command as upper case */
	&iv_flag,		/* V - error	*/
	&iv_flag,		/* W - error	*/
	&iv_flag,		/* X - error	*/
	&iv_flag,		/* Y - error	*/
	&ptk_flag};		/* Z - debugging	*/

BOOL	main(ac,av)
	COUNT	ac;
	TEXT	*av[];
{
	FAST	COUNT	ap,i;
	FAST	TEXT	*a;
	IMPORT	BOOL	*flag_refs[];
	IMPORT	BOOL	hd_flag,ms_flag,sc_flag;
	IMPORT	BOOL	fp_flag;
	IMPORT	COUNT	e_count;
	IMPORT	COUNT	f_name;
	IMPORT	COUNT	lineno;
	IMPORT	COUNT	siglength;
	IMPORT	FILE	cin,cout;
	IMPORT	LVAL	inps[];
	IMPORT	TEXT	*hd_file,*px_type,*file_names[];
	IMPORT	TEXT	VERSION[],tmp_prefix[];
	IMPORT	TINY	ctable[];
	IMPORT	TINY	ucbit;
	IMPORT	VOID	cleanup();
	LOCAL	BOOL	in1_flag;	/* have we initialised ? */
	LOCAL	BOOL	in0_flag	={NO};

	in1_flag = NO;
	onintr(&cleanup);
/* announce version no. */
	cout = STDOUT;
/* if program name is upper case then allow reswords etc to be uc */
	ucbit =| (**av & 040) ^ 040;
	for(ap=1;ap<ac;ap++)
		{
		a = av[ap];
		switch (*a)
			{
	case '-':
			if(! *++a)		/* '-' == stnd i/p */
				{
				cin = 0;
				goto read_ip_file;
				};
			for(;*a;a++)
				{
			/* look for specials	*/
				if(*a == 'o')
					op_file = av[++ap];
				if(*a == 'a')
					{
					inps[ABS_PS] = get_val(++a);
					break;
					};
				if(*a == 't')
					{
					inps[TEXT_PS] = get_val(++a);
					break;
					};
				if(*a == 'd')
					{
					inps[DATA_PS] = get_val(++a);
					break;
					};
				if(*a == 'z')
					{
					inps[ZP_PS] = get_val(++a);
					break;
					};
				if(ctable[*a] & LC)
					*flag_refs[*a - 'a'] = 0;
				else if(ctable[*a] & UC)
					*flag_refs[*a - 'A'] = 0;
				};
			break;
	case '+':
			uc_flag = NO;
			for(;*++a;)
				{
				if(uc_flag) *a = toupper(*a);
			/* look for specials	*/
				if(*a == 'f')
					fp_flag = get_val(++a);
				if(*a == 'h')
					hd_file = av[++ap];
				if(*a == 'p')
					px_type = av[++ap];
				if(*a == 't')
					cpystr(tmp_prefix,av[++ap],NULL);
				if(*a == 'd')
					if(pset_index < MAX_PRESET)
						{
						g_presets[pset_index].pset_name =
								av[++ap];
						g_presets[pset_index].pset_type =
								CONST;
						g_presets[pset_index++].pset_value =
							get_val(av[++ap]);
						}
					else
						perror(0,"M presets");
				if(*a == 'k')
					siglength = get_val(++a);
				if(ctable[*a] & LC)
					*flag_refs[*a - 'a'] = -1;
				else if(ctable[*a] & UC)
					*flag_refs[*a - 'A'] = 1;
				};
			break;
	default:			/* try for i/p file name */
			if((cin = open(av[ap],READ,0)) > 0)
				{
		read_ip_file:
				if(!in0_flag++)
					if(ms_flag) printf(VERSION);
				file_names[f_name] = av[ap];
				if(f_name > 15)
					perror(1,"non fatal E too many files used",NULL);
				lineno = SF_NO(f_name++,0);
				if(!in1_flag++)
					init_1();	/* new start	*/
				else
					set_token(EIF,f_name);
/* do 2nd pass if seen .end or if -c option in force */
				if((i = parse1()) || !sc_flag)
					{
					if(!i) perror(1,"%z X",ENDSYM);
					pass_2();
					in1_flag = NO;
					f_name = 0;
					};
				if(cin) close(cin);	/* don't close S i/p */
				}
			else
				perror(0,"A Q %s",av[ap]);
			break;
			};
		}
/* catch any non 'END'ed files	*/
	if(!in0_flag) perror(0,"no i/p file");
	if(!in1_flag) exit(e_count? XRPTNO : XRPTYES);
	perror(1,"%z X",ENDSYM);
	exit(pass_2());
}

FILE	make_new_file(prefix,suffix,name,rsize)
	TEXT *prefix,*suffix;
	FAST	TEXT	*name;
	COUNT	rsize;
{
	FAST	COUNT	i;
	LOCAL	FILE	ch;

	if(*prefix == '-')	prefix = "xa";	/* trap bad name */
	cpystr(name,prefix,NULL);
	i = lenstr(name);
	if(cmpstr(&name[i-XLSRC],XSRC)) i =- XLSRC;
	cpystr(&name[i],".",suffix,NULL);
	if((ch = create(name,WRITE,rsize))< 0)
		perror(0,"A K %s",name);
	return(ch);
}

VOID	set_start_values()
{
	FAST	COUNT	i;
	IMPORT	BITS	list_mode,list_flags;
	IMPORT	BITS	optn_flags;
	IMPORT	BOOL	ascii_flag;
	IMPORT	BOOL	l1_flag,l2_flag,l_flag,rl_flag;
	IMPORT	BOOL	pass;
	IMPORT	COUNT	lex_blk,n_lex_blk;
	IMPORT	FILE	op_ch;
	IMPORT	LVAL	inps[];
	IMPORT	LVAL	pc;
	IMPORT	struct	psect_item	psects[];
	IMPORT	TINY	c_psects,f_psects;

	pc = 0;
	for(i=0;i<MAX_PSECT;i++)
		psects[i].ps_high = inps[i];
	lex_blk = 0;	n_lex_blk = 1;
/* set to _text or abs	*/
	c_psects = rl_flag ? TEXT_PS : ABS_PS;
/* preset listing options	*/
	l1_flag = l_flag;		/* start off listing eveything	*/
	l2_flag = pass && l_flag;	/* no listing if no file	*/
	list_mode = LIST_on;		/* just normal text	*/
	list_flags = LIST_on;
/* reset all user modifiable flags	*/
	ascii_flag = 7;
	optn_flags = 0;
}


VOID	set_up_proc(s)
	TEXT	*s;
{
	IMPORT	struct	tp_item	typepars[];
	IMPORT	COUNT	u_type;

	for(u_type=0;typepars[u_type].tp_uid;u_type++)
		if(equal_nc(s,typepars[u_type].tp_uid))
			{
			(*typepars[u_type].tp_init)();
			return;
			};
	perror(0,"U P T %s",s);
}

VOID	calc_opt_length()
{
	IMPORT	COUNT	optb_size;
	IMPORT	struct	opitem	(*optable)[];
	FAST	COUNT	i;

	for(i = 0;(*optable)[i].opcode;i++) {};
	optb_size = i;
}

/* include nulls for data */
struct opitem op_null = { 0};

VOID	as_null()		/* dummy	*/
	{}

VOID	i_null()
{
	IMPORT	COUNT	optb_size;
	IMPORT	struct	opitem	(*optable)[];
	IMPORT	VOID	(*assemble)(),as_null();
	IMPORT	struct	opitem	op_null;
	IMPORT	BOOL	hilo_flag;
	optable = &op_null;
	assemble = &as_null;
	hilo_flag = YES;	/* expected order */
	optb_size = -1;	/* no table thus prevent any lookups */
}


VOID	init_1()		/* init for 1st pass	*/
{
	FAST	COUNT	i,j;
	IMPORT	BOOL	hd_flag;
	IMPORT	BOOL	pass;
	IMPORT	BS	(*yylex)(),nbs(),nbs_1d();
	IMPORT	COUNT	e_count;
	IMPORT	COUNT	mt_pptr,mt_fptr;
	IMPORT	COUNT	sdi_count;
	IMPORT	COUNT	str_idx;
	IMPORT	FILE	cout,tmp_ch;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	struct	presets	 g_presets[];
	IMPORT	struct	psect_item	psects[];
	IMPORT	TEXT	*hd_file,*px_type;
	IMPORT	TINY	m_level;

	err_ch = STDERR;		/* default to error o/p	*/
	cout = STDOUT;			/* only use is for debugging	*/
	e_count = 0;
	pass = 0;
/* setup tmp prefix	*/
	if(!tmp_prefix[0])
		{
		cpystr(tmp_prefix,uname(),NULL);
		for(i=lenstr(tmp_prefix);i;i--)
			if(tmp_prefix[i] == '.')
				{
				tmp_prefix[i] = '\0';
				break;
				};
		};
/* 'free' all free space	*/
	r_space(0);			/* -> start	*/
/* clear name table	*/
	for(i=0;i<NLS_SIZE;i++)
		{
		nlist[i].n_ptr = 0;
		nlist[i].n_type = 0;
		nlist[i].n_ps = 0;
		set_sdinum(i, 0);
		};
/* clear sdi count etc	*/
	sdi_count = 0;
	str_idx = 0;
/* init read buffer + ic_ stack	*/
	init_rdbuf();
/* enter presets	*/
	put_in_nametable(&g_presets);
/* init macro space	*/
	mt_pptr = 0;
	mt_fptr = MP_SIZE;
	m_level = 0;
/* init tmp file */
	tmp_ch = 0;
/* init tmp buffer pointer */
	init_tbuf();
/* init psects		*/
	for(i=0;i<MAX_PSECT;i++)
		{
		psects[i].ps_tfile = 0;
		psects[i].ps_cfile = 0;
		psects[i].ps_high = 0;
		psects[i].ps_osdi = 0;
		};
	f_psects = 3;	/* !! should be 3 or 4 depend on zpage etc */
	set_amode(rl_flag);
/* common presets to pass 1 & pass 2	*/
	set_start_values();
/* set u_type info	*/
	set_up_proc(px_type);		/* might be changed by .proc	*/
/* select nbs routine	*/
	yylex = ptk_flag < 0 ? &nbs_1d : &nbs;
/* include any header file	*/
	if(hd_flag < 0)
		include_file(hd_file,YES);
}

VOID	set_amode(m)			/* set addrs mode */
	FAST	COUNT	m;
{
	IMPORT	BOOL	rl_flag;
	IMPORT	COUNT	siglength;

	rl_flag = m;
	if(rl_flag == -1)		/* W old format	*/
		siglength = min(7,siglength);
	else if(rl_flag == 1)		/* W new format	*/
		{
		siglength = min(15,siglength);
		set_nw_mode();
		}
	else				/* INTEL	*/
		siglength = min(32,siglength);
}

LOCAL	struct	presets	zp_preset[]	{
	"_zpage",	PSCT|EXT_T,	3,
	0 };

BOOL	set_zpage_mode()		/* set up for a zero_page */
{
	IMPORT	BOOL	rl_flag,zpflag;
	IMPORT	TINY	f_psects;
	IMPORT	struct	presets	zp_preset[];

	if(rl_flag == 1)
		return(NO);				/* not available in W2.1 format */
	put_in_nametable(&zp_preset);
	f_psects = 4;
	return(zpflag = YES);
}

LOCAL	struct	presets	nw_preset[]	{	/* new W format	*/
	"_bss",		PSCT|EXT_T,	3,
	0};

VOID	set_nw_mode()			/* set up new W format	*/
{
	IMPORT	TINY	f_psects;
	IMPORT	struct	presets	nw_preset[];
	put_in_nametable(&nw_preset);
	f_psects = 4;
}

COUNT	pass_2()
{
	IMPORT	BOOL	ef_flag,l_flag,ms_flag,op_flag,ptk_flag,rl_flag,sym_flag;
	IMPORT	BOOL	pass;
	IMPORT	BS	(*yylex)(),nbs_2(),nbs_2d();
	IMPORT	COUNT	cl_b_ptr;
	IMPORT	COUNT	e_count;
	IMPORT	COUNT	lineno;
	IMPORT	LVAL	ts_length;
	IMPORT	FILE	cout,l_ch,op_ch,tmp_ch;
	IMPORT	struct	psect_item	psects[];
	IMPORT	struct	mtoken	*mactab[];
	LOCAL	FILE	sym_ch;
	LOCAL	TEXT	newname[MAX_FILENAME];
	FAST	COUNT	i;
/* set up for pass 2	*/
	flush_tbuf();
	chk_for_unknowns();
/* clear space used for macros - possibly need in pass 2	*/
	for(i = 0;i<MAX_MTAB;i++)
		{
		free(mactab[i]);
		mactab[i] = 0;
		};
	do_sdi_list();
	ts_length = psects[TEXT_PS].ps_high;	/* size of text segment */
	if(sym_flag)
		{
		sym_ch = sym_flag > 0 ? STDOUT		/* stnd o/p	*/
				:make_new_file(file_names[0], XSYM, newname,0);
		pr_nm_format(sym_ch,"");
		if(sym_flag < 0) close(sym_ch);
		};
	if(e_count && ef_flag)	goto no_pass2;
/* open various output files	*/
	l_ch = op_ch = STDOUT;		/* default to standard o/p	*/
	if(l_flag < 0)			/* listing to file */
		l_ch = make_new_file(file_names[0],
					XLST,
					newname,
					0);
	if(op_flag < 0)
		op_ch = op_file ? create(op_file,WRITE, rl_flag ? 1 : 0)
						: make_new_file(file_names[0],
					rl_flag > 0 ? XR21 : (rl_flag < 0 ? XR20 : XOUT),
					newname,
					rl_flag? 1 : 0);
/* do pass 2	*/
	emit_header();			/* must do before reset psect info */
	pass++;
	set_start_values();
	pl_flag = YES;			/* prevent listing for dummy line */
	CLEAR_CL;			/* clear listing buffer	*/
	lineno = 0;
	yylex = ptk_flag > 0 ?&nbs_2d:&nbs_2;
	c_psects = rl_flag ? TEXT_PS : ABS_PS;
	psects[c_psects].ps_tfile = op_ch;
	change_psect(c_psects);
	parse2();
/* tidy up at end of pass 2	*/
	endcode();		/* flush buffer	*/
	if(op_flag < 0)
		{
		close(op_ch);
		op_file = 0;
		};
	if(l_flag < 0)
		close(l_ch);
no_pass2:
	if(tmp_ch)
		close(tmp_ch);
	cout = STDOUT;
	if(ms_flag)
		printf("%s:\t%s error%sdetected\n",
				file_names[0],
				c_to_string(e_count,newname),
				e_count==1?" ":"s ");
	rm_tmp_files();
	if(err_ch > STDERR) close(err_ch);
	return(e_count? XRPTNO : XRPTYES);
}


VOID	cleanup() 	/* remove tmp file if interrupted during 1st pass */
{
	rm_tmp_files();
	exit(XRPTNO);
}

VOID	rm_tmp_files()
{
	FAST	COUNT	i;

	IMPORT	FILE	op_ch,tmp_ch;
	IMPORT	struct	psect_item	psects[];
	if(tmp_ch)
		{
		remove(tmp_file);		/* link pass 1 to pass 2 */
		tmp_ch = 0;			/* mark as unused	*/
		};
	for(i=0;i<MAX_PSECT;i++)		/* psect tmps	*/
		{
		if(psects[i].ps_tfile &&
			psects[i].ps_tfile != op_ch)
			{
			remove(psects[i].ps_tfname);
			psects[i].ps_tfile = 0;
			};
		if(psects[i].ps_cfile)
			{
			remove(psects[i].ps_cfname);
			psects[i].ps_cfile = 0;
			};
		};
}

/* temp file routines
 * attempt to hold tokens within buffer until
 * forced to write out
 */
#ifndef	TBUFFER
#define	TBUFFER	512		/* multiples of 512 best	*/
#endif
LOCAL	TINY t_buffer[TBUFFER] {0};
LOCAL	TINY	*t_b_ptr	={&t_buffer[0]};

VOID	put_tbuf(from,n)
	TINY *from;
	COUNT n;
{
	FAST	COUNT nn;
	FAST	TINY	*f;
	IMPORT	FILE	tmp_ch;
	IMPORT	TINY	t_buffer[];
	IMPORT	TINY	*t_b_ptr;
	IMPORT	TEXT	tmp_prefix[];

	for(f=from,nn=n;nn;nn--)
		{
		*t_b_ptr++ = *f++;
		if(t_b_ptr >= &t_buffer[ TBUFFER])
			{
			if(!tmp_ch)
				tmp_ch = make_new_file(
						tmp_prefix,XTMP,tmp_file,1);
			if(write(tmp_ch,t_buffer,TBUFFER) != TBUFFER)
				perror(0,"A write tmp file");
			t_b_ptr = t_buffer;
			};
		};
}
VOID	get_tbuf(to,n)
	TINY *to;
	COUNT	 n;
{
	FAST	COUNT nn;
	FAST	TINY	*t;
	IMPORT	FILE	tmp_ch;
	IMPORT	TINY	t_buffer[];
	IMPORT	TINY	*t_b_ptr;

	for(t=to,nn=n;nn;nn--)
		{
		if(t_b_ptr >= &t_buffer[ TBUFFER])
			{
			if(!tmp_ch) perror(0,"Z E M reads");
			if(fread(tmp_ch,t_buffer,TBUFFER) != TBUFFER) perror(0,"A read tmp file");
			t_b_ptr = t_buffer;
			};
		*t++ = *t_b_ptr++;
		};
}
VOID	flush_tbuf()
{
	IMPORT	FILE	tmp_ch;
	IMPORT	TINY	t_buffer[];
	IMPORT	TINY	*t_b_ptr;
	put_tbuf("\0\0\0",3);	/* make sure file is terminated !! */
	if(tmp_ch)
		{
		if(write(tmp_ch,t_buffer,TBUFFER) != TBUFFER)
			perror(0,"A write tmp file");
		close(tmp_ch);
		if((tmp_ch = open(tmp_file,READ,1)) < 0)
			perror(0,"A Q %s for 2nd pass",tmp_file);
		if(fread(tmp_ch,t_buffer,TBUFFER) != TBUFFER) perror(0,"A read tmp file");
		};
	t_b_ptr = t_buffer;
}

VOID	init_tbuf()
{
	IMPORT	TINY	*t_b_ptr,t_buffer[];

	t_b_ptr = t_buffer;
	tmp_ch = 0;
}
/* free space management
 * vrsn 2 - gb_small used for small requests
 *			taken from larger blocks as required
 *			gb_large provides large block
 *			r_space restores space used after request to heap
 *
 */

#define	SPACEQUANTUM	1024
TINY	*fslist	{0};
LOCAL	TINY	*spc_ptr	={0};	/* 1st free location	*/
LOCAL	COUNT	spc_bytes	={0};	/* no. left		*/

TINY	*gb_large(size)
	FAST	COUNT	size;
{
	IMPORT	TINY	*fslist;
	
	fslist = nalloc(size + sizeof(TINY *)+4,fslist);
	if(!fslist) perror(0,"Y E no free space");
	return(fslist + sizeof(TINY *));
}

VOID	z_space()
{
	spc_bytes = 0;
}
TINY	*gb_small(size)
	FAST	COUNT	size;
{
	if(size > spc_bytes)
		spc_ptr = gb_large(spc_bytes = SPACEQUANTUM);
	spc_bytes =- size;
	return(spc_ptr + spc_bytes);
}

/* q_space returns a pointer to current freespace list
 * which can be used as a restoration point
 */
TINY	*q_space()
{
	return(fslist + sizeof(TINY *));
}

/* free all space on freespace upto returned value
 * 0 == end of list (ie. all)
 */
VOID	r_space(s)
	TINY	*s;
{
	IMPORT	TINY	*fslist;

	fslist = frelst(fslist,s ? (s - sizeof(TINY *)) : 0);
}

