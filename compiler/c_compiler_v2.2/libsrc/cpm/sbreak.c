/*	CP/M SET SYSTEM BREAK
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *sbreak(size)
	COUNT size;
	{
	IMPORT TEXT _memory, *_stop;
	TEXT *obrk, yellow[40];
	INTERN TEXT *_brk {_memory};

	obrk = _brk;
	_brk =+ size;
	if (yellow <= _brk || _brk < &_memory)
		{
		_brk = obrk;
		return (NULL);
		}
	else
		{
		if (_stop)
			_stop = _brk;
		return (obrk);
		}
	}
