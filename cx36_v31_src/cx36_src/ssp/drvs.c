/*  TABLE OF I/O-DRIVERS UNDER IBM SYSTEM/36
 *  copyright (c) 1983, 1984, 1985, 1986 by Whitesmiths, Ltd.
 *  written 1986 by Tore Fahlstrom for Unisoft AB of Sweden.
 */
#include <wslxa.h>
#include "ssp.h"

#define NOT_INSTALLED 0
#define INSTALLED 1

#define DISK	INSTALLED
#define PRINTER	INSTALLED
#define SYSIN	INSTALLED
#define SYSLIST	INSTALLED
#define SYSLOG	INSTALLED

IMPORT STAT _nulopen(), _nulclose(), _nulget(), _nulput();
#if DISK
IMPORT STAT _dskopen(), _dskclose(), _dskget(), _dskput();
#endif
#if SYSLOG
IMPORT STAT _logopen(), _logclose(), _logput();
#endif
#if PRINTER
IMPORT STAT _prtopen(), _prtclose(), _prtput();
#endif
#if SYSIN
IMPORT STAT _sinopen(), _sinclose(), _singet();
#endif
#if SYSLIST
IMPORT STAT _sliopen(), _sliclose(), _sliput();
#endif

STAT _nulldrvr(), _nodrvr();

RDONLY DRVR _drvs[]
	{
	"NULL", 0, 0,
		_nulopen,	_nulclose,	_nulget,	_nulput,	_nodrvr,
#if DISK
	"DSK", 0, 0,
		_dskopen,	_dskclose,	_dskget,	_dskput,	_nodrvr,
	"DSKB", DBLANKS, 0,
		_dskopen,	_dskclose,	_dskget,	_dskput,	_nodrvr,
#endif
#if PRINTER
	"PRT", 0, 0,
		_prtopen,	_prtclose,	_nodrvr,	_prtput,	_nodrvr,
#endif
#if SYSLOG
	"SYSLOG", 0, 0,
		_logopen,	_logclose,	_nodrvr,	_logput,	_nodrvr,
#endif
#if SYSIN
	"SYSIN", 0, 0,
		_sinopen,	_sinclose,	_singet,	_nodrvr,	_nodrvr,
#endif
#if SYSLIST
	"SYSLIST", 0, 0,
		_sliopen,	_sliclose,	_nodrvr,	_sliput,	_nodrvr,
#endif
	NULL
	};

STAT _nulldrvr()
	{
	return (0);
	}

STAT _nodrvr()
	{
	_errno = ENODRVR;
	return (NFAIL);
	}
