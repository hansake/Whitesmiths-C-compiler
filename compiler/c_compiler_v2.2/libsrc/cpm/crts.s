/	chdr
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	startup code for C

	public	start
	public	_cpm
	public	__ltor
	public	__rtol
	public	__setint
	public	__svc
	public	__data

/	get control here at startup time
start:
	hl = *6 -> sp		/ setup stack
	bc = &__memory		/ clear bss area
	hl = &__edata
	a = c - l -> c = b -^ h -> b
.00:
	a = c | b jz .01
	a - a -> *hl; hl + 1; bc - 1
	jmp .00
.01:
	hl = __stop - 1		/ set stack top fence
	a = l | h jnz .02
	hl = &__memory -> __stop
.02:
	hl = 0x80 => sp		/ enter C
	call __main
	sp <= bc
	call _exit

/	convert long to random block number
__ltor:
	sp => hl => bc
	hl <> *sp
	a = *(hl + 1 + 1) -> *bc = *(hl + 1) -> *(bc + 1)
	a = *(hl - 1 - 1 - 1) -> *(bc + 1)
	hl <= sp => sp => sp
	jmp *hl

/	convert random block number to long
__rtol:
	sp => hl => bc
	hl <> *sp
	a = *(hl + 1 + 1) -> *bc - a -> *(bc + 1)
	a = *(hl - 1 - 1) -> *(bc + 1) = *(hl + 1) -> *(bc + 1)
	hl <= sp => sp => sp
	jmp *hl

/ set interrupt handler
__setint:
	ret

/	make system call
__svc:
_cpm:
	bc =^ (hl = 2 + sp)
	de => sp =^ (hl + 1)
	hl =a^ (hl + 1)
	call *5
	sp => de
	a -> c + a -^ a -> b
	ret

.:=.data
__data:
	"\0cpm80 Edition 2.2: Copyright (c) 1979, 1983 by Whitesmiths, Ltd. "
	"all rights reserved\0"
