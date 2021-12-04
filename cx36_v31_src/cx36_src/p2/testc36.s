.processor s36
; HEADERS CATG-0
; c36 assembler code, v3.12
_main:
 bd __text
;    1  #define _CVERSION       300
;    2  #define _LVERSION       300
;    3  #define _FPPHARD
;    4  #define _EBCDIC
;    5  #define _OS     ssplim.h
;    6  #define _MACH   m36.h
;    1  #include <wslxa.h>
;    2  #include <stdio.h>
;    3  #include <fcntl.h>
;    4  
;    5  FD fd = {0};
.psect _data
 .byte (2)
;    6  FILE *fp = {0};
.public _fp
_fp:
 .byte (2)
L5:
 .byte 097h, 099h, 0a3h, 07ah, 000h
L51:
 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
 .byte 099h, 093h, 084h, 07ah, 040h, 0f1h, 025h, 000h
L52:
 .byte 0a6h, 000h
L53:
 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
 .byte 099h, 093h, 084h, 07ah, 040h, 0f2h, 025h, 000h
L54:
 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
 .byte 099h, 093h, 084h, 07ah, 040h, 0f3h, 025h, 000h
L55:
 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
 .byte 099h, 093h, 084h, 07ah, 040h, 0f4h, 025h, 000h
L56:
 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
 .byte 099h, 093h, 084h, 07ah, 040h, 06ch, 089h, 025h
 .byte 000h
L57:
 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
 .byte 099h, 093h, 084h, 07ah, 040h, 0f6h, 025h, 000h
L501:
 .byte 088h, 085h, 093h, 093h, 096h, 040h, 0a6h, 096h
 .byte 099h, 093h, 084h, 07ah, 040h, 06ch, 089h, 025h
 .byte 000h
;    7  
;    8  BOOL main()
;    9          {
.psect _text
__text:
 la L62,xr2
 bd __cent
L62:
;   10  
;   11          fd = _open("prt:", O_WRONLY, 0, NULL );
 mvc 245(2,xr1),__const+3
 mvi 242(,xr1),0
 mvi 243(,xr1),0
 mvi 240(,xr1),0
 mvi 241(,xr1),1
 mvc 239(2,xr1),L4+1
 la -12,xr1
 b __open
 mvc _fd+1(2),__ac0+7
;   12          _write(fd, "hello world: 1\n", 15 );
 mvi 244(,xr1),0
 mvi 245(,xr1),15
 mvc 243(2,xr1),L6+1
 mvc 241(2,xr1),_fd+1
 la -10,xr1
 b __write
;   13          fp = fdopen(fd, "w");
 mvc 245(2,xr1),L01+1
 mvc 243(2,xr1),_fd+1
 la -8,xr1
 b _fdopen
 mvc _fp+1(2),__ac0+7
;   14          fwrite("hello world: 2\n", 1, 15, fp);
 mvc 245(2,xr1),_fp+1
 mvi 242(,xr1),0
 mvi 243(,xr1),15
 mvi 240(,xr1),0
 mvi 241(,xr1),1
 mvc 239(2,xr1),L21+1
 la -12,xr1
 b _fwrite
;   15          fputs("hello world: 3\n", fp);
 mvc 245(2,xr1),_fp+1
 mvc 243(2,xr1),L41+1
 la -8,xr1
 b _fputs
;   16          fprintf(fp, "hello world: 4\n");
 mvc 245(2,xr1),L61+1
 mvc 243(2,xr1),_fp+1
 la -8,xr1
 b _fprint
;   17          fprintf(fp, "hello world: %i\n", 5);
 mvi 244(,xr1),0
 mvi 245(,xr1),5
 mvc 243(2,xr1),L02+1
 mvc 241(2,xr1),_fp+1
 la -10,xr1
 b _fprint
;   18          putf(fp, "hello world: 6\n");
 mvc 245(2,xr1),L22+1
 mvc 243(2,xr1),_fp+1
 la -8,xr1
 b _putf
;   19          putf(fp, "hello world: %i\n", 7);
 mvi 244(,xr1),0
 mvi 245(,xr1),7
 mvc 243(2,xr1),L42+1
 mvc 241(2,xr1),_fp+1
 la -10,xr1
 b _putf
;   20          fclose(fp);
 mvc 245(2,xr1),_fp+1
 la -6,xr1
 b _fclose
;   21          }
 b __cret
.external _putf
.external __write
.external __open
.external _fwrite
.external _fputs
.external _fprint
.external _fclose
.external _fdopen
L42:
 .addr L501
L22:
 .addr L57
L02:
 .addr L56
L61:
 .addr L55
L41:
 .addr L54
L21:
 .addr L53
L01:
 .addr L52
L6:
 .addr L51
L4:
 .addr L5
.external __cret
.external __cent
.public _fd
.define _fd = __data
.external __ac0
.external __fp
.external __xr2
.external __const
.end
