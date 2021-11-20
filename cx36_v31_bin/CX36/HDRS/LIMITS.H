/*	GENERIC LIMITS HEADER
 *	copyright (c) 1985 by Whitesmiths, Ltd.
 */

#ifndef __LIMITS__
#define __LIMITS__  1

#include <_OS>
#include <_MACH>

/*	portable values
 */
#define CALL_PARMS					31
#define CONDITIONAL_COMPILES_NEST	6
#define DECLARATION_TYPE_MODIFIERS	6
#define EXTERNAL_NAMES				511
#define INCLUDE_FILES_NEST			4
#define INTERNAL_NAME_LENGTH		63
#define INTERNAL_NAMES				1024
#define MACRO_NAMES					1024
#define MACRO_PARMS					31
#define PAREN_NEST					127
#define STATEMENT_NEST				15
#define SWITCH_CASES				255

/*	operating system dependent
 */
#define CASES_IN_EXTERNAL_NAMES		_CASES
#define EXTERNAL_NAME_LENGTH		_EXLEN
#define SOURCE_LINE_LENGTH			_SLLEN

/*	machine dependent
 */
#define CHAR_BIT					_CBIT
#define CHAR_MAX					_CMAX
#define CHAR_MIN					_CMIN
#define SCHAR_MAX					_SCMAX
#define SCHAR_MIN					_SCMIN
#define UCHAR_MAX					_UCMAX
#define SHRT_MAX					_SMAX
#define SHRT_MIN					_SMIN
#define USHRT_MAX					_USMAX
#define INT_MAX						_IMAX
#define INT_MIN						_IMIN
#define UINT_MAX					_UIMAX
#define LONG_MAX					_LMAX
#define LONG_MIN					_LMIN
#define ULONG_MAX					_ULMAX
#define DBL_RADIX					_DRAD
#define DBL_ROUNDS					_DROUND
#define DBL_MAX_EXP					_DMAXEX
#define DBL_MIN_EXP					_DMINEX
#define DBL_DIG						_DDIG
#define FLT_RADIX					_FRAD
#define FLT_ROUNDS					_FROUND
#define FLT_MAX_EXP					_FMAXEX
#define FLT_MIN_EXP					_FMINEX
#define FLT_DIG						_FDIG
#define LDBL_RADIX					_LDRAD
#define LDBL_ROUNDS					_LDROUND
#define LDBL_MAX_EXP				_LDMAXEX
#define LDBL_MIN_EXP				_LDMINEX
#define LDBL_DIG					_LDDIG

#endif
