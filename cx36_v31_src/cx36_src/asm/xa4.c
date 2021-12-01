/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - PASS 2 DRIVER
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include	<std.h>
#include	"xa.h"

/* table driven parser - picks up handler from p2handlers
 *	reads next bs + calls handler
 * assumes each subsection has read until 1st symbol after
 * its handle
 */
/* x_hhhh is handler used on pass 1
 * y_hhhh is handler used on pass 2
 * xy_hhh is handler used on both passes (would prefer sep but saves space)
 */
struct	p2h_item	{
	VOID	(*p2h_code)();
	};
IMPORT	VOID
	xy_addr(), x_ascii(), xy_byte(), 
	x_end(), x_even(),
	x_list(), x_option(), x_origin(), y_page(),
	x_psect(), xy_text(), x_title(), xy_word(),
	rpt_2err(), xy_long(), y_set(), y_end(), y_psect();

struct	p2h_item	p2handler[]	{
&rpt_2err,	/* AUTOD 0200			*/
&rpt_2err,	/* AUTOI 0201			*/
&xy_addr,	/* ADDR	0202	 addr(ess)	*/
&x_ascii,	/* ASCII	0203	 ascii 7/8 odd/even	*/
&xy_byte,	/* BYTE	0204	 byte	*/
&xy_long,	/* DBLE	0205	 double 	*/
&rpt_2err, 	/* DEF	0206	 def(ine)	*/
&rpt_2err,	/* ELSESYM	0207	 else	*/
&rpt_2err,	/* ENDIF	0210	 endif*/
&rpt_2err,	/* ENDM	0211	 endm(acro)	*/
&rpt_2err,	/* ENDR	0212	 endr(epeat)	*/
&y_end, 	/* ENDSYM	0213	 end	*/
&rpt_2err,	/* EQU	0214	 equ			*/
&rpt_2err,	/* EQUIV	0215	 equivalent (==)	*/
&x_even,	/* EVEN	0216	 even	*/
&rpt_2err,	/* EXITM	0217	 exitm(acro)	*/
&rpt_2err, 	/* EXT	0220	 ext(ernal)	*/
&rpt_2err,	/* GE	0221	 greater than or eq	*/
&rpt_2err,  	/* IFSYM	0222	 if	*/
&rpt_2err,	/* INCLD	0223	 include	*/
&rpt_2err,	/* LBYTE	0224	 l(ower)byte	*/
&rpt_2err,	/* LE	0225	 less than or equal	*/
&rpt_2err,	/* LINUM	0226	 lineno	*/
&x_list,	/* LIST	0227	 list	*/
&rpt_2err,	/* LOGAND	0230	 logical and && */
&rpt_2err,	/* LOGOR	0231	 logical or ||	*/
&rpt_2err,	/* MACRO	0232	 macro	*/
&rpt_2err,	/* NE	0233	 not equal	*/
&rpt_2err,	/* ODD	0234	 odd		*/
&x_option,	/* OPTN	0235	 opt(io)n	*/
&x_origin,	/* ORIGIN	0236	 or(i)g(en)	*/
&y_page,	/* PAGE	0237	 page	*/
&rpt_2err,	/* PROCESSOR	0240	 processor	*/
&y_psect,	/* PSECT	0241	 p(rogram) sect(ion)	*/
&rpt_2err,	/* PUBLIC	0242	 public (definition)	*/
&rpt_2err,	/* REPEAT	0243	 repeat	*/
&y_set,  	/* SET	0244	 set	*/
&rpt_2err,	/* SHL	0245	SHift Left	*/
&rpt_2err,	/* SHR	0246	SHift Right	*/
&xy_text,	/* TEXTSYM	0247	 text	*/
&rpt_2err,	/* UBYTE	0250	 u(pper) byte	*/
&xy_word,	/* WORD	0251	 word */
&xy_long,	/* LFLOAT	0252	 l(ong) float	*/
&xy_long,	/* FLOATS	0253	 4 bytes	*/
&x_title,	/* TITLE	0254	 title	*/
0};

