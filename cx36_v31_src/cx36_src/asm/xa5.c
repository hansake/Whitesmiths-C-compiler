/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - OUTPUT ROUTINES
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

/* Output routines for XA8:
 * Listing
 * Object
 * Symbol Tables
 *
 * Optimised for speed rather than space - thus will prefer to
 *	translate hex etc. rather than call on printf with its
 *	multiple writes
 */

#include	<std.h>
#include	"xa.h"

/* LISTING	*/

/* line format is	lno <wspace> pc <wspace> code <tab> listing */
/* defined in xa.h as CL_***	*/
TEXT	cl_buf[CL_SIZE]	= "                          ";
LVAL	cl_pc = 0;			/* pc at start of code	*/
COUNT	cl_b_ptr = CL_CSTART;
/* enter byte of code into code listing buffer */
VOID	put_cl_buffer(b)
	FAST	TEXT	b;
{
	IMPORT	COUNT	cl_b_ptr;
	IMPORT	LVAL	cl_pc;
	IMPORT	TEXT	cl_buf[];
	IMPORT	TEXT	hex[];
	IMPORT	LVAL	pc;
	if(cl_b_ptr >= CL_END) cl_b_ptr = CL_CSTART;	/* wrap arround */
	if(cl_b_ptr == CL_CSTART) cl_pc = pc;
	cl_buf[cl_b_ptr++] = hex[(b >> 4) & 017];
	cl_buf[cl_b_ptr++] = hex[b & 017];
}

/* complex routine as can be called on to print line under
 * following coditions
 * no code emitted but a '\n' seen
 * code emitted + need print line as read
 * code emitted but line has already been printed
 * no code to print + no linebuffer either
 * flags	l_flag	= listing with l_ch = listing channel
		pl_flag = line buffer has been printed
		list_flags = set of desired modes to list
		list_mode = current mode (eg incld file )
 */

VOID	listline()
{
	IMPORT	BOOL	l2_flag,pl_flag;
	IMPORT	BITS	list_flags,list_mode,nlist_mode;
	IMPORT	FILE	l_ch;
	IMPORT	TEXT	*linebuffer;
	IMPORT	COUNT	linelength;
	IMPORT	COUNT	lineno;
	IMPORT	COUNT	cl_b_ptr;
	IMPORT	LVAL	cl_pc;
	IMPORT	TEXT	cl_buf[];
	IMPORT	TEXT	hex[];
	IMPORT	BOOL	pass;
	FAST	COUNT	i,j;
	FAST	TEXT	*s;

	if(l2_flag && (list_flags >= list_mode) && (list_flags & list_mode))
		{
		if(pl_flag)	/* set = no line_buffer to print	*/
			{
			if(cl_b_ptr != CL_CSTART)  /* any code to do	*/
				{
				for(s = cl_buf,i=CL_CSTART;i;i--)
					*s++ = ' ';
				write(l_ch,cl_buf,cl_b_ptr);
				write(l_ch,"\n",1);
				cl_b_ptr = CL_CSTART;
				};
			/* else print nothing	*/
			}
		else 		/* code + buffer	*/
			{
			for(j = L_NO(lineno),i=CL_LN;i>=0;)
				{
				cl_buf[i--] = '0' + (j % 10);
				if((j =/ 10) == 0) break;
				};
			for(;i>=0;i--) cl_buf[i] = ' ';
			if(cl_b_ptr != CL_CSTART)	/* code ? */
				{
				s = &cl_buf[CL_PC];
#ifdef	A32
				*s++ = hex[(cl_pc) >> 28) & 017];
				*s++ = hex[(cl_pc) >> 24) & 017];
				*s++ = hex[(cl_pc) >> 20) & 017];
				*s++ = hex[(cl_pc) >> 16) & 017];
#endif
#ifdef	A24
				*s++ = hex[(cl_pc) >> 20) & 017];
				*s++ = hex[(cl_pc) >> 16) & 017];
