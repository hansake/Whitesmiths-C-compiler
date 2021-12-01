/*  PRINTER DRIVER UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"
#include "dtfp.h"

#define LINESIZE 132

/*	init buffer variables
 */
LOCAL VOID pfdsinit(p)
	FAST FDS *p;
	{
	p->bleft = LINESIZE;
	p->bnext = p->pbuf;
	p->flags &= ~FDIRTY;
	}

/*	initialize the dtfp struct
 */
LOCAL VOID pdtfinit(prt)
	FAST DTFP *prt;
	{
	prt->device	= 0xe0;
	prt->res0	= 0x80;
	prt->res1	= 0x80;
	prt->fchaina= (TEXT *) 0xffff;
	prt->res2	= 0x80;
	/* prt->lreca is initialized in _prtopen */
	prt->operation = PO_PRINT;
	prt->attrib4= 0x80;
	prt->reclen	= LINESIZE;
	/* prt->filename is initialized in _prtopen */
	prt->bspace	= 1;
	}

/*	open the printer device
 */
STAT _prtopen(p, name, facc)
	FAST FDS *p;
	TEXT *name;
	BYTES facc;
	{
	IMPORT TEXT *_alloc();

	DTFP *prt;

	if (!(p->oflag & O_WRONLY))
		{
		_errno = EBADMODE;
		return (NFAIL);
		}
	p->user = _alloc(sizeof (DTFP), 0);
	_fill(p->user, sizeof (DTFP), '\0');
	prt = (DTFP *) p->user;
	prt->lreca = p->pbuf = _alloc(LINESIZE, NULL);
	_filename((*name ? name : "PRTFILE"), prt->filename);
	pfdsinit(p);
	pdtfinit(prt);
	_svc(SSP_ALOC, prt);
	_svc(SSP_OPEN, prt);
	if (prt->completion != PC_NORMAL)
		{
		_errno = prt->completion;
		return (NFAIL);
		}
	return (0);
	}

/*	put a record on the printer
 */
STAT _prtput(p, add)
	FAST FDS *p;
	BOOL add;
	{
	FAST DTFP *prt = (DTFP *) p->user;
	/*
	 *  setup skip-to-line and space-lines
	 */
	prt->bskip = 0;
	prt->bspace = 0;
	if (p->ctl == CTLNEWP)
		prt->bskip = 1;		/* new page */
	else if (p->ctl == CTLNEWL || p->ctl == CTLNONL)
		prt->bspace = 1;	/* new line */

	prt->operation = PO_PRINT;
	_svc(SSP_PREQ, prt);
	pfdsinit(p);
	if ((prt->completion != PC_NORMAL) && (prt->completion != PC_OVERFLOW))
		{
		_errno = prt->completion;
		return (NFAIL);
		}
	return (0);
	}

/*	close the printer device
 */
STAT _prtclose(p)
	FAST FDS *p;
	{
	FAST DTFP *prt = (DTFP *) p->user;
	UTINY completion;

	_svc(SSP_CLOS, prt);
	completion = prt->completion;

	_free(p->pbuf);
	_free(p->user);

	if (completion != PC_NORMAL)
		{
		_errno = completion;
		return (NFAIL);
		}
	return (0);
	}
