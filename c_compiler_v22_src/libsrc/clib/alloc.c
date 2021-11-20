/*	HEAP MANAGEMENT FUNCTIONS
 *	copyright (c) 1978 by Whitesmiths, Ltd.
 */
#include <std.h>

#define BLKSIZE	512
#define HEAP	struct heap
#define MAXCORE 512
#define SIZHDR	((BYTES)&(0->x))
#define SIZPTR	(sizeof (TEXT *))

struct heap
	{
	BYTES hsize;
	union {
		DOUBLE d;
		HEAP *next;
		} x;
/*	TEXT junk[hsize];	*/
	};

LOCAL BYTES _hunk {0};
LOCAL HEAP *_heap {NULL};
LOCAL TEXT *_hbot {NULL};
LOCAL TEXT *_htop {NULL};

/*	break off a cell
 */
LOCAL BYTES *_breakup(p, size)
	FAST HEAP *p;
	FAST BYTES size;
	{
	FAST HEAP *q = (TEXT *)p + size;

	q->hsize = p->hsize - size;
	q->x.next = p->x.next;
	p->hsize = size - (SIZHDR+SIZPTR);
	return (q);
	}

/*	merge a cell with the next if contiguous
 */
LOCAL VOID *_hmerge(p)
	FAST HEAP *p;
	{
	FAST HEAP *q = p->x.next;

	if (((TEXT *)p + p->hsize + (SIZHDR+SIZPTR)) == q)
		{
		p->hsize =+ q->hsize + (SIZHDR+SIZPTR);
		p->x.next = q->x.next;
		}
	}

/*	allocate, failing hard
 */
BYTES *alloc(need, link)
	BYTES need, *link;
	{
	IMPORT TEXT *_memerr;
	FAST BYTES *p;

	if (p = nalloc(need, link))
		return (p);
	_raise(NULL, &_memerr);
	}

/*	allocate need bytes, set link
 */
BYTES *nalloc(need, link)
	FAST BYTES need;
	BYTES *link;
	{
	IMPORT BYTES _hunk;
	IMPORT HEAP *_heap;
	IMPORT TEXT *_hbot, *_htop;
	FAST HEAP **qb, *q, *s;
	BYTES n;

	need = ((max(need, SIZPTR) + (SIZHDR-1)) & ~(SIZHDR-1)) + SIZHDR;
	FOREVER
		{
		for (qb = &_heap; q = *qb; qb = &q->x.next)
			if (need <= q->hsize + (SIZHDR+SIZPTR))
				{
				if (q->hsize < need)
					*qb = q->x.next;
				else
					*qb = _breakup(q, need);
				q->x.next = link;
				return (&q->x.next);
				}
		if (!_hunk)
			_hunk = MAXCORE;
		else if (_hunk != MAXCORE)
			_hunk =>> 1;
		for (s = NULL; need <= _hunk && !(s = sbreak(n = _hunk)); )
			_hunk =>> 1;
		if (!s)
			s = sbreak(n = need);
		if (!s)
			return (NULL);
		else
			{
			s->hsize = n - (SIZHDR+SIZPTR);
			if (!_hbot || (TEXT *)s < _hbot)
				_hbot = s;
			if (_htop < (TEXT *)s + n)
				_htop = (TEXT *)s + n;
			free(&s->x.next, NULL);
			}
		}
	}

/*	add this buffer to the free list
 */
TEXT *free(addr, link)
	TEXT *addr, *link;
	{
	IMPORT HEAP *_heap;
	IMPORT TEXT *_hbot, *_htop, *_memerr;
	FAST HEAP *p = addr - SIZHDR;
	FAST HEAP *o, *q;

	if (!addr)
		;
	else if (p < _hbot || _htop <= p)
		{
		_memerr = "bad free call";
		_raise(NULL, &_memerr);
		}
	else if (!_heap)
		{
		p->x.next = NULL;
		_heap = p;
		}
	else if (p < _heap)
		{
		p->x.next = _heap;
		_heap = p;
		_hmerge(p);
		}
	else
		{
		for (o = _heap; (q = o->x.next) && q < p; o = q)
			;
		if (q)
			{
			p->x.next = q;
			o->x.next = p;
			_hmerge(p);
			_hmerge(o);
			}
		else
			{
			p->x.next = NULL;
			o->x.next = p;
			_hmerge(o);
			}
		}
	return (link);
	}