#endif
				*s++ = hex[(cl_pc >> 12) & 017];
				*s++ = hex[(cl_pc >> 8) & 017];
				*s++ = hex[(cl_pc >> 4) & 017];
				*s = hex[cl_pc & 017];
				}
			else
				for(s = &cl_buf[CL_PC],i = CL_CSTART - CL_PC;i;i--)
					*s++ = ' ';
			for(s = &cl_buf[cl_b_ptr],i=CL_END-cl_b_ptr;i;i--) *s++ = ' ';
			*s = '\t';
			cl_b_ptr = CL_CSTART;
			write(l_ch,cl_buf,CL_SIZE);
			write(l_ch,linebuffer,linelength);
			write( l_ch,"\n",1);
			};
		pl_flag = YES;
		}
	else
		{
		CLEAR_CL;
		};
	list_mode =| nlist_mode; nlist_mode = 0;
}

VOID	throwpage(hd)
	TEXT	*hd;
{
	IMPORT	FILE	l_ch;
	IMPORT	BOOL	l2_flag;
	LOCAL	TEXT	ch;
	if(l2_flag && (list_flags >= list_mode) && (list_flags & list_mode))
		{
		ch = FORM_FEED;
		write(l_ch,&ch,1);
		if(hd)
			{
			write(l_ch,hd,lenstr(hd));
			write( l_ch,"\n\n",2);
			};
		};
}

/* SYMBOL TABLE	*/
/* routines to print symbol table
 * adopts style produced by nm but using Hex
 *	ie.	vvvvL name
 *	L is blank == constant,
 *	aA == abs,,tT == text (local/global),
 *		dD == data,U == unknown
 */

LOCAL	TEXT	s_types[]	"ATDZ";		/* nb Z==B	*/
#define	PF	"%s0x%4x%c"			/*16 bit value	*/
#define	PB	"%s      %c"			/*6 blanks	*/
#ifdef	A32
#define	PF	"%s0x%8x%c"			/*32 bits	*/
#define	PB	"%s          %c"
#endif
#ifdef	A24
#define	PF	"%s0x%6x%c"			/*24 bits	*/
#define	PB	"%s        %c"
#endif

VOID	pr_nm_format(where,is)
	FILE where;
	TEXT	*is;
{
	FAST	COUNT	i;
	FAST	TEXT	l;
	FAST	FILE	sav_ch;
	IMPORT	FILE	cout;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	BOOL	rl_flag;
	IMPORT	TEXT	s_types[];

	sav_ch = cout;
	cout = where;
	s_types[3] = rl_flag == 1 ? 'B' : 'Z';
	for(i=0;i<NLS_SIZE;i++)
		{
		if(!nlist[i].n_ptr) continue;	/* empty */
		l = s_types[F_PS(i) & 03];
		switch(F_TYPE(i))
			{
	case UNK|PUB_T:
	case UNK:
			printf(PB,is,'U');
			break;
	case CONST|PUB_T:
	case CONST:
			printf(PF,is,F_VAL(i),' ');
			break;
	case LABEL|EXT_T:
			printf(PB,is,l);
			break;
	case LABEL|PUB_T:
			printf(PF,is,F_VAL(i),l);
			break;
	case LABEL:
			printf(PF,is,F_VAL(i),l|040);
			break;
	default:
			continue;
			};
		printf(" %n\n",i);
		};
	cout = sav_ch;
}

LVAL	gt_adj_value(i)
	FAST	COUNT	i;
{
	IMPORT	LVAL	ts_length;

/* _data offset by length of text	*/
	return(F_VAL(i) +  (F_PS(i) == 2 ? ts_length : 0));
}

/* nb link with 'gt_no_symbols' which must return count
 * of number of symbols printed by pr_rel_format
 */
