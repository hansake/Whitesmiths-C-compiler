/*	FREE A LINKED LIST
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

struct list *frelst(p, plast)
	FAST struct {
		TEXT *next;
		} *p, *plast;
	{
	while (p && p != plast)
		p = free(p, p->next);
	return (p);
	}
