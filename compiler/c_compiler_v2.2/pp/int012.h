/*	USED BY C PASS0 & PASS1 & PASS2
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */

/*	the types
		op = ((UNOP | BINOP) & type)
 */
#define BINOP	0200
#define UNOP	0100

/*	the operators
 */
#define LAND	0301
#define LANDAND	0202
#define LCOMP	0103
#define LDECR	0104
#define LDIVIDE	0205
#define LGAND	0206
#define LGDIV	0207
#define LGETS	0210
#define LGLSH	0211
#define LGRSH	0212
#define LGMIN	0213
#define LGMOD	0214
#define LGOR	0215
#define LGPLU	0216
#define LGREAT	0217
#define LGEQ	0220
#define LGTIM	0221
#define LGXOR	0222
#define LINCR	0123
#define LISEQ	0224
#define LLESS	0225
#define LLEQ	0226
#define LLSHIFT	0227
#define LMINUS	0330
#define LMODULO	0231
#define LNOT	0132
#define LNOTEQ	0233
#define LOR		0234
#define LOROR	0235
#define LPLUS	0336
#define LQUERY	0237
#define LRSHIFT	0240
#define LTIMES	0342
#define LXOR	0243
