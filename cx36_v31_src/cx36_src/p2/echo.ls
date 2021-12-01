   1                      	.processor s36
   2                      	; HEADERS CATG-0
   3                      	; c36 assembler code, v3.12
   4                      	_main:
   5    0000  C2100004    	 bd __text
   6                      	;    1  #define _CVERSION       300
   7                      	;    2  #define _LVERSION       300
   8                      	;    3  #define _FPPHARD
   9                      	;    4  #define _EBCDIC
  10                      	;    5  #define _OS     ssplim.h
  11                      	;    6  #define _MACH   m36.h
  12                      	;    1  /*      ECHO ARGUMENTS TO STDOUT
  13                      	;    2   *      copyright (c) 1980, 1985 by Whitesmiths, Ltd.
  14                      	;    3   */
  15                      	;    4  #include <wslxa.h>
  16                      	;    5  
  17                      	;    6  /*      flags:
  18                      	;    7          -m              output newline between arguments
  19                      	;    8          -n              do not put newline at end of arguments
  20                      	;    9   */
  21                      	;   10  BOOL mflag = {NO};
  22                      	.psect _data
  23    0000  02          	 .byte (2)
  24                      	;   11  BOOL nflag = {NO};
  25                      	.public _nflag
  26                      	_nflag:
  27    0001  02          	 .byte (2)
  28                      	;   12  
  29                      	;   13  TEXT *_pname = {"echo"};
  30                      	.public __pname
  31                      	__pname:
  32    0002  0149        	 .addr L5
  33                      	L5:
  34    0004  85          	 .byte 085h, 083h, 088h, 096h, 000h
  35                      	L51:
  36    0009  94          	 .byte 094h, 06bh, 095h, 07ah, 0c6h, 040h, 04ch, 081h
  37    0011  99          	 .byte 099h, 087h, 0a2h, 06eh, 000h
  38                      	L53:
  39    0016  40          	 .byte 040h, 000h
  40                      	L52:
  41    0018  25          	 .byte 025h, 000h
  42                      	L54:
  43    001A  25          	 .byte 025h, 000h
  44                      	;   14  
  45                      	;   15  /*      output args separated by space or newline
  46                      	;   16   */
  47                      	;   17  BOOL main(ac, av)
  48                      	;   18          BYTES ac;
  49                      	;   19          TEXT **av;
  50                      	;   20          {
  51                      	.psect _text
  52                      	__text:
  53    0004  C202000C    	 la L03,xr2
  54    0008  C2100000    	 bd __cent
  55                      	L03:
  56                      	;   21          IMPORT BOOL mflag, nflag;
  57                      	;   22          FAST COUNT n, ns, nw;
  58                      	;   23          TEXT *between;
  59                      	;   24  
  60                      	;   25          getflags(&ac, &av, "m,n:F <args>", &mflag, &nflag);
  61    000C  4C01ED0144  	 mvc 237(2,xr1),L4+1
  62    0011  4C01EB0142  	 mvc 235(2,xr1),L6+1
  63    0016  4C01E90140  	 mvc 233(2,xr1),L01+1
  64    001B  0244FC      	 la 252(,xr1),wr4
  65    001E  34440007    	 st __ac0+7,wr4
  66    0022  4C01E70007  	 mvc 231(2,xr1),__ac0+7
  67    0027  0244FA      	 la 250(,xr1),wr4
  68    002A  34440007    	 st __ac0+7,wr4
  69    002E  4C01E50007  	 mvc 229(2,xr1),__ac0+7
  70    0033  C201FFEA    	 la -22,xr1
  71    0037  C0870000    	 b _getfla
  72                      	;   26          if (!ac)
  73    003B  4D01FB0003  	 clc 251(2,xr1),__const+3
  74    0040  F2010C      	 jne L1
  75                      	;   27                  return (YES);
  76    0043  3C000006    	 mvi __ac0+6,0
  77    0047  3C010007    	 mvi __ac0+7,1
  78    004B  C0870000    	 b __cret
  79                      	L1:
  80                      	;line 28, bytes 99
  81                      	;   28          between = mflag ? "\n" : " ";
  82    004F  0D0100010003	 clc _mflag+1(2),__const+3
  83    0055  F2810A      	  je L21
  84    0058  0C010007013E	 mvc __ac0+7(2),L41+1
  85    005E  C0870068    	 b L61
  86                      	L21:
  87    0062  0C010007013C	 mvc __ac0+7(2),L02+1
  88                      	L61:
  89    0068  4C01EF0007  	 mvc 239(2,xr1),__ac0+7
  90                      	;   29          for (nw = 0, ns = 0; ac; --ac, ++av)
  91    006D  7C00F0      	 mvi 240(,xr1),0
  92    0070  7C00F1      	 mvi 241(,xr1),0
  93    0073  7C00F2      	 mvi 242(,xr1),0
  94    0076  7C00F3      	 mvi 243(,xr1),0
  95                      	L11:
  96                      	;line 29, bytes 172
  97    0079  4D01FB0003  	 clc 251(2,xr1),__const+3
  98    007E  F2816D      	 je L12
  99                      	;   30                  {
 100                      	;   31                  if (nw)
 101    0081  4D01F10003  	 clc 241(2,xr1),__const+3
 102    0086  F28122      	 je L15
 103                      	;   32                          {
 104                      	;   33                          nw += write(STDOUT, between, 1);
 105    0089  7C00EC      	 mvi 236(,xr1),0
 106    008C  7C01ED      	 mvi 237(,xr1),1
 107    008F  5C01EBEF    	 mvc 235(2,xr1),239(,xr1)
 108    0093  7C00E8      	 mvi 232(,xr1),0
 109    0096  7C01E9      	 mvi 233(,xr1),1
 110    0099  C201FFEE    	 la -18,xr1
 111    009D  C0870000    	 b _write
 112    00A1  4E01F10007  	 alc 241(2,xr1),__ac0+7
 113                      	;   34                          ++ns;
 114    00A6  4E01F30007  	 alc 243(2,xr1),__const+7
 115                      	L15:
 116                      	;line 36, bytes 254
 117                      	;   35                          }
 118                      	;   36                  nw += write(STDOUT, *av, n = lenstr(*av));
 119    00AB  7502FD      	 l 253(,xr1),xr2
 120    00AE  6C01EB01    	 mvc 235(2,xr1),1(,xr2)
 121    00B2  C201FFF0    	 la -16,xr1
 122    00B6  C0870000    	 b _lenstr
 123    00BA  4C01F50007  	 mvc 245(2,xr1),__ac0+7
 124    00BF  5C01EDF5    	 mvc 237(2,xr1),245(,xr1)
 125    00C3  7502FD      	 l 253(,xr1),xr2
 126    00C6  6C01EB01    	 mvc 235(2,xr1),1(,xr2)
 127    00CA  7C00E8      	 mvi 232(,xr1),0
 128    00CD  7C01E9      	 mvi 233(,xr1),1
 129    00D0  C201FFEE    	 la -18,xr1
 130    00D4  C0870000    	 b _write
 131    00D8  4E01F10007  	 alc 241(2,xr1),__ac0+7
 132                      	;   37                  ns += n;
 133    00DD  5E01F3F5    	 alc 243(2,xr1),245(,xr1)
 134                      	;   38                  }
 135    00E1  4E01FBFFFF  	 alc 251(2,xr1),__const-1
 136    00E6  4E01FD000B  	 alc 253(2,xr1),__const+11
 137    00EB  F18775      	 j L11
 138                      	L12:
 139                      	;line 38, bytes 353
 140                      	;   39          if (!nflag)
 141    00EE  0D0101470003	 clc _nflag+1(2),__const+3
 142    00F4  F20123      	 jne L16
 143                      	;   40                  {
 144                      	;   41                  nw += write(STDOUT, "\n", 1);
 145    00F7  7C00EC      	 mvi 236(,xr1),0
 146    00FA  7C01ED      	 mvi 237(,xr1),1
 147    00FD  4C01EB013A  	 mvc 235(2,xr1),L22+1
 148    0102  7C00E8      	 mvi 232(,xr1),0
 149    0105  7C01E9      	 mvi 233(,xr1),1
 150    0108  C201FFEE    	 la -18,xr1
 151    010C  C0870000    	 b _write
 152    0110  4E01F10007  	 alc 241(2,xr1),__ac0+7
 153                      	;   42                  ++ns;
 154    0115  4E01F30007  	 alc 243(2,xr1),__const+7
 155                      	L16:
 156                      	;line 44, bytes 428
 157                      	;   43                  }
 158                      	;   44          return (nw == ns);
 159    011A  5D01F1F3    	 clc 241(2,xr1),243(,xr1)
 160    011E  F2010C      	  jne L42
 161    0121  3C000006    	 mvi __ac0+6,0
 162    0125  3C010007    	 mvi __ac0+7,1
 163    0129  C0870135    	 b L62
 164                      	L42:
 165    012D  3C000006    	 mvi __ac0+6,0
 166    0131  3C000007    	 mvi __ac0+7,0
 167                      	L62:
 168    0135  C0870000    	 b __cret
 169                      	;   45          }
 170                      	.external _lenstr
 171                      	.external _write
 172                      	.external _getfla
 173                      	L22:
 174    0139  015F        	 .addr L54
 175                      	L02:
 176    013B  015B        	 .addr L53
 177                      	L41:
 178    013D  015D        	 .addr L52
 179                      	L01:
 180    013F  014E        	 .addr L51
 181                      	L6:
 182    0141  0000        	 .addr _mflag
 183                      	L4:
 184    0143  0146        	 .addr _nflag
 185                      	.external __cret
 186                      	.external __cent
 187                      	.public _mflag
 188                      	.define _mflag = __data
 189                      	.external __ac0
 190                      	.external __fp
 191                      	.external __xr2
 192                      	.external __const
 193                      	.end
