/	zret
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	exit from runtime functions, preserving flags
/	stack: bc, hl, pc, R, L

	public	c.zret
	public	c.lret
c.zret:
	af => sp
	bc =^ (hl = 6 + sp) ->^ (hl + 1 + 1 + 1) <= sp
	c -> *(hl - 1 - 1 - 1)
	sp => bc => hl => af => af
	ret

c.lret:
	bc =^ (hl = 4 + sp) ->^ (hl + 1)
	sp => bc => hl => af
	ret
