/*	XA8 GENERAL PURPOSE CROSS ASSEMBLER - IBM S/36 SYNTAX
 *	copyright (c) 1981, 1984 by Real Time Systems Partnership
 */

#include	<std.h>
LOCAL	TEXT	v_s36[]	"@(#)s36	1.0	21:Nov:86";
#include	"xa.h"

TEXT	VERSION[]	"\nXS36 1.5b (21:Nov:86)\n";
TEXT	COPYRIGHT[]	"V1.5b: copyright (c) 1981, 1984 by Real Time Systems Partnership\nS/36 version copyright (c) 1986 by UNISOFT AB, Sweden";
TEXT	*px_type	={"DATA"};	/* processor type	*/
TEXT	*_pname		={"xs36"};	/* name of program	*/

/* processor information	*/
/* define list of supported types	*/

GLOBAL	i_null();		/* null - data only	*/
VOID	i_s36();

struct	tp_item	typepars[] {
	"data",		&i_null,
	"s36",		&i_s36,
	0};


/* define register classes */
#define XR1 1
#define XR2 2
#define ARR 8
#define ANYREG 0xff

LOCAL	struct	presets rgs36[] {
	"xr1"	,SPECIAL,	0x01,
	"xr2"	,SPECIAL,	0x02,
	"psr"	,SPECIAL,	0x04,
	"arr"	,SPECIAL,	0x08,
	"iar"	,SPECIAL,	0x10,
	"wr4"	,SPECIAL,	0x44,
	"wr5"	,SPECIAL,	0x45,
	"wr6"	,SPECIAL,	0x46,
	"wr7"	,SPECIAL,	0x47,
	"pdir"	,SPECIAL,	0xa0,
	"pxr1"	,SPECIAL,	0xa1,
	"pxr2"	,SPECIAL,	0xa2,
	"piar"	,SPECIAL,	0xa3,
	0};

VOID	i_s36()
	{
	IMPORT	struct	opitem	(*optable)[],ops36[];
	IMPORT	VOID		(*assemble)(),ass36();
	IMPORT	BOOL		hilo_flag;

	assemble	= &ass36;
	optable		= &ops36;
	calc_opt_length();
	hilo_flag	= YES;
/* enter reg names into table	*/
	put_in_nametable(&rgs36);
	}