VOID	pr_rel_format(where)
	FILE where;
{
	FAST	COUNT	i,k;
	IMPORT	BOOL	nm_flag;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	struct	psect_item	psects[];
	IMPORT	TINY	f_psects;

/* must output external labels 1st + issue numbers from 2(f_psects)
 * (ie. not those declared in this program)
 */
	for(k = f_psects,i=0;i<NLS_SIZE;i++)
		if(nlist[i].n_ptr && F_TYPE(i) == (LABEL|EXT_T))
			{
			S_VAL(i,k++);
			pr_r_item(where,i,010,0);
			};
	for(i=0;i<NLS_SIZE;i++)
		{
		if(!nlist[i].n_ptr) continue;
		switch(F_TYPE(i))
			{
	case LABEL:
			if(nm_flag) pr_r_item(where,i,004 | F_PS(i),gt_adj_value(i));
			break;
	case LABEL|PUB_T:
			pr_r_item(where,i,014 | F_PS(i),gt_adj_value(i));
			break;
	case CONST:
			if(nm_flag) pr_r_item(where,i,004,F_VAL(i));
			break;
	case CONST|PUB_T:
			pr_r_item(where,i,014,F_VAL(i));
			break;
	default:
			continue;
			};
		};
}

VOID	set_ext_labels()
{
	FAST	COUNT	i,k;
	IMPORT	BOOL	nm_flag;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	struct	psect_item	psects[];
	IMPORT	TINY	f_psects;

/* number all external refs so that can use by emit_cbyte()
 * nb same code as used in pr_rel but here as new rel format
 * needs it earlier than old
 */
	for(k = f_psects,i=0;i<NLS_SIZE;i++)
		if(nlist[i].n_ptr && F_TYPE(i) == (LABEL|EXT_T))
			S_VAL(i,k++);
}

VOID	pr_r_item(where,i,type,val)
	FAST	FILE	where;
	COUNT	i;
	TINY	type;
	LVAL	val;
{
	FAST	COUNT	l,k;
	LOCAL	TEXT	zeros[15]	{0};
	LOCAL	TEXT	t,v[2];
	IMPORT	BOOL	rl_flag;
	IMPORT	COUNT	siglength;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	COUNT	int_size;

	k = rl_flag == -1 ? 7 : (siglength | 1);
	if(rl_flag == 1)	/* W new format	*/
		w_int(where,val,int_size);
	t = type;
	write(where,&t,1);
	l = lenstr(nlist[i].n_ptr);
	l = min(l,k);
	write(where,nlist[i].n_ptr,l);
	if(l < k) write(where,zeros,k - l);
	if(rl_flag == -1)	/* W old format	*/
		{
		itols(v,val);
		write(where,&v,2);
		};
}

COUNT	chk_for_unknowns()
{
	FAST	COUNT	i,j;
	IMPORT	BOOL	gu_flag,rl_flag;
	IMPORT	struct	nlist_item	nlist[];

	for(j=i=0;i<NLS_SIZE;i++)
		if(nlist[i].n_ptr && (F_TYPE(i) & ~PUB_T) == UNK)
			{
			if(!gu_flag)
				{
				lineno = F_VAL(i);
				perror(1,"%n not Dd",i);
				j++;
				};
/* in case force pass 2 (or using as backend) set to external	*/
			S_TYPE(i,LABEL|EXT_T);
			S_VAL(i,0);
			S_PS(i,rl_flag ? TEXT_PS : ABS_PS);
			};
	return(j);
}

/* counts no. of external + public symbols
 *	+ local labels + consts if nm_flag set
 */
COUNT	gt_no_of_symbols()		/* symbol table entries	*/
{
	FAST	COUNT	i,j,k;
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	BOOL	nm_flag;

	for(i=j=0;i<NLS_SIZE;i++)
		if(nlist[i].n_ptr)
			switch(F_TYPE(i))
				{
		case LABEL|PUB_T:
		case LABEL|EXT_T:
		case CONST|PUB_T:
				j++;
				break;
		case LABEL:	
		case CONST:
				if(nm_flag) j++;
				};
	return(j);
}

/* OBJECT CODE	*/

/* Dump object code in either
 * Whitesmiths relocatable format
 * or Intel absolute format
 */
