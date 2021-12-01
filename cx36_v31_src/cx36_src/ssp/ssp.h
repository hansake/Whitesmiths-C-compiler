/*  SYSTEM INTERFACE MAIN HEADER UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  modified 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */

typedef int STAT;

#define _DEFNAM	"DSK"		/* default driver name */
#define _NAMSIZ 8			/* maximum name length of files and drivers */

typedef struct              /* DRVR - Driver structure */
    {
    TEXT    *drname;        /* Pointer to driver-name */
    BITS    drflags;        /* Flags */
    BYTES   option;         /* Options */
    STAT    (*open)();      /* Open the file */
    STAT    (*close)();     /* Close the file */
    STAT    (*getrec)();    /* Get a record */
    STAT    (*putrec)();    /* Put a record */
    STAT    (*remove)();    /* Remove a file */
    } DRVR;

#define NFILES 16			/* # of open files */

typedef struct              /* FDS - File Descriptor Structure */
    {
    BITS    flags;          /* Flags */
    BOOL    rsize;          /* Text or binary file */
	BITS	oflag;			/* Open flags */
    TEXT    *bnext;         /* Pointer to data to read or where to write */
    BYTES   bleft;          /* Number of bytes left in record */
    TEXT    *pbuf;          /* Pointer to allocated buffer (PUT) */
    DRVR    *drvr;          /* Pointer to I/O driver structure */
    LONG    rcurr;          /* Current recno */
    LONG    lseek;          /* Where to read or write (DA) */
    LONG    rend;           /* Last record number (DA) */
    COUNT   rlen;           /* Record length (DA) */
    UTINY   ctl;            /* Control of printer */
    COUNT   nopen;          /* Number of files open on this FDS */
    TEXT    *user;          /* May be used by drivers */
    LONG    fsize;          /* Size of file */
    BOOL    daflag;         /* Direct access flag */
    } FDS;

/*  ssp codes for supervisor calls
 */
#define SSP_ALOC	1,2,0x4,0x1,0xc		/* allocate file or device */
#define SSP_OPEN	1,2,0x4,0x1,0x2		/* open file or device */
#define SSP_CLOS	1,2,0x4,0x1,0x3		/* close file or device */
#define SSP_SYSLOG	1,2,0x4,0x1,0x5		/* put message to system log */
#define SSP_SYSLST	1,2,0x4,0x1,0x6		/* put message to system list */
#define SSP_SYSIN	1,2,0x4,0x1,0x7		/* get message from system input */
#define SSP_DREQ	1,2,0xc,0x1,0x0		/* disk data management request */
#define SSP_WREQ	1,2,0xc,0x1,0x4		/* workstation request */
#define SSP_PREQ	1,2,0xc,0x1,0x8		/* printer request */
#define SSP_EOJ		1,0,0x4,0x1,0x4		/* end of job */

/*  file descriptor flags -- see FDS above
 */
#define FOPENR		0x01	/* File is open for read */
#define FOPENW		0x02	/* File is open for write */
#define FMRD		0x04	/* Time to read a delayed read */
#define FDIRTY		0x08	/* Buffer must be cleaned by writing */
#define FEOF		0x20	/* EOF has been reached */

/*  driver flags -- see DRVR above
 */
#define DDA			0x01	/* Direct-access driver */
#define DBLANKS		0x02	/* Keep trailing blanks */
#define DIMMED		0x04	/* Immediate output (don't wait for '\n') */
#define DPERMEOF	0x08	/* Once EOF, always EOF, at least until closed */

/*
 * intermodule communication
 */
#define NFAIL		-1		/* general error return value */
#define FAOPEN      0       /* file should exist */
#define FACREATE    1       /* file is to be created (if possible) */

/*  error codes
 */
#define EBADFD		1	/* Bad file-descriptor */
#define EBADNM		2	/* Bad filename */
#define EOPEN		3	/* Open error */
#define ENODRVR		4	/* No such driver */
#define EBADMODE	5	/* Bad mode */
#define EMFILE		6	/* Too many open files */
#define ESEEK		7	/* Bad call to lseek */
#define EOEXCL		8	/* Open exclusive not supported */
#define ENOTSUP		9	/* Function not supported */
#define ENOMEM		10	/* No memory */

/*  printer control codes -- see FDS above
 */
#define CTLNEWL     0       /* write next record on a new line */
#define CTLNEWP     1       /* force new page before next record */
#define CTLSAMEL    2       /* write next record on the same line */
#define CTLNONL     3       /* continue on the same line (hard) */

/*
 *  preprocessor functions
 */
#define CHECKFD(FD)	(FD >= 0 && FD < NFILES ? _fdsps[FD] : NULL)

/*
 *  global variables heavily used
 */
GLOBAL ARGINT _errno;
