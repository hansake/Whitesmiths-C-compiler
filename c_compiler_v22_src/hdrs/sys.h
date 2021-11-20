/*	THE SYSTEM HEADER
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */

/*	the system error codes
 */
#define EPERM	1
#define ENOENT	2
#define ESRCH	3
#define EINTR	4
#define EIO		5
#define ENXIO	6
#define E2BIG	7
#define ENOEXEC	8
#define EBADF	9
#define ECHILD	10
#define EAGAIN	11
#define ENOMEM	12
#define EACCES	13
#define EFAULT	14
#define ENOTBLK	15
#define EBUSY	16
#define EEXIST	17
#define EXDEV	18
#define ENODEV	19
#define ENOTDIR	20
#define EISDIR	21
#define EINVAL	22
#define ENFILE	23
#define EMFILE	24
#define ENOTTY	25
#define ETXTBSY	26
#define EFBIG	27
#define ENOSPC	28
#define ESPIPE	29
#define EROFS	30
#define EMLINK	31
#define EPIPE	32
#define EDOM	33
#define ERANGE	34

/*	the signals
 */
#define NOSIG	1
#define SIGHUP	1
#define SIGINT	2
#define SIGQUIT	3
#define SIGILIN	4
#define SIGTRC	5
#define SIGRNG	6
#define SIGDOM	7
#define SIGFPT	8
#define SIGKILL	9
#define SIGBUS	10
#define SIGSEG	11
#define SIGSYS	12
#define SIGPIPE	13
#define SIGALRM	14
#define SIGTERM	15
#define SIGSVC	16
#define SIGUSR	17
#define NSIG	18

/*	i/o parameters
 */
#define DIRSIZE	14
#define NAMSIZE	64

/*	codes for t_mode
 */
#define M_HUP	0001
#define M_XTABS	0002
#define M_LCASE	0004
#define M_ECHO	0010
#define M_CRMOD	0020
#define M_RAW	0040
#define M_ODDP	0100
#define M_EVENP	0200
#define M_NL1	000400
#define M_NL2	001000
#define M_NL3	001400
#define M_HT1	002000
#define M_HT2	004000
#define M_HT3	006000
#define M_CR1	010000
#define M_CR2	020000
#define M_CR3	030000
#define M_FF1	040000
#define M_BS1	0100000

/*	codes for xecl/xecv flags
 */
#define X_MODE	03
#define X_SYN	00
#define X_ASYN	01
#define X_OVLAY	02
#define X_SYSIG	04
#define X_RUID	010
#define X_EUID	020

/*	macros
 */
#define isblk(mod)	(((mod) & 060000) == 060000)
#define ischr(mod)	(((mod) & 060000) == 020000)
#define isdir(mod)	(((mod) & 060000) == 040000)

/*	error returns
 */
typedef COUNT ERROR;

/*	directory structure
 */
typedef struct {
	UCOUNT d_ino;
	TEXT d_name[DIRSIZE];
	} DIR;

/*	stat and fstat structure
 */
typedef struct {
	UCOUNT s_dev;
	UCOUNT s_ino;
	BITS s_mode;
	UTINY s_link;
	UTINY s_uid, s_gid;
	UTINY s_size0;
	UCOUNT s_size1;
	UCOUNT s_addr[8];
	LONG s_actime, s_uptime;
	} STAT;

/*	stty and gtty structure
 */
typedef struct {
	TEXT t_ispeed, t_ospeed;
	TEXT t_erase, t_kill;
	BITS t_mode;
	} TTY;
