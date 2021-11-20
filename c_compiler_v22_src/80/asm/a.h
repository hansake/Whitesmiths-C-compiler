/*	HEADER FILE FOR A NATURAL
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#define LENNAME	8
#define MAXNAME	64
#define MAXSTR	127
#define NSYM	64

/*	pseudo types
 */
#define LEX		int
#define META	int

#define CODETAB	struct codetab
struct codetab {
	LEX cl, cr;
	TEXT *code;
	};

#define PRETAB	struct pretab
struct pretab {
	TEXT *prename;
	LEX pretype;
	};

#define TERM	struct term
struct term {
	TEXT *next;
	LEX ty;
	COUNT val;
	TEXT *base;
	TEXT nm[LENNAME];
	};

#define LIT		struct lit
struct lit {
	TERM t;
	TEXT *lbl;
	};

/*	modifiers for token types
 */
#define ISTERM	00100
#define BINOP	00400
#define UNOP	00200
#define TDEF	04000
#define DEFF	02000
#define PUBF	01000
#define TMASK	00777
#define INDF	00010

/*	punctuation types
 */
#define EOL		-2
#define LPAREN	-3
#define RPAREN	-4
#define LBRACK	-5
#define RBRACK	-6
#define LABEL	-7

/*	operator types
 */
#define OR		0601
#define AND		0602
#define INDIR	0203
#define PLUS	0604
#define MINUS	0605
#define GETS	0606
#define XOR		0607
#define PLUSC	0410
#define GOESTO	0411
#define MINUSC	0412
#define DEFINE	0413
#define POP		0415
#define SWAP	0416
#define PUSH	0417
#define GETSUP	0420
#define GOESUP	0421
#define ROT		0422
#define ROTC	0423
#define ROTO	0424
#define ROTZ	0425
#define GETSBYA	0426
#define GETSNOT	0427
#define CNZ		0630
#define CZ		0631
#define CNC		0632
#define CC		0633
#define CPO		0634
#define CPE		0635
#define CP		0636
#define CM		0637
#define JNZ		0640
#define JZ		0641
#define JNC		0642
#define JC		0643
#define JPO		0644
#define JPE		0645
#define JP		0646
#define JM		0647
#define CALL	0650
#define JMP		0651
#define JCOND	0652
#define CMP		0453
#define PUBLIC	0254
#define COMMA	0655

/*	term types
 */
#define STRING	0100
#define NN	0101
#define BC	0102
#define DE	0103
#define HL	0104
#define SP	0105
#define IX	0106
#define IY	0107
#define N	0110
#define INN	0111
#define IBC	0112
#define IDE	0113
#define IHL	0114
#define ISP	0115
#define IIX	0116
#define IIY	0117
#define B	0120
#define C	0121
#define D	0122
#define E	0123
#define H	0124
#define L	0125
#define F	0126
#define A	0127
#define AF	0130

/*	meta tokens for code tables
 */
#define Cr	2
#define Ir	3
#define Jc	4
#define Nn	5

/*	codes for intermediate file
 */
#define DATA	0
#define END		1
#define RELOC	2
#define XSEG	3
