/*	BUILD AC AND AV LIST FROM STDIN
 *	copyright (c) 1982 by Whitesmiths, Ltd.
 */
#include <std.h>

#define	NAMSIZE	64

/*	linked list structure
 */
#define LNKLST	struct lnklst
typedef struct lnklst
	{
	struct lnklst *next;
	TEXT *buf;
	};

BOOL getin(pac, pav)
	BYTES *pac;
	TEXT ***pav;
	{
	BOOL eof, success;
	BYTES i;
	COUNT n;
	LNKLST *head, *p, **qb;
	TEXT buf[NAMSIZE], *pbuf, **s;

	eof = NO;
	success = YES;
	*pac = 0;
	head = NULL;
	qb = &head;
	while (!eof)
		{
		for (i = 0, pbuf = &buf; i < NAMSIZE &&
			0 < (n = read(STDIN, pbuf, 1)) && *pbuf != '\n'; ++i, ++pbuf)
			;
		eof = (n <= 0);
		if (!i)
			;
		else if (i == NAMSIZE)
			{
			buf[NAMSIZE - 1] = '\0';
			success = remark("argument too long: ", buf);
			for (pbuf = &buf; 0 < (n = read(STDIN, pbuf, 1)) &&
				*pbuf != '\n'; )
				;
			eof = (n <= 0);
			}
		else
			{
			p = alloc(sizeof(LNKLST), NULL);
			*qb = p;
			qb = &p->next;
			buf[i] = '\0';
			p->buf = buybuf(buf, i + 1);
			++*pac;
			}
		}
	s = alloc(sizeof(**pav) * (*pac + 1), NULL);
	*pav = s;
	for (p = head; p; p = free(p, p->next))
		*s++ = p->buf;
	*s = NULL;
	return (success);
	}
