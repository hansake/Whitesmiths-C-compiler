/*	WORKSTATION I/O ROUTINES FOR IBM SYSTEM/36
 *	copyright (c) 1986 by Whitesmiths, Ltd.
 *	written 1986 by Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"
#include "dtfw.h"

STAT wsopen(dtfw, member)
	DTFW *dtfw;
	TEXT *member;
	{
	_filename(member, dtfw->fmtnm);
	_svc(SSP_ALOC, dtfw);
	_svc(SSP_OPEN, dtfw);
	return(dtfw->completion);
	}

STAT wswrite(dtfw, rcad, outlen, format)
	DTFW *dtfw;
	TEXT *rcad;
	COUNT outlen;
	TEXT *format;
	{
	dtfw->wsopc = WOP_PUT;
	dtfw->reca = rcad;
	dtfw->outl = outlen;
	_filename(format, dtfw->fmtnm);
	_svc(SSP_WREQ, dtfw);
	return(dtfw->rtc);
	}

STAT wsread(dtfw, rcad, inlen)
	DTFW *dtfw;
	TEXT *rcad;
	COUNT inlen;
	{
	dtfw->wsopc = WOP_GET;
	dtfw->reca = rcad;
	dtfw->inl = inlen;
	_svc(SSP_WREQ, dtfw);
	return(dtfw->rtc);
	}

STAT wsclose(dtfw)
	DTFW *dtfw;
	{
	_svc(SSP_CLOS, dtfw);
	return(dtfw->completion);
	}

VOID *wsalign(buf)
	VOID *buf;
	{
	return((VOID *)((BYTES)buf + 7 & 0xfff8));
	}
