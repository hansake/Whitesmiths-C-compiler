 OPTIONS NOXREF
 HEADERS CATG-0
* C36 ASSEMBLER CODE, V3.12
MAIN START 0
 BD $TEXT
*    1  #define _CVERSION       300
*    2  #define _LVERSION       300
*    3  #define _FPPHARD
*    4  #define _EBCDIC
*    5  #define _OS     ssplim.h
*    6  #define _MACH   m36.h
*    1  /*      ECHO ARGUMENTS TO STDOUT
*    2   *      copyright (c) 1980, 1985 by Whitesmiths, Ltd.
*    3   */
*    4  #include <wslxa.h>
*    5  
*    6  /*      flags:
*    7          -m              output newline between arguments
*    8          -n              do not put newline at end of arguments
*    9   */
*   10  BOOL mflag = {NO};
$DATA EQU *
 DS 2CL1
*   11  BOOL nflag = {NO};
 ENTRY NFLAG
NFLAG EQU *
 DS 2CL1
*   12  
*   13  TEXT *_pname = {"echo"};
 ENTRY _PNAME
_PNAME EQU *
 DC AL2($5)
$5 EQU *
 DC XL5'8583889600'
$51 EQU *
 DC XL13'946B957AC6404C819987A26E00'
$53 EQU *
 DC XL2'4000'
$52 EQU *
 DC XL2'2500'
$54 EQU *
 DC XL2'2500'
*   14  
*   15  /*      output args separated by space or newline
*   16   */
*   17  BOOL main(ac, av)
*   18          BYTES ac;
*   19          TEXT **av;
*   20          {
$TEXT EQU *
 LA $03,#XR2
 BD @CENT
$03 EQU *
*   21          IMPORT BOOL mflag, nflag;
*   22          FAST COUNT n, ns, nw;
*   23          TEXT *between;
*   24  
*   25          getflags(&ac, &av, "m,n:F <args>", &mflag, &nflag);
 MVC 237(2,#XR1),$4+1
 MVC 235(2,#XR1),$6+1
 MVC 233(2,#XR1),$01+1
 LA 252(,#XR1),#WR4
 ST @AC0+7,#WR4
 MVC 231(2,#XR1),@AC0+7
 LA 250(,#XR1),#WR4
 ST @AC0+7,#WR4
 MVC 229(2,#XR1),@AC0+7
 LA -22,#XR1
 B GETFLA
*   26          if (!ac)
 CLC 251(2,#XR1),@CONST+3
 JNE $1
*   27                  return (YES);
 MVI @AC0+6,0
 MVI @AC0+7,1
 B @CRET
$1 EQU *
*line 28, bytes 99
*   28          between = mflag ? "\n" : " ";
 CLC MFLAG+1(2),@CONST+3
  JE $21
 MVC @AC0+7(2),$41+1
 B $61
$21 EQU *
 MVC @AC0+7(2),$02+1
$61 EQU *
 MVC 239(2,#XR1),@AC0+7
*   29          for (nw = 0, ns = 0; ac; --ac, ++av)
 MVI 240(,#XR1),0
 MVI 241(,#XR1),0
 MVI 242(,#XR1),0
 MVI 243(,#XR1),0
$11 EQU *
*line 29, bytes 172
 CLC 251(2,#XR1),@CONST+3
 JE $12
*   30                  {
*   31                  if (nw)
 CLC 241(2,#XR1),@CONST+3
 JE $15
*   32                          {
*   33                          nw += write(STDOUT, between, 1);
 MVI 236(,#XR1),0
 MVI 237(,#XR1),1
 MVC 235(2,#XR1),239(,#XR1)
 MVI 232(,#XR1),0
 MVI 233(,#XR1),1
 LA -18,#XR1
 B WRITE
 ALC 241(2,#XR1),@AC0+7
*   34                          ++ns;
 ALC 243(2,#XR1),@CONST+7
$15 EQU *
*line 36, bytes 254
*   35                          }
*   36                  nw += write(STDOUT, *av, n = lenstr(*av));
 L 253(,#XR1),#XR2
 MVC 235(2,#XR1),1(,#XR2)
 LA -16,#XR1
 B LENSTR
 MVC 245(2,#XR1),@AC0+7
 MVC 237(2,#XR1),245(,#XR1)
 L 253(,#XR1),#XR2
 MVC 235(2,#XR1),1(,#XR2)
 MVI 232(,#XR1),0
 MVI 233(,#XR1),1
 LA -18,#XR1
 B WRITE
 ALC 241(2,#XR1),@AC0+7
*   37                  ns += n;
 ALC 243(2,#XR1),245(,#XR1)
*   38                  }
 ALC 251(2,#XR1),@CONST-1
 ALC 253(2,#XR1),@CONST+11
 J $11
$12 EQU *
*line 38, bytes 353
*   39          if (!nflag)
 CLC NFLAG+1(2),@CONST+3
 JNE $16
*   40                  {
*   41                  nw += write(STDOUT, "\n", 1);
 MVI 236(,#XR1),0
 MVI 237(,#XR1),1
 MVC 235(2,#XR1),$22+1
 MVI 232(,#XR1),0
 MVI 233(,#XR1),1
 LA -18,#XR1
 B WRITE
 ALC 241(2,#XR1),@AC0+7
*   42                  ++ns;
 ALC 243(2,#XR1),@CONST+7
$16 EQU *
*line 44, bytes 428
*   43                  }
*   44          return (nw == ns);
 CLC 241(2,#XR1),243(,#XR1)
  JNE $42
 MVI @AC0+6,0
 MVI @AC0+7,1
 B $62
$42 EQU *
 MVI @AC0+6,0
 MVI @AC0+7,0
$62 EQU *
 B @CRET
*   45          }
 EXTRN LENSTR
 EXTRN WRITE
 EXTRN GETFLA
$22 EQU *
 DC AL2($54)
$02 EQU *
 DC AL2($53)
$41 EQU *
 DC AL2($52)
$01 EQU *
 DC AL2($51)
$6 EQU *
 DC AL2(MFLAG)
$4 EQU *
 DC AL2(NFLAG)
 EXTRN @CRET
 EXTRN @CENT
 ENTRY MFLAG
MFLAG EQU $DATA
#XR1 EQU X'01'
#XR2 EQU X'02'
#ARR EQU X'08'
#IAR EQU X'10'
#WR4 EQU X'44'
#WR5 EQU X'45'
#WR6 EQU X'46'
#WR7 EQU X'47'
 EXTRN @AC0
 EXTRN @FP
 EXTRN @XR2
 EXTRN @CONST
 END