/* common buffer	*/
TEXT	b_code[128]	{0};
COUNT	bc_index	0;
COUNT	rel_ps	0;	/* 0 = abs,1,2 = psects	*/
LVAL	ts_ladd	0;	/* last text address for which cbyte issued	*/
LVAL	ds_ladd	0;	/* last data address for which cbyte issued	*/
LVAL	rel_offset	0;
COUNT	rel_limit	127;

/*  buffer + ptrs for abs	*/
#define	EB_SIZE	16
LVAL	eb_low	0;
LVAL	eb_high	0;

VOID	emit_header()
{
	FAST	COUNT	l;
	IMPORT	FILE	op_ch;
	IMPORT	BOOL	op_flag,rl_flag,nm_flag;
	IMPORT	BOOL	hilo_flag,zpflag;
	IMPORT	COUNT	int_bnd;
	IMPORT	COUNT	int_size;
	IMPORT	COUNT	siglength;
	IMPORT	TINY	f_psects;
	IMPORT	LVAL	ts_ladd;
	IMPORT	LVAL	ds_ladd;
	LOCAL	TEXT	v[2];


	if(op_flag)
		switch(rl_flag)
case -1:				/* W old format	*/
			{
			l = 0404 + (zpflag ? 1 : 0)
				 + (hilo_flag ? 0 : 2);
			itols(v,l);
			write(op_ch,v,2);
			itols(v,gt_no_of_symbols());
			write(op_ch,v,2);
			for(l=1;l<f_psects;l++)
				{
				itols(v,psects[l].ps_high);
				write(op_ch,v,2);
				};
			pr_rel_format(op_ch);
			rel_ps = 0;
			break;
case 0:				/* INTEL abs format	*/
			dump_type();
			if(nm_flag) pr_nm_format(op_ch,"$");
			break;
case 1:				/* W new format		*/
			v[0] = 0x99;
			v[1] = ((siglength >> 1) & 07) |	/* siglength */
					(int_size == 2 ? 0 : 010) |
					(hilo_flag ? 0 :020) |
					(int_bnd << 6);				/* boundary limitations */
			write(op_ch,v,2);
			l = gt_no_of_symbols() * ((siglength|1) + 1 + 2);
			w_int(op_ch,(LVAL)l,2);
			for(l=1;l<f_psects;l++) w_int(op_ch,psects[l].ps_high,int_size);
			w_int(op_ch,(LVAL)0,int_size);				/*stack size */
			w_int(op_ch,(LVAL)0,int_size);				/*text offset*/
			w_int(op_ch,psects[TEXT_PS].ps_high,int_size);	/*data offset */
			ts_ladd = 0;
			ds_ladd = 0;
			set_ext_labels();
			};
}

VOID	emit(element)
	FAST	TINY	element;
{
	IMPORT	COUNT	rel_limit;
	IMPORT	BOOL	rl_flag;
	IMPORT	BOOL	pass;
	IMPORT	COUNT	rel_ps,bc_index;
	IMPORT	TEXT	b_code[];
	IMPORT	LVAL	eb_high,eb_low;
	IMPORT	LVAL	pc;

	if(pass)
		{
		if(rl_flag)		/* rel or abs ?	*/
			{
			put_cl_buffer(b_code[bc_index++] = element);
/* nb limit will be changed if emit a block;	*/
			if(bc_index == rel_limit)
					emit_rel_block(bc_index);
			}
		else
			{
			if(bc_index >= EB_SIZE ||
			    eb_high != pc)
				{
				dumpcode();
				eb_high = eb_low = pc;
				}
			put_cl_buffer(b_code[bc_index++] = element);
			eb_high++;
			};
		}
	pc++;
}

