.processor s36

label1:
	alc label1(2),label2
	alc label1(3),12(,xr1)
	alc 10(4,xr1),11(,xr2)
	alc 10(4,1),11(,2)
	alc 13(5,xr2),label2
	mvc label1(20),label2
	a label2,wr4
	a 12(,xr2),wr5
	a 11(,xr1),wr6
	ali label2,128
	ali 14(,xr1),129
	ali 15(,xr2),130
	az label1(4),label2(3)
	az label1(5),12(4,xr1)
	az 10(6,xr1),11(5,xr2)
	az 13(7,xr2),label2(6)
	bc label1,1
	bc 1(,xr1),2
	bc 2(,xr2),3
	bc 2(,arr),3
	b label1
	bnz label2
	jc label1,2
	jc label2,2
	j label1
	j label2
	jnz label2
	src label2(5),6
	src 3(4,xr1),5
	src 4(5,xr2),6
	xfer 1,13
	svc 12,13
	bd label2
	mvx label1(1),label2
	mzz label1,label2
	mzn label1,label2
	mnz label1,label2
	mnn label1,label2
label2:

.end
