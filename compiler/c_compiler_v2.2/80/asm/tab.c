/*	TABLES FOR TRANSLATOR VERSION
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "t.h"

LOCAL CODETAB gettab[] {
	Ir, Nn, "lxi L,R",
	IBC, A, "stax b",
	A, IBC, "ldax b",
	IDE, A, "stax d",
	A, IDE, "ldax d",
	INN, HL, "shld L",
	HL, INN, "lhld R",
	INN, A, "sta L",
	A, INN, "lda R",
	Cr, N, "mvi L,R",
	Cr, Cr, "mov L,R",
	SP, HL, "sphl",
	BC, DE, "mov c,e;mov b,d",
	BC, HL, "mov c,l;mov b,h",
	DE, BC, "mov e,c;mov d,b",
	DE, HL, "mov e,l;mov d,h",
	HL, BC, "mov l,c;mov h,b",
	HL, DE, "mov l,e;mov h,d",
	0};

LOCAL CODETAB poptab[] {
	BC, SP, "pop b",
	DE, SP, "pop d",
	HL, SP, "pop h",
	AF, SP, "pop psw",
	SP, BC, "push b",
	SP, DE, "push d",
	SP, HL, "push h",
	SP, AF, "push psw",
	0};

LOCAL CODETAB swaptab[] {
	HL, ISP, "xthl",
	ISP, HL, "xthl",
	HL, DE, "xchg",
	DE, HL, "xchg",
	0};

LOCAL CODETAB plutab[] {
	HL, Ir, "dad R",
	Ir, 1, "inx L",
	Cr, 1, "inr L",
	A, Cr, "add R",
	A, N, "adi R",
	0};

LOCAL CODETAB mintab[] {
	Ir, 1, "dcx L",
	Cr, 1, "dcr L",
	A, Cr, "sub R",
	A, N, "sui R",
	0};

LOCAL CODETAB pluctab[] {
	A, Cr, "adc R",
	A, N, "aci R",
	0};

LOCAL CODETAB minctab[] {
	A, Cr, "sbb R",
	A, N, "sbi R",
	0};

LOCAL CODETAB andtab[] {
	A, Cr, "ana R",
	A, N, "ani R",
	0};

LOCAL CODETAB xortab[] {
	A, Cr, "xra R",
	A, N, "xri R",
	0};

LOCAL CODETAB ortab[] {
	A, Cr, "ora R",
	A, N, "ori R",
	0};

LOCAL CODETAB cmptab[] {
	A, Cr, "cmp R",
	A, N, "cpi R",
	0};

LOCAL CODETAB rottab[] {
	A, 1, "rlc",
	A, -1, "rrc",
	0};

LOCAL CODETAB rotctab[] {
	A, 1, "ral",
	A, -1, "rar",
	0};

LOCAL CODETAB rototab[] {
	A, 1, "stc;ral",
	A, -1, "stc;rar",
	0};

LOCAL CODETAB rotztab[] {
	A, 1, "ora a;ral",
	A, -1, "ora a;rar",
	0};

LOCAL CODETAB jmptab[] {
	Jc, INN, "jmp R",
	Jc, IHL, "pchl",
	0};

LOCAL CODETAB jctab[] {
	Jc, INN, "L R",
	0};

LOCAL CODETAB goesup[] {
	BC, HL, "mov m,c;inx h;mov m,b",
	DE, HL, "mov m,e;inx h;mov m,d",
	0};

LOCAL CODETAB getsup[] {
	BC, HL, "mov c,m;inx h;mov b,m",
	DE, HL, "mov e,m;inx h;mov d,m",
	0};

LOCAL CODETAB getsbya[] {
	HL, HL, "mov a,m;inx h;mov h,m;mov l,a",
	0};

LOCAL CODETAB getsnot[] {
	A, A, "cma",
	0};

/*	indexed by bincodes
 */
GLOBAL CODETAB *tabtab[] {
	gettab, poptab, swaptab, plutab, mintab, pluctab, minctab,
	andtab, xortab, ortab, cmptab, rottab, rotctab, rototab, rotztab,
	jmptab, jctab, getsup, goesup, getsbya, getsnot, 0};
