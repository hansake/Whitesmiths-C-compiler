/*	MACHINE-INDEPENDENT HEADER FOR DB
 *	copyright (c) 1981 by Whitesmiths, Ltd.
 */

/*	bits returned by getrelb (and expected by getsym)
 */
#define SYMUDEF 1
#define SYMABS	2
#define SYMTREL 4
#define SYMDREL 010
#define SYMBREL 020
#define SYMSREL 040
#define SYMPREL	0100
#define SYMLREF 0200
#define SYMRFLD 077
#define SYMALLR 036
#define SYMBITS 8

/*	Whitesmiths format relocation values   
 */
#define RELPC	1
#define RELISLG 2
#define RELABS	0
#define RELTXT	1
#define RELDATA 2
#define RELBSS	3

/*	Whitesmiths format symbol table flag values
 */
#define STRFLD	7
#define STUDEF	0
#define STABS	4
#define STTREL	5
#define STDREL	6
#define STBREL	7
#define	STEXT	010

/* db global constants
 */
#define CORESIZE 0177776
#define PRMPCH ">"
#define NAMSIZE 64
#define SYMMAXL 12
#define FORWARD 1
#define BACKWARD -1
#define PATSIZE 32
#define SUBSIZE 32

/*	structure for file information
 */
typedef struct {
	COUNT hsize;
	ULONG tfirst;
	ULONG tlast;
	LONG toff;
	ULONG dfirst;
	ULONG dlast;
	LONG doff;
	ULONG bfirst;
	ULONG blast;
	ULONG dol;
	} FINFO;

/*	structure for marking addresses for k command
 */
typedef struct {
	BOOL assigned;
	BOOL ispf;
	ULONG num;
	} MARK;