VOID	emit_address(r)
	FAST	struct	result	*r;
{
	IMPORT	BOOL	rl_flag,hilo_flag;
	IMPORT	COUNT	rel_ps,rel_limit,bc_index;
	IMPORT	LVAL	rel_offset;
	IMPORT	LVAL	ts_ladd;
	IMPORT	LVAL	ds_ladd;
	IMPORT	LVAL	pc;
	IMPORT	LVAL	ts_length;
	IMPORT	struct	nlist_item	nlist[];
	LOCAL	TEXT	m,v[2];
	IMPORT	TINY	c_psects;
	LOCAL	LVAL	trel_offset;
	if(rl_flag && r->r_ps)			/* abs addrsess == data	*/
		{
		if(rl_flag == -1)
			{
			emit_rel_block(bc_index);	/* dump current block	*/
			rel_limit = 7;	/* address will be o/p next block */
			};
		if(r->r_label != -1 && F_TYPE(r->r_label) == (LABEL|EXT_T))
			{
			rel_ps = F_VAL(r->r_label);
			rel_offset = r->r_offset;
			}
		else
			{
			rel_ps = r->r_ps;
			rel_offset = r->r_value;
			};
		if(rl_flag == 1)	/* W new format	*/
			{
/* should allow long ints here	*/
			if (rel_ps == DATA_PS)
				rel_offset =+ ts_length;
			emit_cbyte(NO);
/* NB dont allow zpage yet in this frmt	*/
			trel_offset = rel_offset;
			if(hilo_flag)		/* hi lo	*/
				{
				emit((TINY)( trel_offset >> 8));
				emit((TINY)trel_offset);
				}
			else
				{
				emit((TINY)trel_offset);
				emit((TINY)(trel_offset >> 8));
				};
			if (c_psects == DATA_PS)
				ds_ladd = pc;
			if (c_psects == TEXT_PS)
				ts_ladd = pc;
			return;
			};
		if( r->r_ps == 3)	/* zpage = 1 byte address */
			{
			put_cl_buffer((TINY)(rel_offset & BYTMASK));
			pc++;
			}
		else				/* others = 2 bytes	*/
			{
/* nb code always in hi low order but list in correct order	*/
			if(hilo_flag)
				{
				put_cl_buffer((TINY)((rel_offset >> 8) & BYTMASK));
				put_cl_buffer((TINY)(rel_offset & BYTMASK));
				}
			else
				{
				put_cl_buffer((TINY)(rel_offset & BYTMASK));
				put_cl_buffer((TINY)((rel_offset >> 8) & BYTMASK));
				};
			pc =+ 2;
			};
		}
	else				/* zpage handled diff in abs */
		if(hilo_flag)		/* hi lo	*/
			{
			emit((TINY)( r->r_value >> 8));
			emit((TINY)r->r_value);
			}
		else
			{
			emit((TINY)r->r_value);
			emit((TINY)( r->r_value >> 8));
			};
}
VOID	emit_cbyte(long_int)		/* W new format	*/
	BOOL	long_int;
{
	IMPORT	TINY	c_psects;
	IMPORT	COUNT	rel_ps;
	IMPORT	LVAL	ts_ladd;
	IMPORT	LVAL	ds_ladd;
	IMPORT	LVAL	pc;
	IMPORT	BOOL	pass,op_flag;
	LOCAL	FILE	f;
	LOCAL	TEXT	m,v[2];
	FAST	LVAL	rel_diff;

	if(!pass || !op_flag) return;
	f = psects[c_psects].ps_cfile;
/* nb rel_diff could be > 16 k - hence while loop	*/
	rel_diff = pc - ((c_psects == TEXT_PS) ? ts_ladd : ds_ladd);
	while(rel_diff)
		{
		if(rel_diff < 32)
			{
			m = rel_diff;
			write(f,&m,1);
			psects[c_psects].ps_clen++;
			rel_diff = 0;
			}
		else
			{
			rel_diff =- 32;
			v[0] = ((rel_diff >> 8) & 037) | 040;
			v[1] = rel_diff & BYTMASK;
			write(f,v,2);
			psects[c_psects].ps_clen =+ 2;
			rel_diff =- rel_diff & 017777;
			};
		};
/* issue rel info	*/
	m = ((min(rel_ps,47) + 16) << 2) | (long_int ? 2 : 0);
	write(f,&m,1);
	psects[c_psects].ps_clen++;
	if(rel_ps >= 47)
		{
		if(rel_ps < 175)
			{
			m = rel_ps - 47;
			write(f,&m,1);
			psects[c_psects].ps_clen++;
			}
		else
			{
			v[0] = ((rel_ps - 175) >> 8) | 0200;
			v[1] = (rel_ps - 175) & BYTMASK;
			write(f,v,2);
			psects[c_psects].ps_clen =+ 2;
			};
		};
	rel_ps = 0;			/* clear so can use same emit_rel	*/
}

