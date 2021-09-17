/*	SORT USING QUICKSORT METHOD
 *	copyright (c) 1979 by Whitesmiths, Ltd.
 */
#include <std.h>

/*	internal recursive sort
 */
LOCAL VOID _sort(lv, uv, p)
	COUNT lv, uv;
	FAST struct {
		COUNT (*ordf)();
		VOID (*excf)();
		TEXT *base;
		} *p;
	{
	FAST COUNT i, j;

	while (lv < uv)
		{
		for (i = lv - 1, j = uv; i < j; )
			{
			while (++i < j)
				if (0 < (*p->ordf)(i, uv, &p->base))
					break;
			while (i < --j)
				if ((*p->ordf)(j, uv, &p->base) < 0)
					break;
			if (i < j)
				(*p->excf)(i, j, &p->base);
			}
		if (i != uv)
			(*p->excf)(i, uv, &p->base);
		if (i - lv < uv - i)
			{
			_sort(lv, i - 1, p);
			lv = i + 1;
			}
		else
			{
			_sort(i + 1, uv, p);
			uv = i - 1;
			}
		}
	}

/*	external entry point
 */
VOID sort(n, ordf, excf, base)
	COUNT n;
	COUNT (*ordf)();
	VOID (*excf)();
	TEXT *base;
	{
	_sort(0, n - 1, &ordf);
	}