VOID	ass36(inst)
	COUNT	inst;
	{
	IMPORT	struct	opitem	ops36[];
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	BS	bs,NBS;
	IMPORT	LVAL	lv, pc;
	LOCAL	struct	result	rs1, rs2;
	FAST	struct opitem *p;
	FAST	COUNT	reg1,reg2;
	COUNT len1, len2;

	p = &ops36[inst];
	NBS;		/* grab next input symbol */
	switch (p->optype)
		{
	case 0:
		break;
	case 1:			/* a, l, la, s, st */
		reg1 = geaddr(XR1|XR2, &rs1);
		failifnot(',');
		reg2 = getreg36(ANYREG);
		emit(p->opbase + ((reg1 * 4) << 4));
		emit(reg2);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		break;
	case 2:			/* alc, clc, ed, itc, mvc, slc	*/
		reg1 = gadrlen(XR1|XR2, &rs1, &len1, 256);
		failifnot(',');
		reg2 = geaddr(XR1|XR2, &rs2);
		emit(p->opbase + ((reg2 + (reg1 * 4)) << 4));
		emit(len1);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		if (reg2)
			emit(rs2.r_value);
		else
			emit_address(&rs2);
		break;
	case 3:			/* cli, mvi, sbf, sbn, sli, tbf, tbn	*/
		reg1 = geaddr(XR1|XR2, &rs1);
		failifnot(',');
		checkbyte(reg2 = const_expression());
		emit(p->opbase + ((reg1 * 4) << 4));
		emit(reg2);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		break;
	case 4:			/* az, sz, zaz	*/
		reg1 = gadrlen(XR1|XR2, &rs1, &len1, 31);
		failifnot(',');
		reg2 = gadrlen(XR1|XR2, &rs2, &len2, 16);
		if (len1 < len2)
			perror(2, "length 1 less than length 2");
		emit(p->opbase + ((reg2 + (reg1 * 4)) << 4));
		emit(((len1 - len2) << 4) + len2);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		if (reg2)
			emit(rs2.r_value);
		else
			emit_address(&rs2);
		break;
	case 5:			/* bc	*/
		reg1 = geaddr(XR1|XR2|ARR, &rs1);
		failifnot(',');
		checkbyte(reg2 = const_expression());
		if (reg1 == ARR)
			reg1 = 3;
		emit(p->opbase + (reg1 << 4));
		emit(reg2);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		break;
	case 6:			/* bCC 	*/
		reg1 = geaddr(XR1|XR2|ARR, &rs1);
		if (reg1 == ARR)
			reg1 = 3;
		emit(0xc0 + (reg1 << 4));
		emit(p->opbase);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		break;
	case 7:			/* jc	*/
		expression(&rs1);
		check(reg1 = (rs1.r_value - (pc + 3)), -255, 255);
		failifnot(',');
		checkbyte(reg2 = const_expression());
		if (reg1 < 0)
			{
			emit(0xf1);
			reg1 = -reg1;
			}
		else
			emit(0xf2);
		emit(reg2);
		emit(reg1);
		break;
	case 8:			/* jCC	*/
		expression(&rs1);
		check(reg1 = (rs1.r_value - (pc + 3)), -255, 255);
		if (reg1 < 0)
			{
			emit(0xf1);
			reg1 = -reg1;
			}
		else
			emit(0xf2);
		emit(p->opbase);
		emit(reg1);
		break;
	case 9:			/* src */
		reg1 = gadrlen(XR1|XR2, &rs1, &len1, 16);
		failifnot(',');
		check(reg2 = const_expression(), 0, 16);
		emit(p->opbase + ((reg1 * 4) << 4));
		emit(((reg2 - 1) << 4) + len1);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		break;
	case 10:		/* xfer	*/
		check(reg1 = const_expression(), 0, 1);
		failifnot(',');
		check(reg2 = const_expression(), 0, 255);
		emit(p->opbase);
		emit(reg2);
		emit(reg1);
		break;
	case 11:		/* svc	*/
		check(reg1 = const_expression(), 0, 255);
		failifnot(',');
		check(reg2 = const_expression(), 0, 255);
		emit(p->opbase);
		emit(reg2);
		emit(reg1);
		break;
	case 12:		/* bd	*/
		geaddr(XR1|XR2, &rs1);
		emit(p->opbase);
		emit(0x10);
		emit_address(&rs1);
		break;
	case 13:		/* mvx	*/
		reg1 = gadrlen(XR1|XR2, &rs1, &len1, 3);
		failifnot(',');
		reg2 = geaddr(XR1|XR2, &rs2);
		emit(p->opbase + ((reg2 + (reg1 * 4)) << 4));
		emit(len1 + 1);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		if (reg2)
			emit(rs2.r_value);
		else
			emit_address(&rs2);
		break;
	case 14:		/* mzz, mzn, mnz, mnn	*/
		reg1 = geaddr(XR1|XR2, &rs1);
		failifnot(',');
		reg2 = geaddr(XR1|XR2, &rs2);
		emit(8 + ((reg2 + (reg1 * 4)) << 4));
		emit(p->opbase);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		if (reg2)
			emit(rs2.r_value);
		else
			emit_address(&rs2);
		break;
	case 15:			/* ali */
		reg1 = geaddr(XR1|XR2, &rs1);
		failifnot(',');
		checkbyte(reg2 = const_expression());
		emit(p->opbase + ((reg1 * 4) << 4));
		emit(256 - reg2);
		if (reg1)
			emit(rs1.r_value);
		else
			emit_address(&rs1);
		}
	}

/*	get address(length) or offset(length,reg), return reg
 */
COUNT gadrlen(wregs, rs, len, maxoff)
	COUNT wregs;
	struct result *rs;
	COUNT *len;
	COUNT maxoff;
	{
	COUNT reg;

	expression(rs);
	failifnot('(');
	check(*len = const_expression(), 1, maxoff);
	*len -= 1;
	if (consume(','))
		{
		reg = getreg36(wregs);
		if(rs->r_ps || rs->r_unk)	/* test if constant expr */
			perror(2,"constant X");
		checkbyte(rs->r_value);
		}
	else
		reg = 0;
	failifnot(')');
	return(reg);
	}

/*	get address or offset(,reg) , return reg
 */
COUNT geaddr(wregs, rs)
	COUNT wregs;
	struct result *rs;
	{
	COUNT reg;

	expression(rs);
	if (consume('('))
		{
		failifnot(',');
		reg = getreg36(wregs);
		if(rs->r_ps || rs->r_unk)	/* test if constant expr */
			perror(2,"constant X");
		checkbyte(rs->r_value);
		failifnot(')');
		}
	else
		reg = 0;
	return(reg);
	}

/*	get valid register
 */
COUNT	getreg36(wregs)
	COUNT wregs;
	{
	IMPORT	struct	nlist_item	nlist[];
	IMPORT	BS	bs,NBS;
	IMPORT	LVAL	lv;
	FAST COUNT r;
	FAST LVAL k;

	k = lv;
	if(consume(ID))
		{
		r = F_VAL(k);
		if((F_TYPE(k) != SPECIAL) || !(wregs & r))
			perror(1,"I G T %n",k);
		return(r);
		}
	else
		{
		check(r = const_expression(), 0, 255);
		if(!(wregs & r))
			perror(1,"I register value");
		return(r);
		}
	}