VOID	emit_full_address(r)
	struct	result	*r;
{
	IMPORT	BOOL	rl_flag;
	if(rl_flag == -1 && r->r_ps == 3)		/* zpage needs xpansion	*/
		emit(0);
	emit_address(r);
}

VOID	endcode()		/* tidy up + emit end block	*/
{
	LOCAL	COUNT r_type, sav_ch,checksum;
	FAST	COUNT	i;
	IMPORT	BOOL	op_flag,rl_flag,start_flag;
	IMPORT	FILE	cout,op_ch;
	IMPORT	LVAL	st_address;

	if(op_flag)
		{
		if(rl_flag)
			{
			emit_rel_block(bc_index);
			for(i=1;i<MAX_PSECT;i++)
				{
				if(psects[i].ps_tfile)
					{
				   	if(psects[i].ps_tfile != op_ch)
						{
						close(psects[i].ps_tfile);
						if((psects[i].ps_tfile = open(
								psects[i].ps_tfname,
								READ,
								1)) < 0)
							perror(0,"A Q %s",
								psects[i].ps_tfname);
						while (psects[i].ps_tlen--)
							{
							read(psects[i].ps_tfile,
								b_code,
								1);
							write(op_ch,b_code,1);
							}
						close(psects[i].ps_tfile);
						};
					};
				};
			if(rl_flag == 1)	/* W new format	*/
				{
				pr_rel_format(op_ch);	/* dump names */
				for(i=1;i<3;i++)	/* nb ignore bss */
					{
					if(psects[i].ps_cfile)
						{
						close(psects[i].ps_cfile);
						if((psects[i].ps_cfile = open(
							psects[i].ps_cfname,
							READ,
							1)) < 0)
						perror(0,"A Q %s",
							psects[i].ps_cfname);
						while (psects[i].ps_clen--)
							{
							read(psects[i].ps_cfile,
							b_code,
							1);
							write(op_ch,b_code,1);
							}
						close(psects[i].ps_cfile);
						};
					write(op_ch,"\0",1);
					};
				};
			}
		else			/* INTEL format	*/
			{
			sav_ch = cout;
			cout = op_ch;
			dumpcode();
			r_type = start_flag ? 01 : 00;
			checksum = - ((st_address & BYTMASK)
					+ (st_address >> 8)
					+ r_type);
			printf(":00%4x%2x%2x\n",st_address,r_type,checksum);
			cout = sav_ch;
			};
		}
}
/* o/p routines for 'Whitesmith's relocatable format
 *
 * OLD format
 * A relocatable object image consists of
 *	a 4 word header
 *	a symbol table
 *	a text segment
 *	a data segment
 *
 * header consists of
 *	magic #	(0406 for 8080)
 *	# of symbol entries
 *	# of bytes of code defined by text segment
 *	# of bytes of code defined by data segment
 *	[# of bytes of code defined by zpage segment]
 *
 * symbol table entry
 *	flag byte
 *	7 byte name (null padded)
 *	value word
 * flags are
 *	3	undefined
 *	4	defined absolute
 *	5	defined text relative
 *	6	defined data relative
 *	7	defined zpage rel (if zpage machine - else ext ref)
 *	+010	for globally known (external/public) symbols
 *
 * text & data segments are sequence of load items
 *	+n {..n bytes}	load n following bytes unchanged
 *	-mn	m = 4 bit field;n = 3 bit field
 *		following word is relocated and n additional bytes
 *			loaded unchanged
 *	m = 1	add start address of text
 *	m = 2	add start address of data - start address of text
 *	m = 2 + i	add value of i'th symbol in symbol table
 *	m = 0	following byte = i
 *		add value of i+14'th symbol in symbol table
 *
 * NEW format
 *
 *	A relocatable object image comprises
 *		magic number	0x99 (at present)
 *		configuration byte
 *		header
 *		text seg.
 *		data seg.
 *
 *	config byte 
 *		r|bb|s|l|nnn
 *	r == 1 = no relocation info in file
 *	bb = storage bounds on multiples of 2^ss bytes
 *	s = sex (1== ls byte 1st otherwise ms. byte first)
 *	l = size of ints (1 == 4 bytes otherwise 2)
 *	nnn == siglength = (nnn << 1) + 1
 *	header consists of
 *		short int (2 byte) # of symbol table entries
 *		int (2/4) giving size of text seg.
 *		int	  giving size of data seg.
 *		int	  giving size of bss seg.
 *		int	  giving stack rqrd (-xflag)
 *		int	  giving text offset (0)
 *		int	  giving data offset (=size of text seg)
 *
 *	segments terminated by 0 control byte
 *		consists of Control byte(s) + associated data
 *	Control bytes 1..31 = 'skip' equal # of bytes
 *		     32..63 = 'skip' 32 + (n - 32) * 256 + m bytes
			(m = next byte)
 *		others are relocation bytes
 *			1-bit == load bias must be subtracted
 *			2-bit == long/short int
 *		symbol code = (contrl byte >> 2) - 16
 *	Symbol code 47 is repaced by next 1 or 2 bytes
 *		if next < 128 then sc = n + 47
 *		if next >=128 then sc = (n-128)*256 + 128 + 47 + next byte
 *	Symbol code 0 == no further relocation
 *		    1 == relocated by text bias
 *			
 *		    2 == relocated by data bias
 *		    3 == relocated by bss bias
 *		    4+ == symbol table entry - 4;
 */

