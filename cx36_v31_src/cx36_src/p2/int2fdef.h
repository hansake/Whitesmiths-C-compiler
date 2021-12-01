/*	FUNCTION DEFINITIONS FOR PASS 2
 */
#ifndef __
#define __(a)	()
#endif

/*	FILE: p2conf.c
 */
GLOBAL BOOL putasm __((TEXT *fmt, ...));
GLOBAL COUNT getch __((VOID));
GLOBAL TEXT *sjmp __((TEXT *s));
GLOBAL VOID backch __((COUNT c));
GLOBAL VOID csect __((COUNT nsect));
GLOBAL VOID cswitch __((COUNT nsect));
GLOBAL VOID putchr __((COUNT c));
GLOBAL VOID putftr __((VOID));
GLOBAL VOID puthdr __((VOID));
GLOBAL VOID setmap __((BYTES xmask));
GLOBAL VOID xref __((TEXT *s));

/*	FILE: p2esc.c
 */
GLOBAL TAB *bitstou __((EXPR *p, RESOURCE set));
GLOBAL TAB *divimm __((EXPR *p, RESOURCE set));
GLOBAL TAB *dolsh __((EXPR *p, RESOURCE set));
GLOBAL TAB *dorsh __((EXPR *p, RESOURCE set));
GLOBAL TAB *mulimm __((EXPR *p, RESOURCE set));
GLOBAL TAB *ntobits __((EXPR *p));
GLOBAL TAB *signicmp __((EXPR *p, RESOURCE set));
GLOBAL TAB *signcmp __((EXPR *p, RESOURCE set));
GLOBAL TAB *strtostr __((EXPR *p, RESOURCE set));
GLOBAL TAB *tstbits __((EXPR *p));
GLOBAL TAB *utobits __((EXPR *p, RESOURCE set));

/*	FILE: p2expr.c
 */
GLOBAL RESOURCE binop __((EXPR *p, WANTS want, RESOURCE set, TEXT *tab));
GLOBAL RESOURCE fix __((EXPR *p, WANTS want, RESOURCE set, TYPE ty));
GLOBAL RESOURCE force __((EXPR *p, WANTS want, RESOURCE set, TYPE ty));
GLOBAL RESOURCE getsop __((EXPR *p, WANTS want, RESURCE set, WANTS pwant));
GLOBAL RESOURCE move __((EXPR *l, EXPR *r, RESOURCE set));
GLOBAL RESOURCE postop __((EXPR *p, WANTS want, RESOURCE set));
GLOBAL RESOURCE tempify __((EXPR *p, WANTS want, RESOURCE set));
GLOBAL BOOL adequ __((ADDR *pl, ADDR *pr));
GLOBAL BOOL foreg __((EXPR *p, WANTS want, RESOURCE set, LEX ty, SPACE pspace));
GLOBAL CODE **voidex __((CODE **qb, EXPR *r));
GLOBAL LONG autbuy __((BYTES size));
GLOBAL VOID fixplus __((EXPR *p));

/*	FILE: p2gen.c
 */
GLOBAL BOOL canadd __((EXPR *p, ADDR *la, ADDR *ra, BOOL bxok));
GLOBAL BOOL genac __((EXPR *p, TEXT *tab, RESOURCE set));
GLOBAL RESOURCE gen __((EXPR *p, TEXT *tab, RESOURCE set));
GLOBAL RESOURCE reref __((EXPR *l, EXPR *r, RESOURCE set));
GLOBAL TEXT *genkall __((TEXT *q));
GLOBAL TEXT *xnm __((TINY idx));
GLOBAL TINY canidx __((TINY lidx, TINY ridx));
GLOBAL VOID genad __((EXPR *p, COUNT up, BOOL compl, COUNT type));

/*	FILE: p2gets.c
 */
GLOBAL TYPE gettype __((SPACE *space, SPACE *pspace, LONG *l));
GLOBAL SPACE mapspace __((BITS space));
GLOBAL BOOL getbss __((TEXT *name, BYTES *psize));
GLOBAL EXPR *addrof __((EXPR *q));
GLOBAL EXPR *gexpr __((VOID));
GLOBAL LABEL glabel __((VOID));
GLOBAL LEX doswtab __((VOID));
GLOBAL LEX gcode __((VOID));
GLOBAL LEX needch __((VOID));
GLOBAL TEXT *chread __((VOID));
GLOBAL TEXT *gname __((TEXT *s));
GLOBAL VOID chput __((TEXT *args, ...));
GLOBAL VOID chwrite __((TEXT *s, BYTES n));
GLOBAL VOID doswnum __((ULONG num, BOOL islong));
GLOBAL VOID getspinfo __((SPACE *pspace, BYTES *psize, TEXT name[]));
GLOBAL VOID gint __((LONG *s));
GLOBAL VOID makspace __((TEXT *name));

/*	FILE: p2jump.c
 */
