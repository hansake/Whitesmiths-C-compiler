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
;    1  /*      ECHO ARGUMENTS TO STDOUT
;    2   *      copyright (c) 1980, 1985 by Whitesmiths, Ltd.
;    3   */
;    4  #include <wslxa.h>
;    5  
;    6  /*      flags:
;    7          -m              output newline between arguments
;    8          -n              do not put newline at end of arguments
;    9   */
;   10  BOOL mflag = {NO};
.psect _data
 .byte (2)
;   11  BOOL nflag = {NO};
.public _nflag
_nflag:
 .byte (2)
;   12  
;   13  TEXT *_pname = {"echo"};
.public __pname
__pname:
 .addr L5
L5:
 .byte 085h, 083h, 088h, 096h, 000h
L51:
 .byte 094h, 06bh, 095h, 07ah, 0c6h, 040h, 04ch, 081h
 .byte 099h, 087h, 0a2h, 06eh, 000h
L53:
 .byte 040h, 000h
L52:
 .byte 025h, 000h
L54:
 .byte 025h, 000h
;   14  
;   15  /*      output args separated by space or newline
;   16   */
;   17  BOOL main(ac, av)
;   18          BYTES ac;
;   19          TEXT **av;
;   20          {
.psect _text
__text:
 la L03,xr2
 bd __cent
L03:
;   21          IMPORT BOOL mflag, nflag;
;   22          FAST COUNT n, ns, nw;
;   23          TEXT *between;
;   24  
;   25          getflags(&ac, &av, "m,n:F <args>", &mflag, &nflag);
 mvc 237(2,xr1),L4+1
 mvc 235(2,xr1),L6+1
 mvc 233(2,xr1),L01+1
 la 252(,xr1),wr4
 st __ac0+7,wr4
 mvc 231(2,xr1),__ac0+7
 la 250(,xr1),wr4
 st __ac0+7,wr4
 mvc 229(2,xr1),__ac0+7
 la -22,xr1
 b _getfla
;   26          if (!ac)
 clc 251(2,xr1),__const+3
 jne L1
;   27                  return (YES);
 mvi __ac0+6,0
 mvi __ac0+7,1
 b __cret
L1:
;line 28, bytes 99
;   28          between = mflag ? "\n" : " ";
 clc _mflag+1(2),__const+3
  je L21
 mvc __ac0+7(2),L41+1
 b L61
L21:
 mvc __ac0+7(2),L02+1
L61:
 mvc 239(2,xr1),__ac0+7
;   29          for (nw = 0, ns = 0; ac; --ac, ++av)
 mvi 240(,xr1),0
 mvi 241(,xr1),0
 mvi 242(,xr1),0
 mvi 243(,xr1),0
L11:
;line 29, bytes 172
 clc 251(2,xr1),__const+3
 je L12
;   30                  {
;   31                  if (nw)
 clc 241(2,xr1),__const+3
 je L15
;   32                          {
;   33                          nw += write(STDOUT, between, 1);
 mvi 236(,xr1),0
 mvi 237(,xr1),1
 mvc 235(2,xr1),239(,xr1)
 mvi 232(,xr1),0
 mvi 233(,xr1),1
 la -18,xr1
 b _write
 alc 241(2,xr1),__ac0+7
;   34                          ++ns;
 alc 243(2,xr1),__const+7
L15:
;line 36, bytes 254
;   35                          }
;   36                  nw += write(STDOUT, *av, n = lenstr(*av));
 l 253(,xr1),xr2
 mvc 235(2,xr1),1(,xr2)
 la -16,xr1
 b _lenstr
 mvc 245(2,xr1),__ac0+7
 mvc 237(2,xr1),245(,xr1)
 l 253(,xr1),xr2
 mvc 235(2,xr1),1(,xr2)
 mvi 232(,xr1),0
 mvi 233(,xr1),1
 la -18,xr1
 b _write
 alc 241(2,xr1),__ac0+7
;   37                  ns += n;
 alc 243(2,xr1),245(,xr1)
;   38                  }
 alc 251(2,xr1),__const-1
 alc 253(2,xr1),__const+11
 j L11
L12:
;line 38, bytes 353
;   39          if (!nflag)
 clc _nflag+1(2),__const+3
 jne L16
;   40                  {
;   41                  nw += write(STDOUT, "\n", 1);
 mvi 236(,xr1),0
 mvi 237(,xr1),1
 mvc 235(2,xr1),L22+1
 mvi 232(,xr1),0
 mvi 233(,xr1),1
 la -18,xr1
 b _write
 alc 241(2,xr1),__ac0+7
;   42                  ++ns;
 alc 243(2,xr1),__const+7
L16:
;line 44, bytes 428
;   43                  }
;   44          return (nw == ns);
 clc 241(2,xr1),243(,xr1)
  jne L42
 mvi __ac0+6,0
 mvi __ac0+7,1
 b L62
L42:
 mvi __ac0+6,0
 mvi __ac0+7,0
L62:
 b __cret
;   45          }
.external _lenstr
.external _write
.external _getfla
L22:
 .addr L54
L02:
 .addr L53
L41:
 .addr L52
L01:
 .addr L51
L6:
 .addr _mflag
L4:
 .addr _nflag
.external __cret
.external __cent
.public _mflag
.define _mflag = __data
.external __ac0
.external __fp
.external __xr2
.external __const
.end
