#	C ENVIRONMENT PROTOTYPE FILE
#	C80CPM -- Unix Cross to 8080
#	Programmable flag options:
#
c:(e)pp		-o (o) -x -i(h)/ (h)/utext.h (i)

1:(e)p1		-o (o) -b0 -n8 -u (i)

2:(e)p2.80	-o (o) (i)

s:(e)as.80	-o (o) (i)

o::(e)linkw	-o (o) -eb__memory -ed__edata -htr -tb0x100 \
		-L(l) -lc.80 (l)crtscpm.80 (i)


