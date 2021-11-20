/*	WRAPPER REDEFINITIONS FOR EXTENDED ANSI
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */
#ifndef __ANSXWR__
#define __ANSXWR__	1	

#ifndef __WSLWR__
#ifndef __ANSIWR__
#define onexit	_onexit
#define remove	_remove
#endif
#endif

#ifndef __WSLWR__
#define __WSLWR__	1
#define alloc	_alloc
#define btod	_btod
#define btoi	_btoi
#define btol	_btol
#define btos	_btos
#define close	_close
#define cmpstr	_cmpstr
#define cpybuf	_cpybuf
#define dtento	_dtento
#define dtoe	_dtoe
#define dtof	_dtof
#define error	_error
#define itob	_itob
#define lenstr	_lenstr
#define lseek	_lseek
#define ltob	_ltob
#define onintr	_onintr
#define prefix	_prefix
#define rawmode	_rawmode
#define read	_read
#define remark	_remark
#define sbreak	_sbreak
#define scnbuf	_scnbuf
#define scnstr	_scnstr
#define stob	_stob
#define write	_write
#endif

#ifndef __ANSIWR__
#define __ANSIWR__	1	
#define atan2	_atan2
#define clock	_time
#define exit	_terminate
#define getenv	_getenv
#define realloc	_realloc
#define rename	_rename
#define system	_system
#endif

#endif
