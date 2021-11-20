/*  SEARCHF.C - CP/M Search file function
 *  Usig BDOS Search and Search Next to expand wildcards
 *  Written for  Whitesmiths C compiler version 2.2
 *  You are free to use, modify, and redistribute
 *  this source code. No warranties given.
 *  Hastily Cobbled Together 2021 by Hans-Ake Lund
 */
#include <std.h>
#include <cpm.h>

#define NODEV	127

GLOBAL BOOL dflag;

/* CP/M BDOS FCB structure */
#define BDOSFCB	struct bdosfcb
struct bdosfcb {
    TEXT dr;         /* drive code */
    TEXT f[8];       /* file name, blank padded */
    TEXT t[3];       /* file type, blank padded */
    TEXT ex;         /* current extent number */
    TEXT s[2];       /* reserved for internal system use */
    TEXT rc;         /* record count for extent ex */
    TEXT d[16];      /* reserved for internal system use */
    TEXT r[3];       /* random record number */
    };

/* CP/M directory entry structure */
#define DIRENT struct dirent
struct dirent {
    TEXT st;         /* status */
    TEXT f[8];       /* file name */
    TEXT e[3];       /* file extension (file type) */
    TEXT xi;         /* extension */
    TEXT bc;         /* length of the data used by this extent */
    TEXT xh;         /* extension */
    TEXT rc;         /* length of the data used by this extent */
    TEXT ai[16];     /* block pointers */
    };

LOCAL BDOSFCB srchfcb;
LOCAL TEXT diskbuf[128] {0};
LOCAL TEXT fnmbuf[128 * 15] {0};
LOCAL TINY curdev {NODEV};

/* Disk letter to unit # translation */
#define DEV struct devs
LOCAL DEV {
    TEXT *nm;
    TINY devno;
    } _dev[] {
    "A", 0,
    "B", 1,
    "C", 2,
    "D", 3,
    "E", 4,
    "F", 5,
    "G", 6,
    "H", 7,
    "I", 8,
    "J", 9,
    "K", 10,
    "L", 11,
    "M", 12,
    "N", 13,
    "O", 14,
    "P", 15,
    0};

/*  Get internal device name
 */
LOCAL TINY _getdev(s)
    FAST TEXT *s;
    {
    IMPORT DEV _dev[];
    FAST TEXT *q, *r;
    DEV *p;

    for (p = _dev; p->nm; ++p)
        {
        for (r = s, q = p->nm; *r == *q && *q; ++r, ++q)
            ;
        if (*r == *q)
            return (p->devno);
        }
    return (NODEV);
    }

/*  Get a CP/M filename
 *    Put the filename in an FCB structure,
 *    expanding '*' to '?'s for rest of filename
 *    and extension to support wildcards
 *    returns NO if ok, else error
 */
LOCAL BOOL _getname(pf, s)
    FAST BDOSFCB *pf;
    FAST TEXT *s;
    {
    FAST COUNT i;
    BOOL wild;

    wild = NO;
    for (i = 0; *s != '.' && *s; ++s)
        {
        if (i < 8)
            {
            if (*s == '*')
                {
                wild = YES;
                pf->f[i++] = '?';
                }
            else
                pf->f[i++] = toupper(*s);
            }
        }
    while (i < 8)
        {
        if (wild)
            pf->f[i++] = '?';
        else
            pf->f[i++] = ' ';
        }
    if (*s == '.')
        ++s;
    else if (*s)
        return (YES);
    wild = NO;
    for (i = 0; *s && i < 3; ++s)
        {
        if (*s == '*')
            {
            wild = YES;
            pf->t[i++] = '?';
            }
        else
            pf->t[i++] = toupper(*s);
        }
    while (i < 3)
        {
        if (wild)
            pf->t[i++] = '?';
        else
            pf->t[i++] = ' ';
        }
    return (*s);
    }

/*  Format filename from directory entry
 *  returns length of filename string including NUL termination
 */
LOCAL COUNT ffnm(ifnm, disk, entno)
    TEXT *ifnm;     /* Pointer to area where to put the filename */
    COUNT disk;     /* Disk # of the file */
    BYTES entno;    /* Directory entry # for the file */
    {
    IMPORT TEXT diskbuf[128];
    DIRENT *entptr;
    COUNT fidx, eidx;
    COUNT fnmlen;
    TEXT *fnm;

    fnm = ifnm;
    *fnm++ = 'A' + disk;
    *fnm++ = ':';
    fnmlen = 2;
    entptr = &diskbuf[entno * 32];
    if (entptr->f[0] == ' ') /* blank filenames not allowed */
        return(0);
    for (fidx = 0; fidx < 8; fidx++)
        {
        if (entptr->f[fidx] != ' ')
            {
            *fnm++ = 0x7f & entptr->f[fidx];
            fnmlen++;
            }
        else
            break;
        }
    if (entptr->e[0] != ' ')
        {
        *fnm++ = '.';
        fnmlen++;
        }
    for (eidx = 0; eidx < 3; eidx++)
        {
        if (entptr->e[eidx] != ' ')
            {
            *fnm++ = 0x7f & entptr->e[eidx];
            fnmlen++;
            }
        else
            break;
        }
    *fnm = '\0';
    fnmlen++;

    return (fnmlen);
    }

/*  Search matching filenames
 *  returns number of filenames that matched
 */
COUNT searchf(s, sav, maxfnam)
    FAST TEXT *s;    /* filename which may include wildcards */
    TEXT **sav;      /* pointer to array of pointers to matching filenames */
    COUNT maxfnam;   /* max number of elements in the file name array */
    {
    IMPORT TINY curdev;
    IMPORT BDOSFCB srchfcb;
    IMPORT TEXT diskbuf[128];
    FAST BDOSFCB *pf = &srchfcb;
    FAST COUNT i;
    TINY dev;
    TEXT dbuf[5], *t;
    COUNT cpmres;
    COUNT fnames;
    TEXT *pfnm;
    COUNT ffnmlen;

    if (curdev == NODEV)
        curdev = cpm(CIDRNO, 0);
    for (t = s, i = 0; *s != ':' && *s; ++s)
        if (i < 4)
            dbuf[i++] = toupper(*s);
    dbuf[i] = '\0';
    if (*s != ':')
        {
        s = t;
        dev = curdev;
        }
    else if ((dev = _getdev(dbuf)) == NODEV)
        {
        putfmt("_getdev() returned FAIL\n");
        return (0);
        }
    else
        ++s;
    _clrbuf(pf, sizeof (*pf));
    if (_getname(pf, s))
        {
        putfmt("_getname() returned FAIL\n");
        return (FAIL);
        }
    cpm(CLOGIN, dev);
    cpm(CSETAD, &diskbuf);
        pfnm = fnmbuf;
    if ((cpmres = cpm(CSRCH, pf)) < 0)
        {
        return (fnames);
        }
    fnames = 0;
    if (0 < (ffnmlen = ffnm(pfnm, dev, cpmres)))
        {
        sav[fnames++] = pfnm;
        pfnm += ffnmlen;
        }
    while (fnames < maxfnam)
        {
        if ((cpmres = cpm(CSRCHN, pf)) < 0)
            {
            return (fnames);
            }
        if (0 < (ffnmlen = ffnm(pfnm, dev, cpmres)))
            {
            sav[fnames++] = pfnm;
            pfnm += ffnmlen;
            }
        }
    return (fnames);
    }
