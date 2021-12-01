/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - COMMON ROUTINES PASS 1 AND 2
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include	<std.h>
#include	"xa.h"

VOID	xy_addr()		/* addr(ess)	*/
{
	LOCAL	struct	result	rs;
	do {
		expression(&rs);
		emit_full_address(&rs);
		listline();
	 }while(consume(','));
}

VOID	xy_byte()		/* byte	*/
{
	FAST	LVAL	i;
	FAST	COUNT	j;
	IMPORT	BITS	list_mode,nlist_mode;
	IMPORT	BS	bs,NBS;
	IMPORT	LVAL	lv;
	do {
		while(consume('\n')) {};
		if(consume('['))
			{
			move_pc_by(const_expression());
			failifnot(']');
			}
		else
			{
			emit((TINY)checkbyte(i = const_expression()));
			nlist_mode =| LIST_text;
			listline();
			if(consume('('))
				{
				for(j=const_expression();j>1;j--)
					{
					emit((TINY)i);
					listline();
					};
				failifnot(')');
				};
			};
		} while(consume(','));
		list_mode =& ~LIST_text;
}

VOID	xy_text()		/* text	*/
{
	TEXT	*s;
	IMPORT	TEXT	**strings[];
	do {
		while(consume('\n')) {};
		nlist_mode =| LIST_text;
		if(bs == STRNG)
			{
			for(s = strings[lv >> STR_OFF][lv & STR_MASK];*s;)
				{
				if(*s == '\\')
					{
					s++;
					emit(acnvrt(eschar(&s)));
					}
				else
					emit(acnvrt(*s++));
				listline();
				};
			NBS;
			}
		else
			{
			emit(acnvrt(const_expression()));
			listline();
			};
		} while(consume(','));
	list_mode =& ~LIST_text;
}

VOID	xy_word()		/* word	*/
{
	FAST	COUNT	j;
	LOCAL	struct	result	rs;
	do {
		while(consume('\n')) {};
		if(consume('['))
			{
			move_pc_by(2 * const_expression());
			failifnot(']');
			}
		else
			{
			expression(&rs);
			emit_full_address(&rs);
			nlist_mode =| LIST_text;
			listline();
			if(consume('('))
				{
				for(j=const_expression();j>1;j--)
					{
					emit_full_address(&rs);
					listline();
					};
				failifnot(')');
				};
			};
		} while(consume(','));
	list_mode =& ~LIST_text;
}
VOID	xy_long(sym)
	BS	sym;
{
	FAST	COUNT	n,i;
	FAST	COUNT	j;
	LOCAL	COUNT	k;
	LOCAL	TEXT	*t;
	LOCAL	UTINY	b[8],tmp;
	LOCAL	UTINY	r01[8] = {0,0,0,0,0,0,0,1},
					r1[8] = {0,0,0,1,0,0,0,0};
	IMPORT	BOOL	fp_flag,hilo_flag;
	LOCAL	TINY	sign;

	k = sym == LFLOAT ? 8 : 4;
	do {
		while(consume('\n')) {};
		if(consume('['))
			{
			move_pc_by(const_expression() * k);
			failifnot(']');
			}
		else
			{
			for(n=0;n<8;n++) b[n] = 0;
			sign = consume('-') ? 0200 : 0;
			if(bs == FPNUM || bs == LGNUM)
				t = strings[lv >> STR_OFF][lv & STR_MASK];
			if(bs == FPNUM)
				{
				NBS;
				strfpt(b,t,(fp_flag << 1)|(k == 8 ? 1 : 0));
				b[0] =| sign;
				}
			else if(bs == LGNUM)
				{
				NBS;
				strfpt(b,t,k == 8 ? 7 : 6);
				if(sign)
					{
					for(i = 0;i<8;i++) b[i] = ~b[i];
					addl(b,k == 8 ? &r01 : &r1);
					};
				}
			else
				{
				i = const_expression();
				if(sign) i = - i;
				b[k-1] = i & BYTMASK;
				b[k-2] = i >> 8;
				};
			for(n = 0;n<k;n =+ 2)
				{
				if(!hilo_flag)
					{
					tmp = b[n];b[n] = b[n+1];b[n+1] = tmp;
					};
				};
			for(n = 0;n<k;n++)
				{
				if(n == 4) listline();
				emit(b[n]);
				};
			nlist_mode =| LIST_text;
			listline();
			if(consume('('))
				{
				for(j=const_expression();j>1;j--)
					{
					for(n = 0;n<k;n++)
						{
						if(n == 4) listline();
						emit(b[n]);
						};
					};
				failifnot(')');
				};
			};
		} while(consume(','));
	list_mode =& ~LIST_text;
}

VOID	x_ascii()		/* ascii 7/8 odd/even	*/
{
	FAST	COUNT	i;
	IMPORT	BOOL	ascii_flag;
	if(consume(ODD))	ascii_flag = 0;
	else if(consume(EVEN))	ascii_flag = 1;
	else
		{
		i = const_expression();
		if(i == 7 || i == 8)
			ascii_flag = i;
		};
}

VOID	x_option()		/* opt(io)n	*/
{
	IMPORT	BITS	optn_flags;

	optn_flags = const_expression();
}

VOID	x_list()		/* list	*/
{
	set_list_flags(bs);
}

VOID	move_pc_by(n)
	FAST	COUNT	n;
{
	IMPORT	BOOL	pass;
	IMPORT	BOOL	rl_flag;
	IMPORT	LVAL	pc;
	if(pass && rl_flag)
		while(n--) emit(0);
	else
		pc=+n;
}
/* handle LIST directive
 * LIST term +/- term ....
 * .macro lists all macros,.include all included files
 * .if all non assembled conditionals
 * .text all lines (except 1st) of .byte,.word & .text
 * + turns on this listing
 * - turns it off
 * .list == current things on
 * thus .list + /n	turns all on
 *      .list - /n	turns all off
 *      .list .include	just all included files
 *      .list - .macro	everything other than macros
 */
BITS	l_term(tbs)
	FAST	BS	tbs;
{
	IMPORT	BS	bs,NBS;
	IMPORT	BITS	list_flags;
	switch(tbs)
		{
case '+':
case '-':
		return(list_flags);
case '\n':
		return(31);		/* every thing	*/
case LIST:
		NBS;	return(list_flags);
case INCLD:
		NBS;	return(LIST_include);
case MACRO:
		NBS;	return(LIST_macro);
case IFSYM:
		NBS;	return(LIST_if);
case TEXTSYM:
		NBS;	return(LIST_text);
default:
		NBS;	perror(1,"I %z J",LIST); return(0);
		}
}
VOID	set_list_flags(tbs)
	FAST	BS	tbs;
{
	IMPORT	BS	bs,NBS;
	IMPORT	BITS	list_flags;
	list_flags = l_term(tbs);
	for(;;)
		if(consume('+')) list_flags =| l_term(bs);
		else if(consume('-')) list_flags =& ~l_term(bs);
		else return;
}


