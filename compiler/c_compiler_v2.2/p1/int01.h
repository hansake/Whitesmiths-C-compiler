/*	INTERFACE BETWEEN C PASS0 & PASS1
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */

/*	the types and mask
		cons = (MASK & type == CONS)
 */
#define MASK	0360
#define CONS	0020

/*	the keywords
 */
#define LAUTO	0041
#define LBREAK	0042
#define LCASE	0043
#define LCHAR	0044
#define LCONTIN	0045
#define LDFAULT	0046
#define LDO		0047
#define LDOUBLE	0050
#define LELSE	0051
#define LEXTERN	0052
#define LFLOAT	0053
#define LFOR	0054
#define LGOTO	0055
#define LIF		0056
#define LINT	0057
#define LLONG	0060
#define LREG	0061
#define LRETURN	0062
#define LSHORT	0063
#define LSTATIC	0064
#define LSTRUCT	0065
#define LSWITCH	0066
#define LTYPDEF	0067
#define LUNION	0070
#define LUNSIGN	0071
#define LWHILE	0072

/*	pseudo keyword
 */
#define LSIZEOF	0141

/*	created lexemes, can't be input
 */
#define DARG	0073
#define DEXTERN	0074
#define DSTATIC	0075
#define FSTATIC 0076

/*	the constants
 */
#define LDNUM	0021
#define LIDENT	0022
#define LIFILE	0023
#define LLINENO	0024
#define LLNUM	0025
#define LSNUM	0026
#define LSTRING	0027
#define LCNUM	0030
#define LULNUM	0031
#define LUSNUM	0032
#define LUCNUM	0033

/*	the punctuation
 */
#define LCOLON	0001
#define LCOMMA	0002
#define LDOT	0003
#define LLBRACK	0004
#define LLCURLY	0005
#define LLPAREN	0006
#define LPOINTS	0007
#define LRBRACK	0010
#define LRCURLY	0011
#define LRPAREN	0012
#define LSCOLON	0013
