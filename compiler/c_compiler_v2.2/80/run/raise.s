/	raise
/	copyright (c) 1981 by Whitesmiths, Ltd.
/	exception handling functions
/	stack: val, root, -#cx-16, c.r2, c.r3, c.r4, de, pc, ptr, c1, c2, ... cend
/	condition frame pointer points at saved de

	public	__raise
	public	__when
CS := -8
RS := -10
VS := -12

/	VOID _raise(ptr, cond)	raise a condition
__raise:
	sp => af => de => bc <= bc
	a = c | b jz .2
	hl = de <> *sp			/ *sp == 0 => pop return
	a = b + 1 jnz .2 = c + 1 jnz .2
	bc + 1
.2:
	a = d | e jz .3
	a = d + 1 jnz .4 = e + 1 jnz .4
.3:
	hl = root <> de
	a = e | d jz bomb		/ fail if bad frame
.4:
	sp => hl <= bc <= hl	/ stack: !pop, cx
	a = b | c jnz .6
	bc = 0					/ zero => take first exit
retx:
	bc ->^ (hl = VS + de)	/ return from _when
	sp => hl
	a = l | h jnz retkeep
	hl = 4 + de =a^ hl		/ pop frame on return
	a = l | h jz .5
	bc <= sp ->^ hl			/ post back condition
.5:
	bc =^ (hl = VS + de)
	hl =a^ (hl + 1) -> root
	jmp c.rets

climb:
	de =^ (hl = RS + de)	/ climb to next handler
	a = e | d jz bomb		/ fail if there is none
.6:
	hl = 6 + de				/ scan condition list
.7:
	bc =^ hl
	hl + 1
	a = b + 1 jnz .75 = c + 1 jz climb
.75:
	a = b | c jnz .8
	sp => af <= (bc = 0)	/ default return always pops
	jmp .9
.8:
	hl => sp = 4 + sp
	a = c :: *hl jnz .85 = b :: *(hl + 1)
.85:
	hl <= sp
	jnz .7
.9:
	hl + (bc = -6)			/ got a handler
	a = l - e -> c = h -^ d | a <^> -1 -> b = c <^> -1 -> c
	jmp retx

/	internal routine to cauterize handler chain
bomb:
	hl = 0 => sp = bc
	a = l | h jnz .11
	hl = &erptr
.11:
	bc =^ hl => sp
	call _error
.12:
	jmp .12

/	COUNT _when(ptr, c1, c2, ... cend)	handle conditions
__when:
	call c.ents
	hl =a^ (hl = 4 + de)
	a = l | h jz .20
	de ->^ hl				/ set ptr
.20:
	hl = 6 + de				/ scan conditions
.21:
	bc =^ hl
	hl + 1
	a = c | b jz .22
	a = c + 1 jnz .21 = b + 1 jnz .21
.22:
	hl + (bc = 8)
	a = e - l -> l = d -^ h -> h
	hl => sp = root => sp = -1 => sp
retkeep:
	hl = de -> root			/ return not popping
	hl =a^ (hl = CS + de) + de -> sp
	hl = 4 + de
	e = 8
.23:
	b = *(hl - 1)
	c = *(hl - 1)
	bc => sp
	e - 1 jnz .23
	hl = 12 + sp -> de
	bc <= sp
	jmp c.rets

. := .data
root:
	&0
erptr:
	&ermes
ermes:
	"unchecked condition\0"