VOID	w_int(f,i,is)
	FILE	f;
	FAST	LVAL	i;
	FAST	COUNT	is;
{
	LOCAL	TEXT	v[4];
	IMPORT	BOOL	hilo_flag;
	FAST	COUNT	j;

	if(hilo_flag)
		{
		j = 0;
#ifdef A32
		if(is == 4)
			{
			v[j++] = (i >> 24) & BYTMASK;
			v[j++] = (i >> 16) & BYTMASK;
			};
#endif
		v[j++] = (i >> 8) & BYTMASK;
		v[j] = i & BYTMASK;
		}
	else
		{
		v[0] = i & BYTMASK;
		v[1] = (i >> 8) & BYTMASK;
		v[2] = (i >> 16) & BYTMASK;
		v[3] = (i >> 24) & BYTMASK;
		}
	write(f,v,is);
}

VOID	emit_rel_block(size)
	COUNT	size;
{
	LOCAL	TEXT	m,v[2];
	FAST	FILE	opch;
	IMPORT	BOOL	pass;
	IMPORT	BOOL	op_flag;
	IMPORT	struct	psect_item	psects[];
	IMPORT	TINY	c_psects;
	IMPORT	BOOL	rl_flag;
	IMPORT	COUNT	rel_limit;

	if(pass && op_flag && (size || rel_ps))
		{
		opch = psects[c_psects].ps_tfile;
		if(rl_flag == -1)	/* W old format	*/
			{
			if(rel_ps)		/* W old format	*/
				{
				if(rel_ps > 15)	/* fit in 4 bits ? */
					{
					m = 0200 | size;
					write(opch,&m,1);
					psects[c_psects].ps_tlen++;
					m = rel_ps - 16;	/* o/p m next	*/
					}
				else
					m = 0200 | size  | (rel_ps << 3);
				write(opch,&m,1);
				psects[c_psects].ps_tlen++;
/* data has to be offset by length of text	*/
				itols(v,rel_offset + (rel_ps == 2 ? ts_length :0));
				write(opch,v,2);
				psects[c_psects].ps_tlen =+ 2;
				}
			else
				{
				m = size;
				write(opch,&m,1);
				psects[c_psects].ps_tlen++;
				};
			};
		write(opch,b_code,size);
		psects[c_psects].ps_tlen =+ size;
		};
	rel_ps = rel_offset = bc_index = 0;
	rel_limit = 127;
}

