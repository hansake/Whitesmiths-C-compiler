/*  CLESS.C - a "more" like program for CP/M (less is more).
 *  Written for Whitesmiths C compiler version 2.2.
 *  You are free to use, modify, and redistribute
 *  this source code. The software is provided "as is",
 *  without warranty of any kind.
 *  Hastily Cobbled Together 2021 by Hans-Ake Lund.
 */
#include <std.h>
#include <cpm.h>

COUNT searchf(FAST TEXT *s, TEXT **sav, COUNT maxfnam);

/* flags:
 *  -d      debug flag
 *  -f      print filename before printing file content
 *  -n#     print n lines before asking to continue (23 default)
 */
GLOBAL BOOL dflag = NO;
LOCAL BOOL fflag = NO;
LOCAL COUNT nflag = 23;
LOCAL TEXT **pfnms {0};
LOCAL TEXT *fnms[128] {0};

GLOBAL TEXT *_pname {"cless"};

/*    Get character from keyboard without echo
 *    using CPM/BDOS call.
 */
LOCAL COUNT kbchin()
    {
    COUNT kbchar = 0;

    while (kbchar == 0)
        {
        kbchar = cpm(CDCIO, 0xff, 0);
        }
    return (kbchar);
    }

/*    Print files to STDOUT, default 23 lines at a time
 */
BOOL main(ac, av)
    BYTES ac;
    TEXT **av;
    {
    IMPORT FIO stdin, stdout;
    BOOL okgo;
    COUNT cmdargs;
    COUNT nchars;
    COUNT lines;
    FILE fd;
    FIO inf;
    TEXT txtbuf[BUFSIZE];
    TEXT kbdchar;
    COUNT nfiles;

    getflags(&ac, &av, "d,f,n#:F <args>", &dflag, &fflag, &nflag);

    if (dflag)
        {
        putfmt("dflag = %p, fflag = %p, nflag = %i\n",
             dflag?"YES":"NO",
             fflag?"YES":"NO",
             nflag);
        putfmt("Filenames on command line:\n");
        for (cmdargs = 0; cmdargs < ac; cmdargs++)
            {
            putfmt("  %p\n", av[cmdargs]);
            }
        }

    if (fflag && nflag == 23)
        nflag = 22;
    for (cmdargs = 0; cmdargs < ac; cmdargs++)
        {
        nfiles = searchf(av[cmdargs], fnms, 128);
        pfnms = fnms;
        okgo = YES;
        while (okgo)
            {
          nxtfile:
            fd = getfiles(&nfiles, &pfnms, STDIN, STDERR);
            if (fd < 0)
                break;
            if (fd == STDERR)
                {
                remark("can't read: ", pfnms[-1]);
                okgo = NO;
                }
            else
                {
                if (fflag)
                    putstr(STDOUT, pfnms[-1], ":\n", NULL);
                finit(&inf, fd, READ);
                lines = 1;
                while (nchars = getl(&inf, txtbuf, BUFSIZE))
                    {
                    if (lines < nflag)
                        {
                        putlin(txtbuf, nchars);
                        lines++;
                        }
                    else
                        {
                        putlin(txtbuf, nchars);
                        lines = 1;
                      getkbch:
                        putstr(STDOUT, "-- more --", NULL);
                        kbdchar = kbchin();
                        putstr(STDOUT, "\b\b\b\b\b\b\b\b\b\b", NULL);
                        putstr(STDOUT, "          \r", NULL);
                        if (dflag)
                            {
                            putf(&stdout, "kbdchin() = 0x%hi\n", kbdchar);
                            }
                        switch (kbdchar)
                            {
                        case 0x03: /* Ctrl-C */
                            putstr(STDOUT, "^C\n", NULL);
                            fclose(&inf);
                            return(NO);
                        case 'n':
                        case 'N':
                            fclose(&inf);
                            goto nxtfile;
                        case 'q':
                        case 'Q':
                            fclose(&inf);
                            return(YES);
                        case ' ':
                            break;
                        case '\r':
                            lines = nflag;
                            break;
                        default:
                            goto getkbch;
                            }
                        }
                    }
                fclose(&inf);
                }
            }
        }
    return (YES);
    }
