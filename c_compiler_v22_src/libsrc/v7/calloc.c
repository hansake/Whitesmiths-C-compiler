/*	ALLOCATE SPACE FOR nelem ELEMENTS OF SIZE elsize (V7 STYLE)
 *	Copyright (c) 1983 by Whitesmiths, Ltd.
 */
#include <std.h>

TEXT *calloc(nelem, elsize)
	BYTES nelem, elsize;
	{
	BYTES block;
	TEXT *ptr;

	if (2 < elsize)
		{
		elsize += sizeof (int) - 1;
		elsize -= elsize % sizeof (int);
		}
	block = nelem * elsize;
	ptr = nalloc(block);
	if (ptr != NULL)
		fill(ptr, block, '\0');
	return (ptr);
	}

#ifdef TRYMAIN
main()
	{
	TEXT *p, *p2;

	p = calloc(2, 3);
	p2 = calloc(2, 3);
	putfmt("p=%h ->%i %i %i %i %i %i %i %i %i %i\n",
		p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
	putfmt("p2=%h ->%i %i %i %i %i %i %i %i %i %i\n",
		p2, p2[0], p2[1], p2[2], p2[3], p2[4], p2[5], p2[6], p2[7], p2[8], p2[9]);
	}
#endif