GLOBAL RESOURCE boolify __((EXPR *p, EXPR *q, EXPR *l, EXPR *r, WANTS want, RESOURCE set));
GLOBAL RESOURCE fncall __((EXPR *p, WANTS want, RESOURCE set));
GLOBAL BYTES dobuiltin __((EXPR *p, BYTES size));
GLOBAL BYTES fnargs __((EXPR *p, ARGINT *pset));
GLOBAL BYTES setfn __((EXPR *p, WANTS want));
GLOBAL CODE **jc __((CODE **qb, LEX tok, HEADER *p, LABEL lab, EXPR *l, EXPR *r, RESOURCE set));
GLOBAL CODE **jcond __((CODE **qb, LEX tok, HEADER *p, EXPR *l, EXPR *r));
GLOBAL LABEL jf __((EXPR *p, LABEL fb, LABEL tb, RESOURCE set));
GLOBAL LABEL jt __((EXPR *p, LABEL fb, LABEL tb, RESOURCE set));
GLOBAL LEX cmpset __((EXPR *p, EXPR *l, EXPR *r, LEX tok));
GLOBAL VOID setbool __((EXPR *p));

/*	FILE: p2main.c
 */
GLOBAL BOOL main __((BYTES ac, TEXT **av));
GLOBAL CODE *buycode __((LEX inst, HEADER *hdr, BYTES size));
GLOBAL HEADER *lookup __((LABEL label));
GLOBAL LABEL crs __((VOID));
GLOBAL TEXT *doenter __((LABEL dblab));
GLOBAL TEXT *msp __((COUNT off));
GLOBAL VOID shorten __((HEADER *tab));
GLOBAL VOID visit __((CODE *qs, COUNT nlbl));

/*	FILE: p2subs.c
 */
GLOBAL BITS bmask __((TYPE ty));
GLOBAL RESOURCE isreg __((EXPR *p));	/* shdb BOOL */
GLOBAL WANTS pref __((WANTS want, WANTS need));
GLOBAL RESOURCE xset __((RESOURCE reg));
GLOBAL RESOURCE xtor __((TINY x));
GLOBAL BOOL iscons __((EXPR *p));
GLOBAL BYTES argbias __(());
GLOBAL COUNT mstoi __((UTINY *s));
GLOBAL EXPR *exzap __((EXPR *));
GLOBAL EXPR *fatgfn __((EXPR *q));
GLOBAL EXPR *mexpr __((BYTES namsize));
GLOBAL HEADER *frehlst __((HEADER *p));
GLOBAL TEXT *bldfmt __((TEXT *s, TEXT *t, BYTES u, TEXT *v));
GLOBAL TEXT funtype __((TEXT *s));
GLOBAL TEXT *lbln __((LABEL label));
GLOBAL TEXT *nn __((BYTES n, BYTES base));
GLOBAL TEXT *putlit __((EXPR *p, COUNT littype, LONG *offset));
GLOBAL TINY rtox __((RESOURCE r));
GLOBAL VOID cpyad __((ADDR *pl, ADDR *pr));
GLOBAL VOID ibmflt __((TEXT *d, BOOL dble));
GLOBAL VOID fatsfn __((EXPR *q, BYTES size));
GLOBAL VOID freqlst __((CODE *q));
GLOBAL VOID panic __((TEXT *s));
GLOBAL VOID perror __((TEXT *s1, TEXT *s2));
GLOBAL VOID putbytes __((TEXT *s, COUNT n));
GLOBAL VOID relabel __((HEADER *));
GLOBAL VOID setad __((EXPR *p, TYPE ty, SPACE pspace, TINY idx, TINY refs));
GLOBAL VOID setlines __((CODE **q));
GLOBAL VOID setop __((EXPR *p, LEX op, TYPE ty, SPACE space, EXPR *l, EXPR *r));
GLOBAL WANTS gotten __((EXPR *p));

/*	FILE: p2tab.c
 */
GLOBAL WANTS twant __((ATTR code));
GLOBAL BOOL opick __((EXPR *p, ATTR attr, RESOUCE got, TYPE ty));
GLOBAL TAB *pick __((EXPR *p, BOOL match));
GLOBAL VOID emit __((HEADER *tab, TEXT *leave, BOOL popds));

/*	FILE: p2dump.c
 */
#ifdef DEBUG
#define	FAIL	(dbprint("/(FAIL)\n", dbprint("}")), 0)
#define	SUCC(x)	(dbprint("}"), (x))
GLOBAL VOID dbexpr __((TEXT *msg, EXPR *q, RESOURCE set));
GLOBAL VOID dbfmt __((TEXT *fmt, ...));
GLOBAL VOID dbprint __((TEXT *FMT, ...));
GLOBAL VOID dbtree __((TEXT *msg, EXPR *q, RESOURCE set));
#else			
#define	FAIL	(0)
#define	SUCC(x)	(x)
			/* disable calls */
#define dbexpr(str, expr, set) 
#define dbfmt(msg)
#define dbprint(msg)
#define dbtree(str, expr, set) 
#endif

