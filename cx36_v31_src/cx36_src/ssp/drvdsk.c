/*  DISK DRIVER UNDER IBM SYSTEM/36
 *  copyright (c) 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include <fcntl.h>
#include "ssp.h"
#include "dtfd.h"

/*	move to long (4 bytes) from relative record number (3 bytes)
 */
LOCAL _dmlr (crnp, rrnp)
	UTINY *crnp;
	UTINY *rrnp;
	{
	crnp[0] = 0;			/* msb of l is 0 */
	crnp[1] = rrnp[0];
	crnp[2] = rrnp[1];
	crnp[3] = rrnp[2];
	}

LOCAL _dmrl (rrnp, crn)
	UTINY  *rrnp;	/* relative record number (3 byte integer) pointer */
	LONG crn;		/* clib record number (4 byte integer) */
	{
	rrnp[0] = ((UTINY *)&crn)[1];
	rrnp[1] = ((UTINY *)&crn)[2];
	rrnp[2] = ((UTINY *)&crn)[3];
	}

/*  close a file
 */
STAT _dskclose(p)
	FDS *p;
	{
	DTFD *d = (DTFD *) p->user;

	_svc(SSP_CLOS, d);
	_errno = d->completion;
	_free(d);
	return (_errno == DC_NORMAL ? 0 : NFAIL);
	}

/*  get a record from a file
 */
STAT _dskget(p, recno)
	FDS *p;
	LONG recno;
	{
	DTFD *d = (DTFD *) p->user;

	if (!p->daflag)
		recno = p->rcurr + 1L;
	if (recno >= p->rend)
		return 1;	/* indicate end of file */
	_dmrl(d->rrn, recno);		/* move to rrn from long */
	d->operation = DO_GET_RRN;
	_svc(SSP_DREQ, d);
	if (d->completion != DC_NORMAL)
		{
		_errno = d->completion;
		return (NFAIL);
		}
	p->rcurr = recno;
	p->bnext = p->pbuf;			/* the same as pdtfd->ireca */
	p->bleft = d->reclen;		/* fixed length received */
	return (0);
	}

/*  open a file
 */
STAT _dskopen(p, name, facc)
	FAST FDS *p;
	TEXT *name;
	BYTES facc;
	{
	IMPORT TEXT *_alloc();
	DTFD *d;

	p->user = _alloc(sizeof (DTFD), 0);
	d = (DTFD *) p->user;
	_cpybuf(d, &_dtfd, sizeof (DTFD));
	_filename(name, d->filename);
	if (facc == FAOPEN)
		{
		/*  pseudo open (is used to determine the record length)
		 */
		d->attrib1 = 0xfc;		/* gran/gseq/aeod/arrn/upd/del */
		d->attrib2 = DA2_PSEUDO;
		d->attrib3 = 0x08;
		d->attrib4 = 0;
		d->modifb2 |= DM2_RETRRN;

		_svc(SSP_ALOC, d);
		if (d->completion != DC_NORMAL)
			{
			_errno = d->completion;
			return (NFAIL);
			}
		_svc(SSP_OPEN, d);
		if (d->completion != DC_NORMAL)
			{
			_errno = d->completion;
			return (NFAIL);
			}

		/* The record length is now known - time for a real open */

		d->attrib2 = 0;

		_svc(SSP_OPEN, d);
		if (d->completion != DC_NORMAL)
			{
			_errno = d->completion;
			return (NFAIL);
			}
		
		if (p->oflag & O_BIN)
			p->daflag = YES;
		else
			p->daflag = NO;
		p->rlen = d->reclen;
		d->ireca = d->oreca = p->pbuf = _alloc(p->rlen);

		d->operation = DO_GET_LAST;
		_svc(SSP_DREQ, d);
		switch (d->completion)
			{
		case DC_RECNOTF:
			p->rend = 0;
			break;
		case DC_NORMAL:
			_dmlr(&p->rend, d->rrn);	/* move to long from rrn */
			p->rend += 1L;
			d->operation = DO_SET_BOF;
			_svc(SSP_DREQ, d);
			break;
		default:
			_errno = d->completion;
			return (NFAIL);
			}
		p->fsize = p->rend * p->rlen;
		}
	else
		{
		/*  create file
		 */
		d->attrib1 = 0xfc;
		d->attrib2 = 0x00;
		d->attrib3 = 0x28;	/* create SEQ file - DA works anyhow with GAM */
		d->attrib4 = 0x00;
		d->modifb2 |= DM2_RETRRN;
		if (p->oflag&O_BIN)
			{
			p->daflag = YES;
			d->reclen = (p->rsize == 0 ? 32 : p->rsize);
			}
		else
			{
			p->daflag = NO;
			d->reclen = 96;			/* default reclen for text files */
			}
		p->rlen = d->reclen;
		d->ireca = d->oreca = p->pbuf = _alloc(p->rlen);
		p->fsize = 0;
		_svc(SSP_ALOC, d);
		if (d->completion != DC_NORMAL)
			{
			_errno = d->completion;
			return (NFAIL);
			}
		_svc(SSP_OPEN, d);
		if (d->completion != DC_NORMAL)
			{
			_errno = d->completion;
			return (NFAIL);
			}
		}
	p->bleft = (p->oflag&O_WRONLY ? p->rlen : 0);
	p->bnext = p->pbuf;
		
	return (0); /* open successful */
	}

/*  put a record to a file
 */
STAT _dskput(p, nlflag)
	FDS *p;
	BOOL nlflag;
	{
	COUNT stat;
	DTFD *d = (DTFD *) p->user;

	p->bnext = p->pbuf;		/* d->oreca eq. d->ireca eq. p->pbuf */
 	p->bleft = p->rlen;
	if (!p->daflag)			/* if not direct access */
		p->rcurr++;			/* then increment wanted record number */
	if (p->oflag & O_APPEND)
		p->rcurr = p->rend;
	if (p->rcurr == p->rend)
		{
		d->operation = DO_ADD_EOD;
		_svc(SSP_DREQ, d);
		if (d->completion != DC_NORMAL)
			{
			_errno = d->completion;
			return (NFAIL);
			}
		p->rend++;
		}
	else
		{
		_dmrl(d->rrn, p->rcurr);
		d->operation = DO_RNDUPD;
		_svc(SSP_DREQ, d);
		if (d->completion != DC_NORMAL)
			{
			_errno = d->completion;
			return (NFAIL);
			}
		}
	p->flags &= ~FDIRTY;
	return (0);
	}