/* if change psect then push code into temp file
 * which will be read back into main o/p file at end
 */

VOID	change_psect(new_psect)
	FAST	COUNT	new_psect;
{
	LOCAL	TEXT	extn[4] {XTMP};
	IMPORT	TEXT	tmp_prefix[];
	IMPORT	BOOL	rl_flag;
	IMPORT	struct	psect_item	psects[];

	if(rl_flag)
		{
		emit_rel_block(bc_index);
		if(!psects[new_psect].ps_tfile)
			{
			extn[XMTMP] = 'B' + (new_psect << 1);
			psects[new_psect].ps_tfname = gb_large(MAX_FILENAME);
			psects[new_psect].ps_tfile = make_new_file(
							tmp_prefix,
							extn,
							psects[new_psect].ps_tfname,
							1);
			psects[new_psect].ps_tlen = 0;
			};
		if(rl_flag == 1 && !psects[new_psect].ps_cfile)	/* W new format	*/
			{
			extn[XMTMP] = 'B' + (new_psect << 1) + 1;
			psects[new_psect].ps_cfname = gb_large(MAX_FILENAME);
			psects[new_psect].ps_cfile = make_new_file(
						tmp_prefix,
						extn,
						psects[new_psect].ps_cfname,
						1);
			psects[new_psect].ps_clen = 0;
			};
		};
}
/* output routines for Intel absolute code
 * o/p record is 
 *	marker	:
 *	record length	0..ff		- in 8bit bytes (nb 16 bit machines)
 *	load address	0000 .. ffff
 *	record type	(00 is only one used)
 *	data
 *	checksum	=  negative sum of all fields(xcept marker)
 */



VOID	dumpcode()
{
	FAST	COUNT checksum,i,k;
	IMPORT	TEXT	hex[];
	IMPORT	COUNT	bc_index;
	IMPORT	LVAL	eb_low,eb_high;
	IMPORT	BOOL	op_flag;
	LOCAL	TEXT	a_hdr[9 + EB_SIZE + EB_SIZE + 2 + 1 + 1];

	a_hdr[0] = ':';
	a_hdr[7] = a_hdr[8] = '0';
	if( op_flag && bc_index)
		{
		a_hdr[1] = hex[(bc_index >> 4) & 017];
		a_hdr[2] = hex[bc_index & 017];
		a_hdr[3] = hex[(eb_low >> 12) & 017];
		a_hdr[4] = hex[(eb_low >> 8) & 017];
		a_hdr[5] = hex[(eb_low >> 4) & 017];
		a_hdr[6] = hex[(eb_low) & 017];
		checksum = bc_index	/* length */
				+ ((eb_low >> 8) & 0377)
				+ (eb_low & 0377);	/* nb rec type = 0 */
		for(i=9,k=0;k<bc_index;k++)
			{
			a_hdr[i++] = hex[(b_code[k] >> 4) & 017];
			a_hdr[i++] = hex[b_code[k] & 017];
			checksum =+ b_code[k];
			};
		k = -checksum;
		a_hdr[i++] = hex[(k >> 4) & 017];
		a_hdr[i++] = hex[k & 017];
		a_hdr[i++] = '\n';
		write(op_ch,a_hdr,i);
		};
	bc_index = 0;
}
VOID	dump_type()		/* emit info record giving processor type */
{
	FAST	TEXT	*s;
	IMPORT	BOOL	op_flag;
	IMPORT	struct	tp_item	typepars[];
	IMPORT	COUNT	u_type;
	IMPORT	FILE	op_ch;

	if(op_flag)
		{
		write(op_ch,"$",1);
		s = typepars[u_type].tp_uid;
		write(op_ch,s,lenstr(s));
		write(op_ch,"\n",1 );
		}
}