BOOL	parse2()			/* 2nd pass	*/
{
	FAST	LVAL	i;
	FAST	COUNT	j,k;
	IMPORT	BITS	list_mode,nlist_mode;
	IMPORT	BOOL	end_flag;
	IMPORT	BS	bs,NBS;
	IMPORT	COUNT	cl_b_ptr;
	IMPORT	COUNT	lex_blk,n_lex_blk;
	IMPORT	COUNT	sdi_count;
	IMPORT	FILE	cin;
	IMPORT	LVAL	lv;
	IMPORT	LVAL	pc;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	struct	p2h_item	p2handler[];
	IMPORT	struct	psect_item	psects[];
	IMPORT	TEXT	*linebuffer;	/* needed to hold listing */
	IMPORT	struct	ic_item ic_stk[];
	IMPORT	TINY	c_psects;
	IMPORT	VOID	(*assemble)();
	IMPORT	BOOL	pl_flag;

	linebuffer = ic_stk[0].icrb;
	NBS;				/* read 1st to kick off	*/
	end_flag = NO;
	FOREVER
		{
/* printf("dealing with %z\n",bs); */
		if(bs < 040)
			{
			switch(bs)
				{
case 0:						/* EOF	*/
				psects[c_psects].ps_high = pc;
				return(end_flag);
case BOB:				/* start of lex block (macro) */
				lex_blk = lv;
				n_lex_blk = lex_blk + 1;	/* kludge - assume inc on pass 1 */
				nlist_mode =| LIST_macro;
				break;
case EOB:				/* end of lex block	*/
				lex_blk = lv;
				listline();
				CLEAR_CL;
				list_mode =& ~LIST_macro;
				break;
case BIF:				/* start of include file */
				list_mode =| LIST_include;
				break;
case EIF:				/* end of include file	*/
				listline();
				CLEAR_CL;
				list_mode =& ~LIST_include;
				break;
case NLABEL:			/* numeric labels	*/
				if(F_TYPE(k = find_numlabel(lex_blk,lv)) != LABEL ||
					F_VAL(k) != pc)
					perror(2,"phase E <> %d$",lv);
				NBS;
				break;
case ID:					/* label or exrpsn ? */
				k = lv;
				NBS;		/* : or =	*/
				if(consume(EQU))
					{
					if(F_TYPE(k) == VAR) S_VAL(k,const_expression());
					continue;
					};
				if((F_TYPE(k = lv) & 07) == LABEL)
					{
					if(F_VAL(k) != pc) perror(2,"phase E %n",k);
					lex_blk = n_lex_blk++;
					};
				break;
case '\n':					/* newline */
				break;
case OP:
				(*assemble)((COUNT)lv);
				listline();
				continue;
default:
				rpt_2err(bs);
				};
			NBS;
			}
		else if( bs & 0200)
			{
			k = bs & 077;
			NBS;
			(*p2handler[k].p2h_code)(k | 0200);
			CLEAR_CL;
			}
		else
			{
			rpt_2err(bs);
			NBS;
			}
	};
}
VOID	y_end()		/* end	*/
{
	IMPORT	LVAL	pc,st_address;
	IMPORT	BOOL	end_flag,start_flag;
	IMPORT	struct	psect_item	psects[];
	LOCAL	struct	result	rs;

	psects[c_psects].ps_high = pc;
	if(start_flag = (bs && bs  != '\n'))
		st_address = expression(&rs);
	listline();
	while(bs) NBS;
	end_flag++;
}

VOID	y_psect()		/* p(rogram) sect(ion)	*/
{
	FAST	COUNT	k;

	if(bs != ID || (F_TYPE(k = lv) & 07) != PSCT)
		return;
	psects[c_psects].ps_high = pc;
	change_psect(F_VAL(k));
	c_psects = F_VAL(k);
	pc = psects[c_psects].ps_high;
	NBS;
}

VOID	y_set()		/* set	*/
{
	FAST	COUNT	k;
	do {
		while(consume('\n'))  {};
		if(bs == ID) 
			{
			k = lv;
			NBS;
			failifnot('=');
			if(F_TYPE(k) == VAR)
				S_VAL(k,const_expression());
			};
		} while(consume(','));
}

VOID	rpt_2err(sym)
	BS	sym;
{
	perror(2,"Y E <> %z",sym);
}
VOID	y_page()		/* page	*/
{
	IMPORT	TEXT	*title_s;
	IMPORT	TEXT	**strings[];
	listline();
	throwpage(bs == STRNG ? strings[lv >> STR_OFF][lv & STR_MASK] : title_s);
	consume(STRNG);
}


