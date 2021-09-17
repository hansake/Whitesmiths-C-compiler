/*	header for dump/restor
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */

/*	constants and defaults
 */
#define NOFILE -1
#define NOTDMPD 0
#define CKSUM 031415
#define TSIZE 20000
#define DATFIL "/adm/dump"
#define IPERBLK (BUFSIZE / sizeof (FINODE))
#define TPADDR (BUFSIZE - 2 * sizeof (UCOUNT))

/*	dump tape header
 */
typedef struct {
	BLOCK d_isize;
	BLOCK d_fsize;
	ULONG d_date;
	ULONG d_epoch;
	UCOUNT d_tsize;
	TEXT d_pad[496];
	UCOUNT d_cksum;
	} DHEAD;

/*	entry in dump log file
 */
typedef struct {
	TEXT dev[DIRSIZE + 6];
	ULONG time;
	} LOGENT;
