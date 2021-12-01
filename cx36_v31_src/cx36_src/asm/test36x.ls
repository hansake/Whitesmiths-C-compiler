   1                      	.processor s36
   2                      	
   3                      	label1:
   4    0000  0E010000009C		alc label1(2),label2
   5    0006  1E0200000C  		alc label1(3),12(,xr1)
   6    000B  6E030A0B    		alc 10(4,xr1),11(,xr2)
   7    000F  6E030A0B    		alc 10(4,1),11(,2)
   8    0013  8E040D009C  		alc 13(5,xr2),label2
   9    0018  0C130000009C		mvc label1(20),label2
  10    001E  3644009C    		a label2,wr4
  11    0022  B6450C      		a 12(,xr2),wr5
  12    0025  76460B      		a 11(,xr1),wr6
  13    0028  3F80009C    		ali label2,128
  14    002C  7F7F0E      		ali 14(,xr1),129
  15    002F  BF7E0F      		ali 15(,xr2),130
  16    0032  06120000009C		az label1(4),label2(3)
  17    0038  161300000C  		az label1(5),12(4,xr1)
  18    003D  66140A0B    		az 10(6,xr1),11(5,xr2)
  19    0041  86150D009C  		az 13(7,xr2),label2(6)
  20    0046  C0010000    		bc label1,1
  21    004A  D00201      		bc 1(,xr1),2
  22    004D  E00302      		bc 2(,xr2),3
  23    0050  F00302      		bc 2(,arr),3
  24    0053  C0870000    		b label1
  25    0057  C001009C    		bnz label2
  26    005B  F1025E      		jc label1,2
  27    005E  F2023B      		jc label2,2
  28    0061  F18764      		j label1
  29    0064  F28735      		j label2
  30    0067  F20132      		jnz label2
  31    006A  3E54009C    		src label2(5),6
  32    006E  7E4303      		src 3(4,xr1),5
  33    0071  BE5404      		src 4(5,xr2),6
  34    0074  F50D01      		xfer 1,13
  35    0077  F40D0C      		svc 12,13
  36    007A  C210009C    		bd label2
  37    007E  08010000009C		mvx label1(1),label2
  38    0084  08000000009C		mzz label1,label2
  39    008A  08010000009C		mzn label1,label2
  40    0090  08020000009C		mnz label1,label2
  41    0096  08030000009C		mnn label1,label2
  42                      	label2:
  43                      	
  44                      	.end
