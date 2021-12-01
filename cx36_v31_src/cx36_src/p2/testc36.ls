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
  12                      	;    1  #include <wslxa.h>
  13                      	;    2  #include <stdio.h>
  14                      	;    3  #include <fcntl.h>
  15                      	;    4  
  16                      	;    5  FD fd = {0};
  17                      	.psect _data
  18    0000  02          	 .byte (2)
  19                      	;    6  FILE *fp = {0};
  20                      	.public _fp
  21                      	_fp:
  22    0001  02          	 .byte (2)
  23                      	L5:
  24    0002  97          	 .byte 097h, 099h, 0a3h, 07ah, 000h
  25                      	L51:
  26    0007  88          	 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
  27    000F  99          	 .byte 099h, 093h, 084h, 07ah, 040h, 0f1h, 025h, 000h
  28                      	L52:
  29    0017  A6          	 .byte 0a6h, 000h
  30                      	L53:
  31    0019  88          	 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
  32    0021  99          	 .byte 099h, 093h, 084h, 07ah, 040h, 0f2h, 025h, 000h
  33                      	L54:
  34    0029  88          	 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
  35    0031  99          	 .byte 099h, 093h, 084h, 07ah, 040h, 0f3h, 025h, 000h
  36                      	L55:
  37    0039  88          	 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
  38    0041  99          	 .byte 099h, 093h, 084h, 07ah, 040h, 0f4h, 025h, 000h
  39                      	L56:
  40    0049  88          	 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
  41    0051  99          	 .byte 099h, 093h, 084h, 07ah, 040h, 06ch, 089h, 025h
  42    0059  00          	 .byte 000h
  43                      	L57:
  44    005A  88          	 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
  45    0062  99          	 .byte 099h, 093h, 084h, 07ah, 040h, 0f6h, 025h, 000h
  46                      	L501:
  47    006A  88          	 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
  48    0072  99          	 .byte 099h, 093h, 084h, 07ah, 040h, 06ch, 089h, 025h
  49    007A  00          	 .byte 000h
  50                      	;    7  
  51                      	;    8  BOOL main()
  52                      	;    9          {
  53                      	.psect _text
  54                      	__text:
  55    0004  C202000C    	 la L62,xr2
  56    0008  C2100000    	 bd __cent
  57                      	L62:
  58                      	;   10  
  59                      	;   11          fd = _open("prt:", O_WRONLY, 0, NULL );
  60    000C  4C01F50003  	 mvc 245(2,xr1),__const+3
  61    0011  7C00F2      	 mvi 242(,xr1),0
  62    0014  7C00F3      	 mvi 243(,xr1),0
  63    0017  7C00F0      	 mvi 240(,xr1),0
  64    001A  7C01F1      	 mvi 241(,xr1),1
  65    001D  4C01EF0106  	 mvc 239(2,xr1),L4+1
  66    0022  C201FFF4    	 la -12,xr1
  67    0026  C0870000    	 b __open
  68    002A  0C0100010007	 mvc _fd+1(2),__ac0+7
  69                      	;   12          _write(fd, "hello world: 1\n", 15 );
  70    0030  7C00F4      	 mvi 244(,xr1),0
  71    0033  7C0FF5      	 mvi 245(,xr1),15
  72    0036  4C01F30104  	 mvc 243(2,xr1),L6+1
  73    003B  4C01F10001  	 mvc 241(2,xr1),_fd+1
  74    0040  C201FFF6    	 la -10,xr1
  75    0044  C0870000    	 b __write
  76                      	;   13          fp = fdopen(fd, "w");
  77    0048  4C01F50102  	 mvc 245(2,xr1),L01+1
  78    004D  4C01F30001  	 mvc 243(2,xr1),_fd+1
  79    0052  C201FFF8    	 la -8,xr1
  80    0056  C0870000    	 b _fdopen
  81    005A  0C0101090007	 mvc _fp+1(2),__ac0+7
  82                      	;   14          fwrite("hello world: 2\n", 1, 15, fp);
  83    0060  4C01F50109  	 mvc 245(2,xr1),_fp+1
  84    0065  7C00F2      	 mvi 242(,xr1),0
  85    0068  7C0FF3      	 mvi 243(,xr1),15
  86    006B  7C00F0      	 mvi 240(,xr1),0
  87    006E  7C01F1      	 mvi 241(,xr1),1
  88    0071  4C01EF0100  	 mvc 239(2,xr1),L21+1
  89    0076  C201FFF4    	 la -12,xr1
  90    007A  C0870000    	 b _fwrite
  91                      	;   15          fputs("hello world: 3\n", fp);
  92    007E  4C01F50109  	 mvc 245(2,xr1),_fp+1
  93    0083  4C01F300FE  	 mvc 243(2,xr1),L41+1
  94    0088  C201FFF8    	 la -8,xr1
  95    008C  C0870000    	 b _fputs
  96                      	;   16          fprintf(fp, "hello world: 4\n");
  97    0090  4C01F500FC  	 mvc 245(2,xr1),L61+1
  98    0095  4C01F30109  	 mvc 243(2,xr1),_fp+1
  99    009A  C201FFF8    	 la -8,xr1
 100    009E  C0870000    	 b _fprint
 101                      	;   17          fprintf(fp, "hello world: %i\n", 5);
 102    00A2  7C00F4      	 mvi 244(,xr1),0
 103    00A5  7C05F5      	 mvi 245(,xr1),5
 104    00A8  4C01F300FA  	 mvc 243(2,xr1),L02+1
 105    00AD  4C01F10109  	 mvc 241(2,xr1),_fp+1
 106    00B2  C201FFF6    	 la -10,xr1
 107    00B6  C0870000    	 b _fprint
 108                      	;   18          putf(fp, "hello world: 6\n");
 109    00BA  4C01F500F8  	 mvc 245(2,xr1),L22+1
 110    00BF  4C01F30109  	 mvc 243(2,xr1),_fp+1
 111    00C4  C201FFF8    	 la -8,xr1
 112    00C8  C0870000    	 b _putf
 113                      	;   19          putf(fp, "hello world: %i\n", 7);
 114    00CC  7C00F4      	 mvi 244(,xr1),0
 115    00CF  7C07F5      	 mvi 245(,xr1),7
 116    00D2  4C01F300F6  	 mvc 243(2,xr1),L42+1
 117    00D7  4C01F10109  	 mvc 241(2,xr1),_fp+1
 118    00DC  C201FFF6    	 la -10,xr1
 119    00E0  C0870000    	 b _putf
 120                      	;   20          fclose(fp);
 121    00E4  4C01F50109  	 mvc 245(2,xr1),_fp+1
 122    00E9  C201FFFA    	 la -6,xr1
 123    00ED  C0870000    	 b _fclose
 124                      	;   21          }
 125    00F1  C0870000    	 b __cret
 126                      	.external _putf
 127                      	.external __write
 128                      	.external __open
 129                      	.external _fwrite
 130                      	.external _fputs
 131                      	.external _fprint
 132                      	.external _fclose
 133                      	.external _fdopen
 134                      	L42:
 135    00F5  0171        	 .addr L501
 136                      	L22:
 137    00F7  0161        	 .addr L57
 138                      	L02:
 139    00F9  0150        	 .addr L56
 140                      	L61:
 141    00FB  0140        	 .addr L55
 142                      	L41:
 143    00FD  0130        	 .addr L54
 144                      	L21:
 145    00FF  0120        	 .addr L53
 146                      	L01:
 147    0101  011E        	 .addr L52
 148                      	L6:
 149    0103  010E        	 .addr L51
 150                      	L4:
 151    0105  0109        	 .addr L5
 152                      	.external __cret
 153                      	.external __cent
 154                      	.public _fd
 155                      	.define _fd = __data
 156                      	.external __ac0
 157                      	.external __fp
 158                      	.external __xr2
 159                      	.external __const
 160                      	.end
