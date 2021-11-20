/*	SORT (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

LOCAL ARGINT (*mycompar)() {NULL};
LOCAL BYTES mywidth {0};

LOCAL VOID excf(i, j, pbase)
	BYTES i, j;
	TEXT **pbase;
	{
	BYTES k;
	TEXT t, *p1, *p2;

	p1 = (*pbase) + i * mywidth;
	p2 = (*pbase) + j * mywidth;
	for (k = 0; k < mywidth; ++k, ++p1, ++p2)
		t = *p1, *p1 = *p2, *p2 = t;
	}

LOCAL ARGINT ordf(i, j, pbase)
	BYTES i, j;
	TEXT **pbase;
	{
	BYTES k;
	TEXT t, *p1, *p2;

	p1 = (*pbase) + i * mywidth;
	p2 = (*pbase) + j * mywidth;
	return ((*mycompar)(p1, p2));
	}

VOID qsort(base, nel, width, compar)
	TEXT *base;
	BYTES nel, width;
	COUNT (*compar)();
	{
	mywidth = width;
	mycompar = compar;
	sort(nel, ordf, excf, base);
	}

#ifdef TRYMAIN
LOCAL TEXT test[4][6] = {"abcd", "abc", "xyz", "a"};
main()
	{
	IMPORT COUNT strcmp();
	COUNT i;

	qsort(test, 4, 6, strcmp);
	for (i = 0; i < 4; ++i)
		putfmt("%p\n", test[i]);
	exit(1);
	}
#endif
